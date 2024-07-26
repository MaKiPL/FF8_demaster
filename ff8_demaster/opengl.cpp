// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr


#include "coreHeader.h"
#include "opengl.h"

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
    return ffWindow;
}
ImGuiContext * imguiContext;
void CreateImGuiImplementation()
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
    
}

GLuint GetCurrentBoundTextureID()
{
    GLint currentTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    return static_cast<GLuint>(currentTexture);
}

inline bool bStartedSwapBuffers = false;

LPVOID swapBuffersTrampoline;

/**
 * \brief This hook intercepts the wglSwapBuffers call for operations before the actual Windows API SwapBuffers(HDC)
 * \param hdc as in SwapBuffers(HDC)
 * \return as in SwapBuffers(HDC)
 */
BOOL* __stdcall HookSwapBuffers(const HDC hdc)
{
    if(IMGUI_DEBUG)
    {
        bStartedSwapBuffers = true;
        //glViewport(0, 0, ffWindowWidth, ffWindowHeight);
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

    //add your logic here
    
    return static_cast<BOOL* (__stdcall*)(HDC)>(swapBuffersTrampoline)(hdc);
}

LPVOID glewUseProgramTrampoline;
void HookGlUseProgram(const GLuint program)
{
    OutputDebug("\nglUseProgram: %d", program);
    return static_cast<void(__stdcall*)(GLuint)>(glewUseProgramTrampoline)(program);
}

float testFloat = 0.0f;

static GLsizei firstWidth, firstHeight;

/**
 * \brief This works closely with glViewport. It's responsible for making additional openGL hooks and injections
 * after the game have correctly created GLFW window, correctly passed glewInit and is fully ready to use openGL.
 * \param backdoorUsed reference to static variable inside glViewport
 */
void ViewportBackdoorInject(bool& backdoorUsed)
{
    OutputDebug("OpenGL version: %s", glGetString(GL_VERSION));
    //Maki: PUT BACKDOORS FOR EXTENSIONS HERE
    if (TEXTURE_PATCH && DIRECT_IO)
    {
        MH_CreateHookApi(L"OPENGL32", "glBindTexture", HookGlBindTexture, &ogl_bind_texture);
        MH_CreateHookApi(L"OPENGL32", "glTexParameteri", HookGlTextParameteri, &ogl_tex_parametri);
        //creating api hook is probably mandatory. Other way is to hook the entry of the api, but idk
        MH_CreateHookApi(L"OPENGL32", "glTexImage2D", HookGlTexImage2D, &ogl_tex_image2d);
        MH_CreateHookApi(L"OPENGL32", "glTexSubImage2D", HookGlTexSubImage2D, &ogl_subTexImage2D);
        MH_CreateHookApi(L"OPENGL32", "glTextureSubImage2D", HookGlTextureSubImage2D,
                         &ogl_subTextureImage2D);
    }
    MH_CreateHook(&SwapBuffers, &HookSwapBuffers, &swapBuffersTrampoline);
    MH_EnableHook(MH_ALL_HOOKS);

    if(IMGUI_DEBUG)
        CreateImGuiImplementation();

    //leave below as final line
    backdoorUsed = true;
}

void GetBackBufferPixels(const GLsizei width, const GLsizei height)
{
    //Working code to get last backBuffer into file
    if(frame.pixels == nullptr)
    {
        frame.channels = 4;
        frame.width = width;
        frame.height = height;
        frame.pixels = new char[frame.width * frame.height * frame.channels];
    }
    glReadPixels(0,0,frame.width,frame.height,GL_RGBA,GL_UNSIGNED_BYTE,frame.pixels);
    if(glfwGetKey(ffWindow, GLFW_KEY_F2) == GLFW_PRESS)
    {
        File file("frame.bin", FileOpenMode::write);
        file.WriteBuffer(frame.pixels, frame.width * frame.height * frame.channels);
        file.Close();
    }
}

void* __stdcall HookGlViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
{
    CheckGlew();
    if (static bool backdoorUsed = false; glewInitialized && !backdoorUsed)
        ViewportBackdoorInject(backdoorUsed);


    //GetBackBufferPixels(width, height);
    
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
    GLsizei finalX = x;
    GLsizei finalY = y;
    GLsizei finalWidth = width;
    GLsizei finalHeight = height;

    if(FILL_ASPECT_RATIO)
        if(x>=32)
        {
            finalX = finalY = 0;
            finalWidth = ffWindowWidth;
            finalHeight = ffWindowHeight;
        }

    return static_cast<void* (__stdcall*)(GLint, GLint, GLsizei, GLsizei)>(oglViewport)
    (finalX, finalY, finalWidth, finalHeight);
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