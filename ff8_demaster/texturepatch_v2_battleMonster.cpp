#include "coreHeader.h"
#include "texturepatch_battle_data.h"

void ApplyBattleMonsterPatch()
{
	//step 1. Patch calls to our struct instead of vanilla to avoid GetHdTexInfo: id error.
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON0), reinterpret_cast<DWORD>(&TexFuncMonsterTexProvider->filename));
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON1), reinterpret_cast<DWORD>(TexFuncMonsterTexProvider));
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON2), reinterpret_cast<DWORD>(&TexFuncMonsterTexProvider->filename));
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON3), reinterpret_cast<DWORD>(&TexFuncMonsterTexProvider->filename));

	//this patches force release of tex 84 that is invalid/wrong pointer
	ModPage(IMAGE_BASE + GetAddress(BTLMON4), 6);
	*reinterpret_cast<DWORD*>(IMAGE_BASE + GetAddress(BTLMON4)) = 0x0000D8E9;
	*reinterpret_cast<WORD*>(IMAGE_BASE + GetAddress(BTLMON5)) = 0x9000;
	
	InjectDWORD(IMAGE_BASE + GetAddress(BTLMON6), reinterpret_cast<DWORD>(TexFuncMonsterTexProvider));
}