#pragma once
#include <stdio.h>
#include <Windows.h>
#ifndef COMMONDEF
#define COMMONDEF
//CONFIG
extern BOOL UVPATCH, DIRECT_IO, TEXTURE_PATCH, DEBUG_PATCH, LOG;
extern BOOL BATTLE_CHARA, FIELD_ENTITY, BATTLE_HOOK, FIELD_BACKGROUND, WORLD_TEXTURES;

BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize);
BOOL modPage(DWORD address, int size);
void ReplaceCALLWithNOP(DWORD address);
void InjectDWORD(DWORD address, DWORD value);
DWORD bspVoid(UINT textures, int a2, char* pixels);

extern long long IMAGE_BASE;
extern long long OPENGL_HANDLE;
extern DWORD attr;
extern const char* DIRECT_IO_EXPORT_DIR;
extern DWORD DIRECT_IO_EXPORT_DIR_LEN;

void ApplyDirectIO();
void ApplyUVPatch();
void ReplaceTextureFunction();

void ApplyBattleCharacterPatch();
void ApplyFieldEntityPatch();
void ApplyBattleHookPatch();
void ApplyBattleMonsterPatch();
void ApplyBattleFieldPatch();
void ApplyFieldBackgroundPatch();
void ApplyWorldPatch();
void HookOpenGL();

extern char texPath[256];
extern int lastWidth;
extern int lastHeight;
extern UCHAR* lastrgbBuffer;
extern UINT OPENGL_TEXTURES;

extern DWORD* tex_header;
extern DWORD* tex_struct;
extern DWORD* gl_textures;
extern DWORD pixelsPtr;
extern DWORD texturesPtr;

extern DWORD TEX_TYPE;



DWORD _wtpCheck();
void _wtpGl();

extern DWORD parm1; //arg+8

extern DWORD* langIdent_ESI;

extern int currentStage;

extern FILE* logFile;
void OutputDebug(const char* c);


void _wtp03();

#define EXPORT __declspec(dllexport)
#pragma warning(disable:4996)
#endif