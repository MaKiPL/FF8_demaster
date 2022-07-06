#include "main.h"
#include <windows.h>
#include <thread>

int main(int argc, char *argv[])
{
	DR_CREATE_LOGGER
	spdlog::info("Initializing engine...");

	config = std::make_unique<Config>();
	language = std::make_unique<Language>();

	std::string dllLoad =
		config->GetStringValue("language") == language->defaultLanguage ?
		EFIGSDLL : JPDLL;

	HMODULE dllModule = LoadLibraryA(dllLoad.c_str());
	std::thread child = std::thread([dllModule, dllLoad]() {
		if (dllModule == NULL)
		{
			spdlog::error("Failed to load {}", dllLoad);
			return;
		}
		typedef void(*InitFunc)();
		InitFunc init = (InitFunc)GetProcAddress(dllModule,
			"runGame");
		if (init == NULL)
		{
			spdlog::error("Failed to get proc address for {}", JPDLL);
			return;
		}
		init();
	});

	child.join();
	return 0;
}