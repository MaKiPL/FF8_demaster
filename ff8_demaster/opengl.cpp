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
#include <GLFW/glfw3.h>
#include "coreHeader.h"
#include "imgui/backends/imgui_impl_win32.h"
#include <filesystem>

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
    //int glfwInit_ = glfwInit();
    int initGlfw = static_cast<int(*)()>(glfwInitTrampoline)();
    return initGlfw;
}


int ffWindowWidth, ffWindowHeight;

GLFWwindow* HookGlfwWindow(const int width, const int height, const char* title, GLFWmonitor* monitor,
                           GLFWwindow* share)
{
    
    //GLFWwindow* ffWindow = nullptr;
    ffWindow = static_cast<GLFWwindow*(*)(int, int, const char*, GLFWmonitor*, GLFWwindow*)>(glfwWindowTrampoline)
    (width, height, title, monitor, share);
    if(ffWindow == nullptr)
    {
        ffWindow = glfwCreateWindow(width, height, title, monitor, share);
    }
    // int testa;
    // int testb;
    // glfwGetWindowSize(ffWindow, &testa, &testb);
    // GLFWwindow * currentContext = glfwGetCurrentContext();
    // OutputDebug("currentContext: %p", currentContext);
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
    if(!ImGui_ImplWin32_Init(GetForegroundWindow()))
    {
        
    }
    if(!ImGui_ImplOpenGL3_Init("#version 130"))
    {
        OutputDebug("Error initializing ImGui");
        return;
    }
    
}
inline bool bStartedSwapBuffers = false;

LPVOID swapBuffersTrampoline;

void RegisterImguiInput(int button, bool down)
{
if(IMGUI_DEBUG)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, down);
}

}

//You might ask- why? Well.. I don't even know :D
#define CREATE_PATH_OR_RETURN(imgNumber, varName, extension) __CREATE_SAVEPATH(varName) \
__PATH_DIRS_OR_RET(varName) varName += std::to_string(imgNumber) + extension;
#define __CREATE_SAVEPATH(varName) std::filesystem::path varName = std::filesystem::current_path() \
   / DIRECT_IO_EXPORT_DIR / "textures" / "SavedTextures" / "img";
#define __PATH_DIRS_OR_RET(varName) if(!std::filesystem::exists(varName)) \
    if(!std::filesystem::create_directories(varName)) { \
    OutputDebug("MakiImGui save texture failed, because path %s could not be created", (varName).string().c_str());\
        return;}

void MakiIm_SaveTexture(const GLuint selectedTextureId, const int width, const int height)
{
    CREATE_PATH_OR_RETURN(selectedTextureId, texSavePath, ".png")
    BYTE* pixelBuffer = new BYTE[width * height * 4];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
    //glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixelBuffer);
    bx::FileWriter writer;
    
    if (bx::open(&writer, bx::FilePath(texSavePath.string().c_str())))
    {
        const int32_t bytesWritten =  bimg::imageWritePng(&writer, width, height,
            width * 4, pixelBuffer, bimg::TextureFormat::RGBA8,false);
        OutputDebug("MakiImGui::Saved %d bytes of image(%d) to %s\n", bytesWritten,
            selectedTextureId, texSavePath.string().c_str());
    }
}

void MakiIm_ReplaceTexture(const GLuint texId)
{
    //texture is already bound!
    CREATE_PATH_OR_RETURN(texId, texSavePath, ".png");
    if(!std::filesystem::exists(texSavePath))
    {
        OutputDebug("File for replacement texture %s does not exist",
            texSavePath.string().c_str());
        return;
    }
    const SafeBimg texReplacement = LoadImageFromFile(texSavePath.string().c_str());
    
    RenderTexture(texReplacement.get());
}

void MakiImGui_DrawActiveTextures(const float w, const float h)
{
    ImGui::SetNextWindowSize(ImVec2(w,h), ImGuiCond_Once);
    ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);
    ImGui::Begin("DEMASTER::ACTIVE TEXTURES");
    ImGui::Text("Active bound textures: %d", activeTextures.size());
    for(const GLuint& texture : activeTextures)
    {
        constexpr float imageResolution = 128.0f;
        constexpr float width = 128.0f;
        constexpr float rowCount = 2.0f;
        glBindTexture(GL_TEXTURE_2D,texture);
        int w, h;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);


        ImGui::Text("Texture: %d",texture);
        ImGui::SameLine();
        ImGui::Image(reinterpret_cast<ImTextureID>(texture),
                     ImVec2(imageResolution,imageResolution),
                     ImVec2(0,0), ImVec2(1,1));
        ImGui::SameLine();
        if(ImGui::Button(("Save as PNG##" + std::to_string(texture)).c_str()))
            MakiIm_SaveTexture(texture, w, h);
        ImGui::SameLine();
        if(ImGui::Button(("Replace texture##" + std::to_string(texture)).c_str()))
            MakiIm_ReplaceTexture(texture);
        ImGui::Text("%dx%d",w, h);
    }
    ImGui::End();
}

/**
 * \brief This hook intercepts the wglSwapBuffers call for operations before the actual Windows API SwapBuffers(HDC)
 * \param hdc as in SwapBuffers(HDC)
 * \return as in SwapBuffers(HDC)
 */
BOOL* __stdcall HookSwapBuffers(const HDC hdc)
{
    if(IMGUI_DEBUG)
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //RegisterImguiInput(0, true);
        ImGuiIO &io = ImGui::GetIO();

        
        int vk = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;
        if(GetAsyncKeyState(vk) < 0)
        {
            io.AddMouseButtonEvent(ImGuiMouseButton_Left, true);
        }
        else
        {
            io.AddMouseButtonEvent(ImGuiMouseButton_Left, false);
        }
        
        
        MakiImGui_DrawActiveTextures(400,800);

        
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
        bStartedSwapBuffers = false;
    }
    
    return static_cast<BOOL* (__stdcall*)(HDC)>(swapBuffersTrampoline)(hdc);
}

LPVOID glewUseProgramTrampoline;
void HookGlUseProgram(const GLuint program)
{
    OutputDebug("\nglUseProgram: %d", program);
    return static_cast<void(__stdcall*)(GLuint)>(glewUseProgramTrampoline)(program);
}

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
        MH_CreateHookApi(L"OPENGL32", "glDeleteTextures", HookGlDeleteTextures, &ogl_deleteTexture);
        MH_CreateHookApi(L"OPENGL32", "glEnable", HookGlEnable, &ogl_enable);
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

void* __stdcall HookGlEnable(GLenum cap)
{
    GLenum returnCap = cap;
    if(cap==GL_SCISSOR_TEST && FILL_ASPECT_RATIO)
    {
        returnCap = -1;
        glDisable(GL_SCISSOR_TEST);
    }
        
    return static_cast<void* (__stdcall*)(GLenum)>(ogl_enable)(returnCap);
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