#pragma once

#include <sstream>
#include <cstdio>

#include <INIReader.h>
#include "renderer.h"
#include <memory>
#include <Psapi.h>

#include "debug.h"
#include "mymemory.h"
#include "image.h"
#include "file.h"


#define EXPORT __declspec(dllexport)

#define DEBUG_BREAK_IF_DEBUGGER if(IsDebuggerPresent()) DebugBreak();

// ReSharper disable once CppInconsistentNaming
BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize = 5);
BOOL ModPage(DWORD address, int size = 5);
void ReplaceWithNOP(DWORD address, int instructionSize = 5);
void InjectDWORD(DWORD address, DWORD value);


/****************************************************************************/
//    Copyright (C) 2009 Aali132                                            //
//    Copyright (C) 2018 quantumpencil                                      //
//    Copyright (C) 2018 Maxime Bacoux                                      //
//    Copyright (C) 2020 myst6re                                            //
//    Copyright (C) 2020 Chris Rizzitello                                   //
//    Copyright (C) 2020 John Pritchard                                     //
//    Copyright (C) 2023 Julian Xhokaxhiu                                   //
/****************************************************************************/
//From FFNx : https://github.com/julianxhokaxhiu/FFNx/blob/master/src/patch.h

void ReplaceFunction(uint32_t offset, void *func);
uint32_t GetRelativeCall(uint32_t base, uint32_t offset);
uint32_t GetRelativeJmp(uint32_t base, uint32_t offset);
uint32_t GetAbsoluteValue(uint32_t base, uint32_t offset);
void ReplaceCall(uint32_t offset, void *func);
void ReplaceCallFunction(uint32_t offset, void* func);

inline DWORD IMAGE_BASE;
inline DWORD OPENGL_HANDLE;
inline const char* DIRECT_IO_EXPORT_DIR = "DEMASTER_EXP\\";
inline DWORD DIRECT_IO_EXPORT_DIR_LEN = strlen(DIRECT_IO_EXPORT_DIR);

struct ModuleInformation
{
    char * name;
    MODULEINFO info;
};

inline std::vector<ModuleInformation> moduleInformations;

extern bx::DefaultAllocator texAllocator;

#define UINT unsigned int

void ApplyDirectIO();
void ApplyUVPatch();

void ApplyBattleCharacterPatch();
void ApplyFieldEntityPatch();
void ApplyBattleHookPatch();
void ApplyBattleMonsterPatch();
void ApplyBattleFieldPatch();
void ApplyBattleMagicPatch();
void ApplyFieldBackgroundPatch();
void ApplyWorldPatch();
DWORD GetFieldBackgroundReplacementExist();

inline int lastWidth;
inline int lastHeight;
inline UCHAR* lastrgbBuffer;
inline UINT OPENGL_TEXTURES;

inline DWORD* tex_header;
inline DWORD* tex_struct;
inline DWORD* gl_textures;
inline DWORD pixelsPtr;
inline DWORD texturesPtr;

inline DWORD TEX_TYPE;

inline int currentMode;
inline std::string currentModeStr;

inline bool battleMagicTextureSignal = false;
inline int battleMagicTextureID = -1;

static enum Modes
{
    MODE_NONE,
    MODE_FIELD = 2,
    MODE_MENU,
    MODE_MENU2,
    MODE_WORLDMAP,
    MODE_CARDGAME,
    MODE_BATTLE,
    MODE_BATTLESWIRL,
    MODE_INTRO,
    MODE_CD_CHANGE,
    MODE_CHANGE_TO_MAIN,
    MODE_MAIN,
    MODE_TITLE,
    MODE_UNKNOWN
};

DWORD WtpCheck();
void WtpGl();
void FbgGl();

inline DWORD parm1; //arg+8

inline DWORD* langIdent_ESI;

inline int currentStage;
