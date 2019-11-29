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
	//ApplyUVPatch();
	//ApplyDirectIO();
	//ReplaceTextureFunction();


													//ApplyTextureUpscaleMod();
	return 1; //all success
}

