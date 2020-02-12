#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <stdio.h>
#include <Windows.h>
#include "coreHeader.h"
#include "ini.h"

/*
KURSE ALL SEEDS!
*/


/*
*
*
*/
long long IMAGE_BASE = 0;
long long OPENGL_HANDLE = 0;
const char * DIRECT_IO_EXPORT_DIR = "DEMASTER_EXP\\";
FILE* logFile = NULL;
DWORD* tex_header = 0;
DWORD attr = -1;
DWORD DIRECT_IO_EXPORT_DIR_LEN = -1;
DWORD* tex_struct = 0;
DWORD* gl_textures= 0;
DWORD pixelsPtr = 0;
DWORD texturesPtr = 0;
DWORD TEX_TYPE = 0;
int currentStage = -1;
DWORD* langIdent_ESI;
BOOL UVPATCH, DIRECT_IO, TEXTURE_PATCH, DEBUG_PATCH, LOG;
BOOL BATTLE_CHARA, FIELD_ENTITY, BATTLE_HOOK, FIELD_BACKGROUND, WORLD_TEXTURES;
BOOL LINEAR_PATCH, OPENGL_HOOK;


void OutputDebug(const char* c)
{
	printf(c);
	if (LOG)
	{
		logFile = fopen("demasterlog.txt", "a+");
		if (logFile != NULL)
		{
			fwrite(c, sizeof(char), strlen(c), logFile);
			fclose(logFile);
		}
	}
	
}

DWORD _dllmainBackAddr1;
DWORD _dllmainBackAddr2;

const char* windowTitle = "FINAL FANTASY VIII Remastered - Demaster patch by Maki [1.2.6] BETA";

__declspec(naked) void _dllMain2()
{
	__asm
	{
		PUSH windowTitle
		JMP _dllmainBackAddr2
	}
}


void GetWindow()
{
	_dllmainBackAddr2 = (DWORD)InjectJMP(IMAGE_BASE + 0x1601065, (DWORD)_dllMain2, 5);
}

//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest()
{
	OutputDebug("DEMASTER ENGINE LOADED!\n");
	return;
}

char DEB_buf[256];
DWORD lastJMP;

void DEB_JMP(char* c, DWORD a, DWORD b, DWORD cc, DWORD d, DWORD e)
{
	if (c < (char*)IMAGE_BASE)
		return;
	char localD[32];
	localD[0] = '\0';
	sprintf(localD, "Wrong address at: %08X\n", c);
	if (IsBadReadPtr(c, 4)) 
	{
		//OutputDebugStringA(localD);
		__asm
		{
			//INT 3
		}
		return;
	}
	if (*c == 183)
	{
		__asm
		{
			INT 3
		}
	}
	sprintf(DEB_buf, c, a, b, cc, d, e);
	OutputDebug(DEB_buf);
	return;
}

__declspec(naked) void nullsub()
{
	__asm
	{
		ret
	}
}

DWORD DEB_backAdd;

void ApplyDebugOutputPatch()
{
	DEB_backAdd = (DWORD)InjectJMP(IMAGE_BASE + 0x33DC0, (DWORD)DEB_JMP, 5);
	//critical sections are null here, but they point to common_fieldEC, that's not cool
	*(DWORD*)(IMAGE_BASE + 0x16EDF68) = (DWORD)nullsub;
	*(DWORD*)(IMAGE_BASE + 0x16EDF7C) = (DWORD)nullsub;
	*(DWORD*)(IMAGE_BASE + 0x16EDA7C) = (DWORD)nullsub;
	*(DWORD*)(IMAGE_BASE + 0x16EDD74) = (DWORD)nullsub;
	modPage(IMAGE_BASE + 0x1696058, 4);
	*(DWORD*)(IMAGE_BASE + 0x1696058) = (DWORD)nullsub;
	modPage(IMAGE_BASE + 0x169605C, 4);
	*(DWORD*)(IMAGE_BASE + 0x169605C) = (DWORD)nullsub;
	ReplaceCALLWithNOP(0x159F3BD);
	ReplaceCALLWithNOP(0x4485FF);
	ReplaceCALLWithNOP(0x15824B6);
}

DWORD _deb00_ECX;
DWORD _deb00_EAX;

void DEB_JMPv2_00()
{
	const char* format = "FSArchive:: %s - %s\n";
	char localn[256];
	sprintf(localn, format, _deb00_EAX, _deb00_ECX);
	OutputDebug(localn);
}

__declspec(naked) void DEB_JMPv2()
{
	__asm
	{
		MOV _deb00_ECX, ECX
		MOV _deb00_EAX, EAX
		CALL DEB_JMPv2_00
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x1692994
		PUSH EAX
		JMP DEB_backAdd
	}
}

void ApplyDebugOutputPatchV2()
{
	DEB_backAdd = (DWORD)InjectJMP(IMAGE_BASE + 0x1581270, (DWORD)DEB_JMPv2, 5);

}

//force GL_LINEAR with non-fixed UV was a d*k move ...
void ApplyFilteringPatch()
{
	//InjectDWORD(IMAGE_BASE + 0x1564D97 + 1, 0x2600); //main indirect
	//InjectDWORD(IMAGE_BASE + 0x1564D86 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x1564F23 + 1, 0x2600); //main indirect
	//InjectDWORD(IMAGE_BASE + 0x1564F34 + 1, 0x2600);


	//InjectDWORD(IMAGE_BASE + 0x156505E + 1, 0x2600); //unknown glDrawElements
	//InjectDWORD(IMAGE_BASE + 0x1565073 + 1, 0x2600);
	//
	//InjectDWORD(IMAGE_BASE + 0x1566ED5 + 1, 0x2600); //as above fxaa2
	//InjectDWORD(IMAGE_BASE + 0x1566EE6 + 1, 0x2600);



	//InjectDWORD(IMAGE_BASE + 0x15686D3 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x15686E4 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x1568805 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x1568816 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x15689C9 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x15689DA + 1, 0x2600);

	InjectDWORD(IMAGE_BASE + 0x15693EF + 1, 0x2600);
	InjectDWORD(IMAGE_BASE + 0x1569409 + 1, 0x2600);
	InjectDWORD(IMAGE_BASE + 0x156A348 + 1, 0x2600);
	InjectDWORD(IMAGE_BASE + 0x156A359 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x156D47F + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x156D490 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x156D830 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x156D841 + 1, 0x2600);
}



void ReadConfigFile()
{
	DWORD attr = GetFileAttributesA("demaster.conf");
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("File demaster.ini not found- all failed\n");
		return;
	}
	OutputDebug("Reading config file demaster.ini\n");
	//ini_t* conf = ini_load("demaster.conf");

	INIReader conf("demaster.conf");

	UVPATCH = conf.GetInteger("", "UV_PATCH", 0);
	DIRECT_IO = conf.GetInteger("", "DIRECT_IO", 0);
	LOG = conf.GetInteger("", "LOG", 0);
	DEBUG_PATCH = conf.GetInteger("", "MORE_DEBUG_OUTPUT_PATCH", 0);
	BATTLE_CHARA = conf.GetInteger("", "BATTLE_CHARACTER", 0);
	FIELD_ENTITY = conf.GetInteger("", "FIELD_ENTITY", 0);
	BATTLE_HOOK = conf.GetInteger("", "BATTLE_HOOK_MONSTER_FIELDS", 0);
	FIELD_BACKGROUND = conf.GetInteger("", "FIELD_BACKGROUND", 0);
	WORLD_TEXTURES = conf.GetInteger("", "WORLD_TEXTURES", 0);
	TEXTURE_PATCH = conf.GetInteger("", "TEXTURE_PATCH", 1); //this one lacks actual demaster.conf so default to 1
	LINEAR_PATCH = conf.GetInteger("", "LINEAR_PATCH", 1);
	OPENGL_HOOK = conf.GetInteger("", "OPENGL_HOOK", 1);
}

BOOL WINAPI DllMain(

	HINSTANCE hinstDLL, // handle to DLL module
	DWORD fdwReason, // reason for calling function
	LPVOID lpReserved) // reserved
{
	if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
		return 0;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CON", "r", stdin);
	InitTest();
	ReadConfigFile();

	HMODULE IMG_BASE = GetModuleHandleA("FFVIII_EFIGS");
	IMAGE_BASE = (long long)IMG_BASE;
	OPENGL_HANDLE = (long long)GetModuleHandleA("OPENGL32");
	char localn[256];
	sprintf(localn, "IMAGE_BASE at: %lX; OPENGL at: %lX\n", IMAGE_BASE, OPENGL_HANDLE);
	OutputDebug(localn);

	GetWindow();

	//LET'S GET THE HACKING DONE
	if(DIRECT_IO)
		ApplyDirectIO();
	if(UVPATCH)
		ApplyUVPatch();
	if(TEXTURE_PATCH && DIRECT_IO)
		ReplaceTextureFunction();
	if (DEBUG_PATCH)
		ApplyDebugOutputPatchV2();
	if (LINEAR_PATCH)
		ApplyFilteringPatch();
	if (OPENGL_HOOK)
		HookOpenGL();

	//if(DEBUG_PATCH)
	//	ApplyDebugOutputPatch(); //they have hella of debug info shit, but then function is nullsub-
							//quite usual- vanilla ff8 from 2000 had the same for harata battle debug
							//but worry not- we can write such function from scratch
													//ApplyTextureUpscaleMod();
	return 1; //all success
}

