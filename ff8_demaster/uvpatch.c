#include "coreHeader.h"

unsigned int uvPatchAddresses[] =
{
	0x8A2CB2,
	0x8A2CB8,
	0x8A2D17,
	0x8A2D1D,
	0x8A2D6D,
	0x8A2D73,
	0x8A2DC5,
	0x8A2DCB,
	0x8A2E1B,
	0x8A2E21,
	0x8A2E6F,
	0x8A2E75
};


BOOL uvPatchModMnemonic(DWORD address)
{
	if (!modPage(address, 4))
		return FALSE;
	*((BYTE*)address + 3) = 0;
	return TRUE;
}



void ApplyUVPatch()
{
	BYTE* firstMnemonic = IMAGE_BASE + 0x8A2CB2;
	DWORD addMnemonic = *(DWORD*)firstMnemonic & 0xFFFFFF; //get mnemonic without parameter
	if (addMnemonic != 0x084680)
	{
		OutputDebugStringA("DEMASTER::ApplyUvPatch::The addresses are wrong! We found no ADD byte ptr[esi+8] at given rel call\n");
		return;
	}
	for (int i = 0; i < 12; i++)
		if (!uvPatchModMnemonic(IMAGE_BASE + uvPatchAddresses[i]))
			return;
}