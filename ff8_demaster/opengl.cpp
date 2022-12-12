// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr


#include "opengl.h"
#ifdef APIENTRY
#undef APIENTRY
#endif
#include "config.h"
#include "debug.h"
#include "texture.h"
#include "minhook/include/MinHook.h"

void CheckGlew()
{
    if (glewInitialized) return;

    glewInitialized = true;
    if (const GLenum err = glewInit(); GLEW_OK != err)
        /* Problem: glewInit failed, something is seriously wrong. */
        PLOG_FATAL << "GLEW Error: " << glewGetErrorString(err);
}

GLFWwindow* HookGlfwWindow(const int width, const int height, const char* title, GLFWmonitor* monitor,
    GLFWwindow* share)
{
    HookGlFunctionsPostGLFW();
    ffWindow = static_cast<GLFWwindow*(*)(int, int, const char*, GLFWmonitor*, GLFWwindow*)>(glfwWindowTrampoline)
    (width, height, title, monitor, share);
    printf("OpenGL Version: %p\n", glGetString(GL_VERSION));
    return ffWindow;
}

void HookGlFunctionsPostGLFW() {  }


void* __stdcall HookGlViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
{
    CheckGlew();
    if (static bool backdoorUsed = false; glewInitialized && !backdoorUsed)
    {
        PLOG_INFO << "OpenGL Version: " << glGetString(GL_VERSION);
        //Maki: PUT BACKDOORS FOR EXTENSIONS HERE
        if (TEXTURE_PATCH && DIRECT_IO)
        {
            MH_CreateHookApi(L"OPENGL32", "glBindTexture", HookGlBindTexture, &ogl_bind_texture);
            MH_CreateHookApi(L"OPENGL32", "glTexParameteri", HookGlTextParameteri, &ogl_tex_parametri);
            MH_CreateHookApi(L"OPENGL32", "glTexImage2D", HookGlTexImage2D, &ogl_tex_image2d);
            MH_EnableHook(MH_ALL_HOOKS);
        }
        backdoorUsed = true;
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