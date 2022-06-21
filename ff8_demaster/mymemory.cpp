#include "mymemory.h"

//uhm
void InitTable(unsigned int baseAddress)
{


	for(std::pair<const ADDRESS_NAME, std::vector<unsigned>> key : PATCHADDRESS)
	{

		
	}
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