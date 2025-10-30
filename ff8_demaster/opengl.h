#pragma once
#include <GL/glew.h>
#include "GLFW/include/GLFW/glfw3.h"
#include "imgui/imgui.h"

void CheckGlew();
inline bool glewInitialized = false;


typedef GLFWwindow* (*OGlfwCreateWindow)(int width, int height, const char* title, GLFWwindow* monitor, GLFWwindow* share);
inline GLFWwindow* ffWindow;
inline GLFWwindow* ffWindowSecond;

int HookGlfwInit();
inline void* glfwInitTrampoline;

typedef BOOL (__stdcall* tSwapBuffers)(HDC hdc);
inline tSwapBuffers swapBuffersTrampoline = nullptr;

inline void* glfwWindowTrampoline;
GLFWwindow* HookGlfwWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

//glViewport - called very early after OpenGL context creation as one of the first OpenGL calls
//it works as an anchor for backdoor initialization of OpenGL hacks that required original OpenGL context
void* __stdcall HookGlViewport(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void __stdcall GlViewport(GLint x, GLint y, GLsizei width, GLsizei height);
inline void* oglViewport;

struct frameStructure
{
    char * pixels;
    int width;
    int height;
    int channels;
};

inline frameStructure frame;


//====Other OpenGL functions
typedef void(__stdcall* OglClear)(GLbitfield);
static OglClear oglClear;

typedef void(__stdcall* OglClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
static OglClearColor oglClearColor;

typedef void(__stdcall* OglBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
static OglBufferData oglBufferData;

// typedef int(__fastcall* tGameClockUpdate)(int a1, int a2);
// static tGameClockUpdate pGameClockUpdateTrampoline = nullptr;
//
// int __fastcall HookGameClockUpdate(int a1, int a2);

void PumpMessages();

void UpdateImGuiIO(HDC hdc, float ffWindowWidth, float ffWindowHeight);
void DrawImGuiFrame();
inline bool bPaused = false;
inline bool bPauseAdvanceFrame = false;

void CreateImGuiImplementation();

// IMGUI debug functions
void ImGui_DisplayDebugButtons();
void ImGui_DisplayTexturesSection();
// End of IMGUI debug functions

GLuint GetCurrentBoundTextureID();

void DumpTextureToDisk(GLuint textureID);
void ReuploadTexture(GLuint textureID);

void CheckCreateImGuiTextureDumpDirectory(std::string& directoryPath);

inline const char* InternalFormatToString(GLint format)
{
    switch (format)
    {
    case GL_RGB: return "GL_RGB";
    case GL_RGBA: return "GL_RGBA";
    case GL_RED: return "GL_RED";
    case GL_RG: return "GL_RG";
    
    case GL_R8: return "GL_R8";
    case GL_RG8: return "GL_RG8";
    case GL_RGB8: return "GL_RGB8";
    case GL_RGBA8: return "GL_RGBA8";
    
    case GL_BGRA: return "GL_BGRA";
    
    case GL_SRGB: return "GL_SRGB";
    case GL_SRGB8: return "GL_SRGB8";
    case GL_SRGB_ALPHA: return "GL_SRGB_ALPHA";
    case GL_SRGB8_ALPHA8: return "GL_SRGB8_ALPHA8";
    
    case GL_RGBA32F: return "GL_RGBA32F";
    case GL_RGB32F: return "GL_RGB32F";
    case GL_RGBA16F: return "GL_RGBA16F";
    case GL_RGB16F: return "GL_RGB16F";
    
    case GL_DEPTH_COMPONENT: return "GL_DEPTH_COMPONENT";
    case GL_DEPTH_COMPONENT16: return "GL_DEPTH_COMPONENT16";
    case GL_DEPTH_COMPONENT24: return "GL_DEPTH_COMPONENT24";
    case GL_DEPTH_COMPONENT32F: return "GL_DEPTH_COMPONENT32F";
    case GL_DEPTH_STENCIL: return "GL_DEPTH_STENCIL";
    
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT: return "DXT1_RGB";
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: return "DXT1_RGBA";
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: return "DXT3_RGBA";
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: return "DXT5_RGBA";

    default: return "Unknown Format";
    }
}

inline bimg::TextureFormat::Enum GLFormatToBimgFormat(GLint internalFormat)
{
    switch (internalFormat)
    {
        // DXT/BC formats
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:        return bimg::TextureFormat::BC1;
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:       return bimg::TextureFormat::BC1;
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:       return bimg::TextureFormat::BC2;
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:       return bimg::TextureFormat::BC3;
    case GL_COMPRESSED_SRGB_S3TC_DXT1_EXT:       return bimg::TextureFormat::BC1;
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: return bimg::TextureFormat::BC1;
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT: return bimg::TextureFormat::BC2;
    case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: return bimg::TextureFormat::BC3;
    case GL_RGBA8:  return bimg::TextureFormat::BC1;
        
        // Default/fallback
    default:                                    return bimg::TextureFormat::BC1;
    }
}