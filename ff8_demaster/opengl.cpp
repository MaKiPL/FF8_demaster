// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr


#include "coreHeader.h"
#include "opengl.h"

#include <filesystem>

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

// int __fastcall HookGameClockUpdate(int a1, int a2)
// {
//     if (bPaused)
//     {
//         return 0;
//     }
//     return pGameClockUpdateTrampoline(a1,a2);
// }

void PumpMessages()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void UpdateImGuiIO(HDC hdc, float ffWindowWidth, float ffWindowHeight)
{
    glfwPollEvents();
    
    HWND hwnd = WindowFromDC(hdc);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(ffWindowWidth, ffWindowHeight);
    
    static INT64 g_Time = 0;
    static INT64 g_TicksPerSecond = 0;
    if (!g_TicksPerSecond)
    {
        QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&g_TicksPerSecond));
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&g_Time));
    }
    INT64 current_time;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
    io.DeltaTime = static_cast<float>(current_time - g_Time) / g_TicksPerSecond;
    g_Time = current_time;
    
    if (hwnd && GetForegroundWindow() == hwnd)
    {
        POINT mouse_screen_pos;
        GetCursorPos(&mouse_screen_pos);
        ScreenToClient(hwnd, &mouse_screen_pos);
        
        io.AddMousePosEvent(static_cast<float>(mouse_screen_pos.x), static_cast<float>(mouse_screen_pos.y));

        io.AddMouseButtonEvent(0, (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
        io.AddMouseButtonEvent(1, (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0);
        io.AddMouseButtonEvent(2, (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0);
    }
    else
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
}

void DrawImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("DEMASTER DEBUG");
    ImGui_DisplayDebugButtons(); // Critical: the user interacts with this to set bPaused = false
    ImGui::Separator();
    if (ImGui::BeginChild("Demaster::Textures" ,ImVec2(0,0), true))
    {
        ImGui_DisplayTexturesSection();
        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::Render();
}

void CreateImGuiImplementation()
{
    IMGUI_CHECKVERSION();
    imguiContext = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    if(!ImGui_ImplGlfw_InitForOpenGL(ffWindow, false))
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

void ImGui_DisplayDebugButtons()
{
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    if (ImGui::Button("Pause"))
    {
        bPaused = !bPaused;
    }
    ImGui::SameLine();
    if (ImGui::Button("Advance one frame"))
    {
        bPaused = false;
        bPauseAdvanceFrame = true;
    }
}

void ImGui_DisplayTexturesSection()
{
    if(ImGui::CollapsingHeader("Textures"))
    {
        ImGui::Text("Captured %zu textures", g_capturedTextures.size());
        for (const auto& tex : g_capturedTextures)
        {
            const auto& texInfo = tex.second;
            float aspect = static_cast<float>(texInfo.width) / static_cast<float>(texInfo.height);
            ImVec2 thumb_size = { 128.0f * aspect, 128.0f };

            ImGui::Text("ID: %u (%dx%d) [%s]", texInfo.id, texInfo.width, texInfo.height, InternalFormatToString(texInfo.internalFormat));

            ImGui::SameLine();
            ImGui::PushID(texInfo.id);
            if(ImGui::Button("Dump"))
            {
                DumpTextureToDisk(texInfo.id);
            }
            
            ImGui::SameLine();
            if(ImGui::Button("Upload"))
            {
                ReuploadTexture(texInfo.id);
            }
            ImGui::PopID();
            ImVec2 image_pos = ImGui::GetCursorScreenPos();
            
            ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texInfo.id)), thumb_size);
            
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRect(image_pos, ImVec2(image_pos.x + thumb_size.x, image_pos.y + thumb_size.y), 
                              IM_COL32(128, 128, 128, 255), 0.0f, 0, 1.0f);
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("ID: %u (%dx%d)", texInfo.id, texInfo.width, texInfo.height);
                
                ImVec2 preview_size = { 256.0f * aspect, 256.0f };
                ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texInfo.id)), preview_size);
                ImGui::EndTooltip();
            }
            ImGui::Separator();
        }
    }
}

GLuint GetCurrentBoundTextureID()
{
    GLint currentTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    return static_cast<GLuint>(currentTexture);
}

void DumpTextureToDisk(GLuint textureID)
{
    std::string directoryPath;
    CheckCreateImGuiTextureDumpDirectory(directoryPath);
    std::string textureFilePath = directoryPath + "texture_" + std::to_string(textureID) + ".png";

    if (g_capturedTextures.find(textureID) == g_capturedTextures.end())
    {
        OutputDebug("DumpTextureToDisk: Texture ID %u not found in captured list.", textureID);
        return;
    }

    const auto& texInfo = g_capturedTextures[textureID];
    const int width = texInfo.width;
    const int height = texInfo.height;

    GLint isCompressed = 0;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &isCompressed);

    bx::FileWriter writer;
    if (!bx::open(&writer, bx::FilePath(textureFilePath.c_str())))
    {
        OutputDebug("Failed to open file for writing: %s", textureFilePath.c_str());
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }

    bx::DefaultAllocator allocator;

    if (isCompressed)
    {
        GLint currentInternalFormat = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &currentInternalFormat);
        
        if (currentInternalFormat != texInfo.internalFormat)
        {
            OutputDebug("Texture %u format updated on GPU: %s -> %s", textureID,
                InternalFormatToString(texInfo.internalFormat),
                InternalFormatToString(currentInternalFormat));
        }

        GLint compressedSize = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize);

        std::vector<unsigned char> compressedData(compressedSize);
        glGetCompressedTexImage(GL_TEXTURE_2D, 0, compressedData.data());
        
        bimg::TextureFormat::Enum srcFormat = GLFormatToBimgFormat(currentInternalFormat);

        if (srcFormat != bimg::TextureFormat::Unknown)
        {
            std::vector<unsigned char> decompressedPixels(width * height * 4);

            bimg::imageDecodeToRgba8(
                &allocator,
                decompressedPixels.data(),
                compressedData.data(),
                width,
                height,
                width * 4,
                srcFormat
            );

            bimg::imageWritePng(&writer, width, height, width * 4, decompressedPixels.data(), bimg::TextureFormat::RGBA8, true);
        }
        else
        {
            OutputDebug("Failed to dump texture %u: Unsupported compressed format (%s).", textureID, InternalFormatToString(currentInternalFormat));
        }
    }
    else
    {
        // This path is for textures that are confirmed to be uncompressed
        OutputDebug("Dumping uncompressed texture ID %u...", textureID);
        const int channels = 4;
        std::vector<unsigned char> pixels(width * height * channels);

        GLint previous_alignment;
        glGetIntegerv(GL_PACK_ALIGNMENT, &previous_alignment);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        glPixelStorei(GL_PACK_ALIGNMENT, previous_alignment);

        bimg::imageWritePng(&writer, width, height, width * channels, pixels.data(), bimg::TextureFormat::RGBA8, false);
    }

    bx::close(&writer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ReuploadTexture(GLuint textureID)
{
    std::string directoryPath;
    CheckCreateImGuiTextureDumpDirectory(directoryPath);
    std::string textureFilePath = directoryPath + "texture_" + std::to_string(textureID) + ".png";
    if (!std::filesystem::exists(textureFilePath))
    {
        OutputDebug("ReuploadTexture: Texture ID %u not found in captured list (file doesn't exist).", textureID);
        return;
    }
    bx::DefaultAllocator allocator;
    bx::FileReader reader;
    
    if (bx::open(&reader, textureFilePath.c_str()))
    {
        uint32_t size = static_cast<uint32_t>(bx::getSize(&reader));
        void* buffer = bx::alloc(&allocator, size);
        bx::read(&reader, buffer, size);
        bx::close(&reader);
        
        bimg::ImageContainer* imageContainer = bimg::imageParse(&allocator, buffer, size, bimg::TextureFormat::RGBA8);

        if (nullptr != imageContainer)
        {
            const int width = imageContainer->m_width;
            const int height = imageContainer->m_height;
            const void* data = imageContainer->m_data;

            GLint previous_alignment;
            glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_alignment);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, flippedData.data());
            glBindTexture(GL_TEXTURE_2D, 0);

            glPixelStorei(GL_UNPACK_ALIGNMENT, previous_alignment);

            OutputDebug("Successfully re-uploaded texture %u from %s", textureID, textureFilePath.c_str());
            
            bimg::imageFree(imageContainer);
        }
        else
            OutputDebug("ReuploadTexture: Failed to parse image file for texture ID %u.", textureID);
        
        bx::free(&allocator, buffer);
    }
    else
        OutputDebug("ReuploadTexture: Failed to open file for texture ID %u.", textureID);
}

void CheckCreateImGuiTextureDumpDirectory(std::string& directoryPath)
{
    std::string textureFilePath = std::string(DIRECT_IO_EXPORT_DIR) + "IMGUI_DUMPS/";
    directoryPath = textureFilePath;
    if(!std::filesystem::exists(textureFilePath))
    {
        std::filesystem::create_directory(textureFilePath);
    }
}

/**
 * \brief This hook intercepts the wglSwapBuffers call for operations before the actual Windows API SwapBuffers(HDC)
 * \param hdc as in SwapBuffers(HDC)
 * \return as in SwapBuffers(HDC)
 */
BOOL __stdcall HookSwapBuffers(const HDC hdc)
{
    if (bPaused && IMGUI_DEBUG)
    {
        while (bPaused)
        {
            UpdateImGuiIO(hdc, static_cast<float>(ffWindowWidth), static_cast<float>(ffWindowHeight));

            PumpMessages();

            // render texture slot ID=1 drawing fullscreen quad to prevent jumpings
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            GLint last_program;
            glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
            glUseProgram(0);
            glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, ffWindowWidth, 0, ffWindowHeight, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 1);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(static_cast<float>(ffWindowWidth), 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(static_cast<float>(ffWindowWidth), static_cast<float>(ffWindowHeight));
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, static_cast<float>(ffWindowHeight));
            glEnd();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
            glPopAttrib();
            glUseProgram(last_program);

            DrawImGuiFrame();
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            swapBuffersTrampoline(hdc);
        }
        if (bPauseAdvanceFrame)
        {
            bPaused = true;
            bPauseAdvanceFrame = false;
            return swapBuffersTrampoline(hdc);
        }
        return TRUE; // Continue the original SwapBuffers call
    }
    else
    {
        if (IMGUI_DEBUG)
        {
            UpdateImGuiIO(hdc, static_cast<float>(ffWindowWidth), static_cast<float>(ffWindowHeight));
        
            DrawImGuiFrame();
        
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }
    
    return swapBuffersTrampoline(hdc); //static_cast<BOOL (__stdcall*)(HDC)>(swapBuffersTrampoline)(hdc);
}

LPVOID glewUseProgramTrampoline;
void HookGlUseProgram(const GLuint program)
{
    OutputDebug("\nglUseProgram: %d", program);
    return static_cast<void(__stdcall*)(GLuint)>(glewUseProgramTrampoline)(program);
}


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
    MH_CreateHook(&SwapBuffers, &HookSwapBuffers, reinterpret_cast<LPVOID*>(&swapBuffersTrampoline));

    //LPVOID pGameClockAddress = reinterpret_cast<LPVOID>(GetAddressBase(SUB_BATTLESWIRL_SLEEP));
    //MH_CreateHook(pGameClockAddress, &HookGameClockUpdate, reinterpret_cast<LPVOID*>(&pGameClockUpdateTrampoline));
    
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