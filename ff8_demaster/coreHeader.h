#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <StackWalker.h>
#include <INIReader.h>
#include "renderer.h"

#define EXPORT __declspec(dllexport)

#define DEMASTER_CONF "demaster.conf"

//CONFIG
extern BOOL UVPATCH, DIRECT_IO, TEXTURE_PATCH, DEBUG_PATCH, LOG;
extern BOOL BATTLE_CHARA, FIELD_ENTITY, BATTLE_HOOK, FIELD_BACKGROUND, WORLD_TEXTURES;

BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize = 5);
BOOL modPage(DWORD address, int size);
void ReplaceCALLWithNOP(DWORD address);
void InjectDWORD(DWORD address, DWORD value);

extern DWORD IMAGE_BASE;
extern DWORD OPENGL_HANDLE;
extern const char* DIRECT_IO_EXPORT_DIR;
extern DWORD DIRECT_IO_EXPORT_DIR_LEN;

extern bx::DefaultAllocator texAllocator;
bimg::ImageContainer* LoadImageFromFile(char* filename);

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
void OutputDebug(const char* fmt, ...);

void RenderTexture(bimg::ImageContainer* img);
void RenderUncompressedTexture(bimg::ImageContainer* img, TextureFormatInfo& texInfo);
void RenderCompressedTexture(bimg::ImageContainer* img, TextureFormatInfo& texInfo);

//CLASS
class DemasteredStackWalker : public StackWalker
{
public:
	DemasteredStackWalker() : StackWalker() {}
protected:
	virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr)
	{
		// Silence is golden.
	}

	virtual void OnOutput(LPCSTR szText)
	{
		OutputDebug(szText);
	}
};
