#include "dllmain.h"
#include <windows.h>
#include <thread>

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

	DR_CREATE_LOGGER
	spdlog::info("Demaster Reborn initializing...");

	CONFIG = std::make_unique<Config>();

	FF8DLL = CONFIG->GetStringValue("LANG") == "JP"? JPDLL : EFIGSDLL;
	IMAGEBASE = reinterpret_cast<DWORD>(GetModuleHandleA(FF8DLL.c_str()));
	spdlog::info("[MAIN] FF8 DLL: {} ({:#x})", FF8DLL,IMAGEBASE);

	return true;
}