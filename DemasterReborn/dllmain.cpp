#include "dllmain.h"
#include <windows.h>
#include <thread>

#include "memory.h"
#include "patchio.h"
#include "zzz.h"

int main(int argc, char *argv[])
{
	DR_CREATE_LOGGER
	spdlog::info("Initializing engine...");

	CONFIG = std::make_unique<Config>();

	// std::string dllLoad =
	// 	CONFIG->GetStringValue("language") == language->defaultLanguage ?
	// 	EFIGSDLL : JPDLL;

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason != DLL_PROCESS_ATTACH)
		return false;

	// Create log
	DR_CREATE_LOGGER
	spdlog::info("Demaster Reborn initializing...");

	// Config.cfg file handling
	CONFIG = std::make_unique<Config>();

	// Get image base of selected language DLL and set language pointer
	FF8DLL = CONFIG->GetStringValue("LANG") == "JP"? JPDLL : EFIGSDLL;
	LANGUAGE_POINTER = FF8DLL == EFIGSDLL? 0 : 1;
	IMAGEBASE = reinterpret_cast<DWORD>(GetModuleHandleA(FF8DLL.c_str()));
	spdlog::info("[MAIN] FF8 DLL: {} ({:#x})", FF8DLL,IMAGEBASE);

	// Unpack files if needed. See if "ff8_en.exe.dat" exist. If it doesn't, then check the files
	if (CONFIG->GetIntValue("ALWAYS_CHECK_ZZZ_UNPACK") == 1 || !std::filesystem::exists(EXPORTPATH+"ff8_en.exe.dat"))
	{
		const bool forceUnpack = CONFIG->GetIntValue("FORCE_ZZZ_UNPACK_EVERY_CHECK") == 1;
		zzz zzzMain("main.zzz", forceUnpack);
		zzz zzzOther("other.zzz", forceUnpack);
	}

	if (CONFIG->GetIntValue("PATCH_IO"))
		PatchIO();

	return true;
}