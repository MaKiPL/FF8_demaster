#include "coreHeader.h"

unsigned int uvPatchAddresses[] =
{
	GetAddress(UVPATCH0),GetAddress(UVPATCH1),GetAddress(UVPATCH2),GetAddress(UVPATCH3),GetAddress(UVPATCH4),GetAddress(UVPATCH5),
	GetAddress(UVPATCH6),GetAddress(UVPATCH7),GetAddress(UVPATCH8),GetAddress(UVPATCH9),GetAddress(UVPATCH10),GetAddress(UVPATCH11),
};

BOOL uvPatchModMnemonic(DWORD address)
{
	if (!ModPage(address, 4))
		return FALSE;
	*((BYTE*)address + 3) = 0;
	return TRUE;
}

void ApplyUVPatch()
{
	BYTE* firstMnemonic = (BYTE*)(IMAGE_BASE + GetAddress(UVFUNCPNTR));
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