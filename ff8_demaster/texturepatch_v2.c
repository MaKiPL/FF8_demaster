#include "coreHeader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <gl/GL.h>


//const DWORD TEXPATCH1 = 0x16054FE; //first: 0x15A992F;
//const DWORD TEXPATCH2 = 0x1606A1C; //first: 0x15AA3E6;		//0x15AA3EB; [as above]
//const DWORD TEXPATCH3 = 0x1603900; //first: 0x15A9842;
//const DWORD TEXPATCH4 = 0x1601E4B; //first: 0x15AC4A4;



DWORD* _thisFF8 = 0; //__thiscall, so this is ECX

void ReplaceTextureFunction()
{
	OutputDebugStringA("Applying texture patches...");
	ApplyBattleCharacterPatch();
	ApplyFieldEntityPatch();

	//int textureFunction = IMAGE_BASE + TEXPATCH1;	//0x15A9920; [comment for no SEH/rewind]
	//tex_returnAddress = IMAGE_BASE + TEXPATCH2;		//0x15AA3EB; [as above]
	//modPage(textureFunction, 5);
	//*(BYTE*)(textureFunction) = 0xE9;
	//DWORD jmpparm = (DWORD)LoadGameTexture - textureFunction - 5;
	//*(DWORD*)(textureFunction + 1) = jmpparm;
}