#pragma once
#include <stdio.h>
#include <Windows.h>

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

#define EXPORT __declspec(dllexport)
#pragma warning(disable:4996)