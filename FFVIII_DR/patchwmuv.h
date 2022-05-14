#pragma once
#include "core.h"

unsigned char uvBinary[] = { 0x00 };
std::string uvInstructions[] = { "No instructions" };
Patch PatchUV = Patch(uvBinary, 
	std::vector<std::string>(uvInstructions, uvInstructions + 1),
	)