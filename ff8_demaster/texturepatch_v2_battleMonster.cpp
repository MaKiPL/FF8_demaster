#include "coreHeader.h"
#include "texturepatch_battle_data.h"

void ApplyBattleMonsterPatch()
{
	//step 1. Patch calls to our struct instead of vanilla to avoid GetHdTexInfo: id error.
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON0), (DWORD)&TexFuncMonsterTexProvider->filename);
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON1), (DWORD)TexFuncMonsterTexProvider);
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON2), (DWORD)&TexFuncMonsterTexProvider->filename);
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON3), (DWORD)&TexFuncMonsterTexProvider->filename);

	//this patches force release of tex 84 that is invalid/wrong pointer
	ModPage(IMAGE_BASE + GetAddress(BTLMON4), 6);
	*(DWORD*)(IMAGE_BASE + GetAddress(BTLMON4)) = 0x0000D8E9;
	*(WORD*)(IMAGE_BASE + GetAddress(BTLMON5)) = 0x9000;


	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON6), (DWORD)TexFuncMonsterTexProvider);
}