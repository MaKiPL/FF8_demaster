#include "coreHeader.h"

unsigned int uvPatchAddresses[] =
{
	0x911A05,
	0x911A0B,
	0x911A67,
	0x911A6D,
	0x911AC4,
	0x911ACA,
	0x911B23,
	0x911B29,
	0x911B80,
	0x911B86,
	0x911BC6,
	0x911BCC

	//0x8A2CB2, //NEW IS 10911A05 so 011A05
	//0x8A2CB8,
	//0x8A2D17,
	//0x8A2D1D,
	//0x8A2D6D,
	//0x8A2D73,
	//0x8A2DC5,
	//0x8A2DCB,
	//0x8A2E1B,
	//0x8A2E21,
	//0x8A2E6F,
	//0x8A2E75
};

BOOL uvPatchModMnemonic(DWORD address)
{
	if (!modPage(address, 4))
		return FALSE;
	*((BYTE*)address + 3) = 0;
	return TRUE;
}

//
DWORD uvFunctionPointer = 0x911A05;
/*

First release =		8A2CB2
Second release =	911A05

*/

void ApplyUVPatch()
{
	BYTE* firstMnemonic = IMAGE_BASE + 0x911A05;
	DWORD addMnemonic = *(DWORD*)firstMnemonic & 0xFFFFFF; //get mnemonic without parameter
	if (addMnemonic != 0x084680)
	{
		OutputDebug("DEMASTER::ApplyUvPatch::The addresses are wrong! We found no ADD byte ptr[esi+8] at given rel call\n");
		return;
	}
	for (int i = 0; i < 12; i++)
		if (!uvPatchModMnemonic(IMAGE_BASE + uvPatchAddresses[i]))
			return;
}