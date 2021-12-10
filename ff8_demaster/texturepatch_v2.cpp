#include "coreHeader.h"


#define DEBUGOUT FALSE

/*
11 - BATTLE MONSTER + SCENERY
18 - WORLD MAP TEXTURE
25 - FIELD BACKGROUND [DONE]
35 - BATTLE CHARACTER PLAYER [DONE]
57 - FIELD ENTITY [DONE]
OTHER - SYSTEM, MENU, PROLOGUE etc. [AS-IS]
*/

BYTE* cltBackAdd1;
BYTE* cltBackAdd2;
DWORD* _thisFF8 = 0; //__thiscall, so this is ECX



const char* const GetTextureType(int texType)
{
	static char textureType[4]{0};
	switch (texType)
	{
	case 11:
		return "BATTLE";
	case 18:
		return "WORLDMAP";
	case 25:
		return "FIELDBG";
	case 35:
		return "BATTLECHARACTER";
	case 57:
		return "FIELDENTITY";
	default:
		sprintf(textureType, "%02d", texType % 100);
		return textureType;
	}
}

__declspec(naked) void _cltObtainTexHeader()
{
	__asm
	{
		MOV tex_header, ESI
		MOV gl_textures, EDI //gl_textures is actually tex_struct

		//ORIGINAL CODE
		PUSH ECX
		PUSH EDX //this one gets cleared after 1020
		PUSH UPLOADVRAM
		CALL GetAddress

		MOV ECX, EAX
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, ECX //upload_texture_vram
		POP EDX
		POP ECX
		CALL EAX
			
		JMP cltBackAdd2
	}
}

void _cltVoid()
{
	int textureType = tex_struct[48];
	TEX_TYPE = textureType;
	int palette = tex_header[52];
	UINT unknownDword = tex_struct[65];
	BOOL bHaveHD = tex_struct[49];
	DWORD unknownCheckHD = tex_struct[47];
	DWORD tPage = tex_struct[50];



	char n[255];
	n[0] = '\0';
	if (textureType == 0)
		return;
	if (textureType == 18 && (tPage <= 15 || tPage>=28))
		return;
	if (textureType == 57) //field
		return;
	OutputDebug("\ncommon_load_texture: tex_type: %s, pal: %d, unk: %08x, bHaveHD: %s, Tpage: %d\n", GetTextureType(textureType), palette, unknownDword, bHaveHD > 0 ? "TRUE" : "FALSE", tPage);

	return;
}

__declspec(naked) void _cltObtainTexStructDebug()
{
	__asm
	{
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX
		MOV tex_struct, EDX
		CALL _cltVoid

		POP EDX
		POP ECX
		POP EBX
		POP EAX

		//ORIGINAL CODE

		CMP dword ptr[edx + 0x0BC], 0
		JMP cltBackAdd1
	}
}

void ReplaceTextureFunction()
{
	OutputDebug("Applying texture patches...\n");
	if (BATTLE_CHARA)
	{
		OutputDebug("Applying BATTLE_CHARA patch\n");
		ApplyBattleCharacterPatch();
	}
	if (FIELD_ENTITY)
	{
		OutputDebug("Applying FIELD_ENTITY patch\n");
		ApplyFieldEntityPatch();
	}
	if (BATTLE_HOOK)
	{
		OutputDebug("BATTLE_HOOK- STARTING PATCHING\n");
		ApplyBattleHookPatch();
		ApplyBattleMonsterPatch();
		ApplyBattleFieldPatch();
	}
	if (FIELD_BACKGROUND)
	{
		OutputDebug("Applying FIELD_BACKGROUND PATCH\n");
		ApplyFieldBackgroundPatch();
	}
	if (WORLD_TEXTURES)
	{
		OutputDebug("Applying WORLD_TEXTURES PATCH\n");
		ApplyWorldPatch();
	}

	cltBackAdd2 = InjectJMP(IMAGE_BASE + GetAddress(CLTBACKADD1), (DWORD)_cltObtainTexHeader, 5);
	cltBackAdd1 = InjectJMP(IMAGE_BASE + GetAddress(CLTBACKADD2), (DWORD)_cltObtainTexStructDebug, 7);
}