// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "coreHeader.h"
#include <GL/GL.h>
#include "minhook/include/MinHook.h"
#include <chrono>
#include <filesystem>
#include "texture.h"
#include "config.h"
#include "opengl.h"


//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest() { OutputDebug("DEMASTER ENGINE LOADED!\n"); }

#ifndef JAPANESE_PATCH
#define JAPANESE_PATCH 0
#endif

const char* windowTitle = "FINAL FANTASY VIII Remastered - Demaster patch by Maki [1.3]";
#if JAPANESE_PATCH
const char* moduleName = "FFVIII_JP";
#else
const char* moduleName = "FFVIII_EFIGS";
#endif



DWORD dllmainBackAddr;
__declspec(naked) void AsmReplaceWindowTitle()
{
	__asm
	{
		PUSH windowTitle
		JMP dllmainBackAddr
	}
}


void GetWindow()
{
	dllmainBackAddr = reinterpret_cast<DWORD>(InjectJMP(IMAGE_BASE + GetAddress(WINDOWTITLE),
		reinterpret_cast<DWORD>(AsmReplaceWindowTitle), 5));
	const unsigned int wndGlfw = IMAGE_BASE + GetAddress(WINDOWTITLE) + 0x12
	+ *reinterpret_cast<DWORD*>(IMAGE_BASE + GetAddress(WINDOWTITLE) + 0x12) + 4;
	MH_CreateHook(reinterpret_cast<LPVOID>(wndGlfw), HookGlfwWindow, &glfwWindowTrampoline);
	const unsigned int wndGlfwInit = IMAGE_BASE + GetAddress(WINDOWTITLE) - 0xA2
	 + *reinterpret_cast<DWORD*>(IMAGE_BASE + GetAddress(WINDOWTITLE) - 0xA2) + 4;
	MH_CreateHook(reinterpret_cast<LPVOID>(wndGlfwInit), HookGlfwInit, &glfwInitTrampoline);

	const unsigned int wndGlfwSetInputMode = IMAGE_BASE + GetAddress(WINDOWTITLE) + 0x162; //PUSH GLFW_HIDDEN_CURSOR
	InjectDWORD(wndGlfwSetInputMode+1, GLFW_CURSOR_NORMAL);
}




//force GL_LINEAR with non-fixed UV was a d*k move ... ~Maki
void ApplyFilteringPatch()
{
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH1) + 1, GL_NEAREST);
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH2) + 1, GL_NEAREST);
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH3) + 1, GL_NEAREST);
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH4) + 1, GL_NEAREST);
}

const char * GetModeName(const int mode)
{
	const char *result; // eax
	currentMode = mode;
	switch ( mode )
	{
		case 0:
			result = "MODE_NONE";
			break;
		case 2:
			result = "MODE_FIELD";
			break;
		case 3:
			result = "MODE_MENU";
			break;
		case 4:
			result = "MODE_MENU2";
			break;
		case 5:
			result = "MODE_WORLDMAP";
			break;
		case 6:
			result = "MODE_CARDGAME";
			break;
		case 7:
			result = "MODE_BATTLE";
			break;
		case 8:
			result = "MODE_BATTLESWIRL";
			break;
		case 9:
			result = "MODE_INTRO";
			break;
		case 10:
			result = "MODE_CD_CHANGE";
			break;
		case 11:
			result = "MODE_CHANGE_TO_MAIN";
			break;
		case 12:
			result = "MODE_MAIN";
			break;
		case 13:
			result = "MODE_TITLE";
			break;
		default:
			result = "MODE_UNKNOWN";
			break;
	}
	currentModeStr = result;
	OutputDebug("Mode changed to %s\n", result);
	return result;
}

BOOL WINAPI DllMain(

	HINSTANCE hinstDLL, // handle to DLL module
	DWORD fdwReason, // reason for calling function
	LPVOID lpReserved) // reserved
{
	if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
		return 0;

	SetUnhandledExceptionFilter(ExceptionHandler);

	
	

	AllocConsole();
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CON", "r", stdin);
	InitTest();
	ReadConfigFile();
	if (LOG) logFile = decltype(logFile){ fopen("demasterlog.txt", "wb"), fclose };
	
#if USE_SERVER
		serverInst.WriteBuffer("LOL", 3);
		serverInst.WriteBuffer("ABCDEF", 6);
#endif
	
	IMAGE_BASE = reinterpret_cast<DWORD>(GetModuleHandleA(moduleName));
	OPENGL_HANDLE = reinterpret_cast<DWORD>(GetModuleHandleA("OPENGL32"));

	OutputDebug("IMAGE_BASE at: %lX; OPENGL at: %lX\n", IMAGE_BASE, OPENGL_HANDLE);

	MH_Initialize();
	GetWindow();
	MH_CreateHookApi(L"OPENGL32", "glViewport", HookGlViewport, &oglViewport);

	//uint32_t getModeName = get_relative_call(GetAddress(CHANGEMODE), 0x1FC);
	ReplaceCallFunction(GetAddressBase(CHANGEMODE)+0x1FC, GetModeName);

	//LET'S GET THE HACKING DONE
	if (DIRECT_IO)
		ApplyDirectIO();
	if (UVPATCH)
		ApplyUVPatch();
	if (TEXTURE_PATCH && DIRECT_IO)
		ReplaceTextureFunction();
	if (DEBUG_PATCH)
		InjectJMP(IMAGE_BASE + GetAddress(NULLSUB_DEBUG), reinterpret_cast<DWORD>(OutputDebug));
	if (LINEAR_PATCH)
		ApplyFilteringPatch();

	
	MH_EnableHook(MH_ALL_HOOKS);
	return 1; //all success
}
#undef CRASHLOG