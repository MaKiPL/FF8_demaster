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


void CreateImGuiImplementation();

GLuint GetCurrentBoundTextureID();