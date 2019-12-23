#include <stdio.h>
#include <Windows.h>
#include "coreHeader.h"

/*
KURSE ALL SEEDS!
*/


/*
*
*
*/

//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest()
{
	OutputDebugStringA("DEMASTER ENGINE LOADED!- we will try to repair the lazy remaster... ;-;\n");
	DIRECT_IO_EXPORT_DIR = "EXP\\";
	return;
}

char DEB_buf[256];
DWORD lastJMP;

void DEB_JMP(char* c, DWORD a, DWORD b, DWORD cc, DWORD d, DWORD e)
{
	if (c == 0x3)
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
	OutputDebugStringA(DEB_buf);
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
	DEB_backAdd = InjectJMP(IMAGE_BASE + 0x33DC0, (DWORD)DEB_JMP, 5);
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

BOOL WINAPI DllMain(

	HINSTANCE hinstDLL, // handle to DLL module
	DWORD fdwReason, // reason for calling function
	LPVOID lpReserved) // reserved
{
	if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
		return 0;

	IMAGE_BASE = GetModuleHandleA("FFVIII_EFIGS");
	OPENGL_HANDLE = GetModuleHandleA("OPENGL32");
	
	InitTest();


	//LET'S GET THE HACKING DONE
	ApplyUVPatch();
	ApplyDirectIO();
	//ReplaceTextureFunction_deprecated();
	ReplaceTextureFunction();

	//ApplyDebugOutputPatch(); //they have hella of debug info shit, but then function is nullsub-
							//quite usual- vanilla ff8 from 2000 had the same for harata battle debug
							//but worry not- we can write such function from scratch
													//ApplyTextureUpscaleMod();
	return 1; //all success
}

