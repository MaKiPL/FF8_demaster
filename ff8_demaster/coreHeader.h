#pragma once
#include <stdio.h>
#include <Windows.h>

//CONFIG
BOOL UVPATCH, DIRECT_IO, TEXTURE_PATCH, DEBUG_PATCH, LOG;
BOOL BATTLE_CHARA, FIELD_ENTITY, BATTLE_HOOK, FIELD_BACKGROUND, WORLD_TEXTURES;

BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize);
BOOL modPage(DWORD address, int size);
void ReplaceCALLWithNOP(DWORD address);
void InjectDWORD(DWORD address, DWORD value);
DWORD bspVoid(UINT textures, int a2, char* pixels);

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

DWORD _wtpCheck();
void _wtpGl();

DWORD parm1; //arg+8

int currentStage;

FILE* logFile;
void OutputDebug(char* c);

#define EXPORT __declspec(dllexport)
#pragma warning(disable:4996)