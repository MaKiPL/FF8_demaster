#include "coreHeader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <gl/GL.h>


//const DWORD TEXPATCH1 = 0x16054FE; //first: 0x15A992F;
//const DWORD TEXPATCH2 = 0x1606A1C; //first: 0x15AA3E6;		//0x15AA3EB; [as above]
//const DWORD TEXPATCH3 = 0x1603900; //first: 0x15A9842;
//const DWORD TEXPATCH4 = 0x1601E4B; //first: 0x15AC4A4;




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

__declspec(naked) void _cltObtainTexHeader()
{
	__asm
	{
		MOV tex_header, ESI
		MOV gl_textures, EDI //gl_textures is actually tex_struct

		//ORIGINAL CODE
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x1573A40 //upload_texture_vram
		CALL EAX
			
		JMP cltBackAdd2
	}
}

void _cltVoid()
{
	int textureType = tex_struct[48];
	int palette = tex_header[52];
	UINT unknownDword = tex_struct[65];
	BOOL bHaveHD = tex_struct[49];
	DWORD unknownCheckHD = tex_struct[47];
	DWORD tPage = tex_struct[50];

	char n[255];
	if (textureType != 57) { //I don't want field chara tex update spam - you may want to delete that if you want full debug
		sprintf(n, "common_load_texture: tex_type: %d, pal: %d, unk: %08x, bHaveHD: %s, Tpage: %d\n", textureType, palette, unknownDword, bHaveHD > 0 ? "TRUE" : "FALSE", tPage);
		OutputDebugStringA(n);
	}

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
	OutputDebugStringA("Applying texture patches...");
	ApplyBattleCharacterPatch();
	ApplyFieldEntityPatch();
	ApplyBattleMonsterPatch();
	ApplyFieldBackgroundPatch();

	cltBackAdd2 = InjectJMP(IMAGE_BASE + 0x155CD05, (DWORD)_cltObtainTexHeader, 5);
	cltBackAdd1 = InjectJMP(IMAGE_BASE + 0x155CD7A, (DWORD)_cltObtainTexStructDebug, 7);

	//int textureFunction = IMAGE_BASE + TEXPATCH1;	//0x15A9920; [comment for no SEH/rewind]
	//tex_returnAddress = IMAGE_BASE + TEXPATCH2;		//0x15AA3EB; [as above]
	//modPage(textureFunction, 5);
	//*(BYTE*)(textureFunction) = 0xE9;
	//DWORD jmpparm = (DWORD)LoadGameTexture - textureFunction - 5;
	//*(DWORD*)(textureFunction + 1) = jmpparm;
}