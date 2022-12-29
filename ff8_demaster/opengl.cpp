// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr


#include "opengl.h"
#ifdef APIENTRY
#undef APIENTRY
#endif
#include "config.h"
#include "debug.h"
#include "texture.h"
#include "minhook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>


void CheckGlew()
{
    if (glewInitialized) return;

    glewInitialized = true;
    if (const GLenum err = glewInit(); GLEW_OK != err)
        /* Problem: glewInit failed, something is seriously wrong. */
        OutputDebug("%s - GLEW Error: %s\n", __func__, glewGetErrorString(err));
}

int HookGlfwInit()
{
    //int testInit = glfwInit(); //this creates glfw on our thread - nice!
    int testInit = static_cast<int(*)()>(glfwInitTrampoline)();
    return testInit;
}


int ffWindowWidth, ffWindowHeight;
GLFWwindow* HookGlfwWindow(const int width, const int height, const char* title, GLFWmonitor* monitor,
    GLFWwindow* share)
{
    ffWindow = static_cast<GLFWwindow*(*)(int, int, const char*, GLFWmonitor*, GLFWwindow*)>(glfwWindowTrampoline)
    (width, height, title, monitor, share);
    if(ffWindow == nullptr)
    {
        ffWindow = glfwCreateWindow(width, height, title, monitor, share);
    }
    ffWindowWidth = width;
    ffWindowHeight = height;
    OutputDebug("OpenGL version: %s", glGetString(GL_VERSION));
    /*
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    ffWindowSecond = static_cast<GLFWwindow*(*)(int, int, const char*, GLFWmonitor*, GLFWwindow*)>(glfwWindowTrampoline)
    (width, height, title, monitor, ffWindow);
    */
    return ffWindow;
}
ImGuiContext * imguiContext;
void HookGlFunctionsPostGLFW()
{
    IMGUI_CHECKVERSION();
    imguiContext = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    if(!ImGui_ImplGlfw_InitForOpenGL(ffWindow, true))
    {
        OutputDebug("Error initializing ImGui");
        return;
    }
    if(!ImGui_ImplOpenGL3_Init("#version 130"))
    {
        OutputDebug("Error initializing ImGui");
        return;
    }
    return;
}
ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
void imgui_test()
{

}

inline bool bStartedSwapBuffers = false;

LPVOID SwapBuffersTrampoline;
BOOL* __stdcall HookSwapBuffers(const HDC hdc)
{
    if(IMGUI_DEBUG)
    {
        bStartedSwapBuffers = true;
        glViewport(0, 0, ffWindowWidth, ffWindowHeight);
        ImDrawData* drawData = ImGui::GetDrawData();
        drawData->DisplaySize.x = static_cast<float>(ffWindowWidth);
        drawData->DisplaySize.y = static_cast<float>(ffWindowHeight);

        
        for(int cmdListIdx=0; cmdListIdx<drawData->CmdListsCount; cmdListIdx++)
        {
            for(ImDrawList* cmdList = drawData->CmdLists[cmdListIdx]; ImDrawCmd&drawCmd : cmdList->CmdBuffer)
            {
                drawCmd.ClipRect.z = static_cast<float>(ffWindowWidth);
                drawCmd.ClipRect.w = static_cast<float>(ffWindowHeight);
            }
        }
        
        
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
        bStartedSwapBuffers = false;
    }
    return static_cast<BOOL* (__stdcall*)(HDC)>(SwapBuffersTrampoline)(hdc);
}

float testFloat = 0.0f;

void* __stdcall HookGlViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
{
    CheckGlew();
    if (static bool backdoorUsed = false; glewInitialized && !backdoorUsed)
    {
        OutputDebug("OpenGL version: %s", glGetString(GL_VERSION));
        //Maki: PUT BACKDOORS FOR EXTENSIONS HERE
        if (TEXTURE_PATCH && DIRECT_IO)
        {
            MH_CreateHookApi(L"OPENGL32", "glBindTexture", HookGlBindTexture, &ogl_bind_texture);
            MH_CreateHookApi(L"OPENGL32", "glTexParameteri", HookGlTextParameteri, &ogl_tex_parametri);
            MH_CreateHookApi(L"OPENGL32", "glTexImage2D", HookGlTexImage2D, &ogl_tex_image2d);
            //create mh hook for swapbuffers
            MH_CreateHook(&SwapBuffers, &HookSwapBuffers, &SwapBuffersTrampoline);
            MH_EnableHook(MH_ALL_HOOKS);
            HookGlFunctionsPostGLFW();
        }
        backdoorUsed = true;
    }
    if(IMGUI_DEBUG && !bStartedSwapBuffers)
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("DEMASTER DEBUG WINDOW");
        ImGui::Text("Looks like I'm working!");
        ImGui::End();

        ImGui::Begin("SECOND WINDOW TEST");
        ImGui::Text("Sample text");
        ImGui::DragFloat("sample float test", &testFloat, 0.1f);
        ImGui::End();

        ImGui::Render();
    }
    return static_cast<void* (__stdcall*)(GLint, GLint, GLsizei, GLsizei)>(oglViewport)(x, y, width, height);
}

[[maybe_unused]]
void __stdcall KglClear(const GLbitfield mask)
{
    oglClear(mask);
}

[[maybe_unused]]
void __stdcall KglClearColor(const GLclampf red, const GLclampf green, const GLclampf blue, const GLclampf alpha)
{
    //_oglClearColor(red, green, 1.0f, 1.0f); //Maki: Try it :D
    oglClearColor(red, green, blue, alpha);

}

[[maybe_unused]]
void __stdcall KoglBufferData(const GLenum target, const GLsizeiptr size, const void* data, const GLenum usage)
{
    oglBufferData(target, size, data, usage);
}