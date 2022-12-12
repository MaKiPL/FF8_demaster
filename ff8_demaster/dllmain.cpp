// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "coreHeader.h"
#include <GL/GL.h>
#include "minhook/include/MinHook.h"
#include <chrono>
#include <filesystem>
#include "texture.h"
#include "config.h"
#include "opengl.h"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "plog/Helpers/HexDump.h"


//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest() {  PLOG_INFO << "DEMASTER ENGINE LOADED!"; }

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
}




//force GL_LINEAR with non-fixed UV was a d*k move ... ~Maki
void ApplyFilteringPatch()
{
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH1) + 1, GL_NEAREST);
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH2) + 1, GL_NEAREST);
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH3) + 1, GL_NEAREST);
	InjectDWORD(IMAGE_BASE + GetAddress(FILTERPATCH4) + 1, GL_NEAREST);
}

BOOL WINAPI DllMain(

	HINSTANCE hinstDLL, // handle to DLL module
	DWORD fdwReason, // reason for calling function
	LPVOID lpReserved) // reserved
{
	if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
		return 0;
	

	AllocConsole();
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CON", "r", stdin);
	static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	ReadConfigFile();
	if (FILE_LOG)
		plog::init(static_cast<plog::Severity>(LOG_SEVERITY), LOG_FILENAME.c_str())
	.addAppender(&consoleAppender);
		/*logFile = decltype(logFile){ fopen("demasterlog.txt", "wb"), fclose };*/
	InitTest();



	PLOG_INFO << "Demaster patch by Maki v1.3";
	
	IMAGE_BASE = reinterpret_cast<DWORD>(GetModuleHandleA(moduleName));
	OPENGL_HANDLE = reinterpret_cast<DWORD>(GetModuleHandleA("OPENGL32"));
	
	PLOG_INFO << "IMAGE_BASE at: " << std::hex << std::uppercase << IMAGE_BASE << "; OPENGL at: " << OPENGL_HANDLE
	<< std::dec;
	

	InitTable(IMAGE_BASE);

	MH_Initialize();
	GetWindow();
	MH_CreateHookApi(L"OPENGL32", "glViewport", HookGlViewport, &oglViewport);

	//LET'S GET THE HACKING DONE
	if (DIRECT_IO)
		ApplyDirectIO();
	if (UVPATCH)
		ApplyUVPatch();
	if (TEXTURE_PATCH && DIRECT_IO)
		ReplaceTextureFunction();
	if (DEBUG_PATCH)
		InjectJMP(IMAGE_BASE + GetAddress(NULLSUB_DEBUG)
			, reinterpret_cast<DWORD>(OutputDebugPatch));
	if (LINEAR_PATCH)
		ApplyFilteringPatch();

	
	MH_EnableHook(MH_ALL_HOOKS);
	return 1; //all success
}
#undef CRASHLOG