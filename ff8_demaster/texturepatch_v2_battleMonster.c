#include "coreHeader.h"
#include "texturepatch_battle_data.h"

void ApplyBattleMonsterPatch()
{
	//step 1. Patch calls to our struct instead of vanilla to avoid GetHdTexInfo: id error.
	InjectDWORD(IMAGE_BASE + 0x15FF3D0, &TexFuncMonsterTexProvider->filename);
	InjectDWORD(IMAGE_BASE + 0x15FF3DA, TexFuncMonsterTexProvider);
	InjectDWORD(IMAGE_BASE + 0x15FF3F9, &TexFuncMonsterTexProvider->filename);
	InjectDWORD(IMAGE_BASE + 0x15FF400, &TexFuncMonsterTexProvider->filename);
	//InjectDWORD(IMAGE_BASE + 0x1601CE2, TexFuncMonsterTexProvider);
	//InjectDWORD(IMAGE_BASE + 0x1601CFE, TexFuncMonsterTexProvider);

	//InjectDWORD(IMAGE_BASE + 0x157DB2C+2, &TexFuncMonsterTexProvider->filename);
	//InjectDWORD(IMAGE_BASE + 0x157DB53+2, &TexFuncMonsterTexProvider->fileIndex);
	//InjectDWORD(IMAGE_BASE + 0x157DB5E+2, &TexFuncMonsterTexProvider->dUnk);
	//InjectDWORD(IMAGE_BASE + 0x157DB67+2, &TexFuncMonsterTexProvider->wUnk);
	//InjectDWORD(IMAGE_BASE + 0x157DB70+2, &TexFuncMonsterTexProvider->wUnk2);
	//InjectDWORD(IMAGE_BASE + 0x157DB79+2, &TexFuncMonsterTexProvider->wUnk3);
	//InjectDWORD(IMAGE_BASE + 0x157DB82+2, &TexFuncMonsterTexProvider->wUnk4);
	//InjectDWORD(IMAGE_BASE + 0x157DB94+2, &TexFuncMonsterTexProvider->filename);

	//InjectDWORD(IMAGE_BASE + 0x1581C83+2, &TexFuncMonsterTexProvider->dUnk);
	//InjectDWORD(IMAGE_BASE + 0x1581C8A+2, &TexFuncMonsterTexProvider->fileIndex);
	//InjectDWORD(IMAGE_BASE + 0x1581C93+2, &TexFuncMonsterTexProvider->unk);
	//InjectDWORD(IMAGE_BASE + 0x1581DEA+2, &TexFuncMonsterTexProvider->dUnk);
	//InjectDWORD(IMAGE_BASE + 0x1581DF1+2, &TexFuncMonsterTexProvider->fileIndex);
	//InjectDWORD(IMAGE_BASE + 0x1581DFA+2, &TexFuncMonsterTexProvider->unk);

	//InjectDWORD(IMAGE_BASE + 0x1601CE1+1, &TexFuncMonsterTexProvider->fileIndex);
	//InjectDWORD(IMAGE_BASE + 0x1601CFC+2, &TexFuncMonsterTexProvider->fileIndex);

	//this patches force release of tex 84 that is invalid/wrong pointer
	modPage(IMAGE_BASE + 0x1601E02, 6);
	*(DWORD*)(IMAGE_BASE + 0x1601E02) = 0x0000D8E9;
	*(WORD*)(IMAGE_BASE + 0x1601E06) = 0x9000;


	InjectDWORD(IMAGE_BASE + 0x1601CFE, TexFuncMonsterTexProvider);

	//battlemonster struct
	//InjectDWORD(IMAGE_BASE + 0x157DCD5 + 2, &BattleHookProvider->filename);
	//InjectDWORD(IMAGE_BASE + 0x157DCE0 + 1, &BattleHookProvider->filename);
	//InjectDWORD(IMAGE_BASE + 0x157DCF4 + 3, &BattleHookProvider->filename);
	//InjectDWORD(IMAGE_BASE + 0x157DCFC + 3, &BattleHookProvider->filename);

}