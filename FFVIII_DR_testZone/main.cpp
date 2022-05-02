#include <iostream>
#include "../FFVIII_DR/core.h"


int main()
{
	DR_CREATE_LOGGER
	spdlog::info("This is a sample text");
	spdlog::warn("Whoops, be careful!");
	
	return 0;
}