#pragma once
#include <stdio.h>
#include <Windows.h>

BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize);
BOOL modPage(DWORD address, int size);
void ReplaceCALLWithNOP(DWORD address);

__int64 IMAGE_BASE;
DWORD OPENGL_HANDLE;
DWORD attr;
char* DIRECT_IO_EXPORT_DIR;
DWORD DIRECT_IO_EXPORT_DIR_LEN;

char texPath[256];
int lastWidth;
int lastHeight;
UCHAR* lastrgbBuffer;
UINT OPENGL_TEXTURES;

DWORD* tex_header;
DWORD* tex_struct;
DWORD* gl_textures;

DWORD parm1; //arg+8

#define EXPORT __declspec(dllexport)
#pragma warning(disable:4996)