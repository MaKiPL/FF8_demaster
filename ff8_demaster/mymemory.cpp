#include "mymemory.h"

//uhm
void InitTable()
{
	//PATCHADDRESS[UVPATCH0] = std::vector<unsigned int>{ 0x00, 0x01, 0x02 };
	return;
}

//this is a good place for managing game version and language indexes
unsigned int _stdcall GetAddress(const ADDRESS_NAME name)
{
	//unsigned int value = PATCHADDRESS[name][FORCEGAME];
	if (PATCHADDRESS.size() == 0)
	{
		return 0;
	}
	return PATCHADDRESS.at(name)[FORCEGAME];// PATCHADDRESS[name][FORCEGAME];
}