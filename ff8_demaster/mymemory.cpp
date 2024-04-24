#include "mymemory.h"
#include "coreHeader.h"

//this is a good place for managing game version and language indexes
unsigned int _stdcall GetAddress(const ADDRESS_NAME name)
{
	//unsigned int value = PATCHADDRESS[name][FORCEGAME];
	if (PATCHADDRESS.empty())
		return 0;
	return PATCHADDRESS.at(name)[FORCEGAME];// PATCHADDRESS[name][FORCEGAME];
}

unsigned int _stdcall GetAddressBase(const ADDRESS_NAME name)
{
	//unsigned int value = PATCHADDRESS[name][FORCEGAME];
	if (PATCHADDRESS.size() == 0)
	{
		return 0;
	}
	return IMAGE_BASE + PATCHADDRESS.at(name)[FORCEGAME];// PATCHADDRESS[name][FORCEGAME];
}