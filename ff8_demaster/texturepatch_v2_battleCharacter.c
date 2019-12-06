#include "coreHeader.h"
#include <gl/GL.h>
//#include "texturepatch_battle_data.h"

BYTE* InjectJMP(DWORD address, DWORD functionAddress, int JMPsize);

DWORD* tex_struct;
DWORD* langIdent_ESI;

int width = 768;
int height = 768;

BYTE* backAdd1;
BYTE* backAdd2;
BYTE* backAdd3;

//casual is 384x384, therefore the final should be 1st texture *2
void _bcpObtainTextureDatas(int aIndex)
{
	char n[256];
	int width_, height_, channels;
	sprintf(n, "%stextures\\battle.fs\\hd_new\\d%xc%03u_0.png", DIRECT_IO_EXPORT_DIR, (aIndex - 4097) / 100, (aIndex - 4097) % 100);
	unsigned char* buffer = stbi_load(n, &width_, &height_, &channels, 0); //chara 0
	width = width_ * 2;
	height = height_ * 2;

	char out[256];
	sprintf(out, "_bcpObtainTextureDatas:: width=%d, height=%d, filename=%s", width, height, n);
	OutputDebugStringA(out);
	stbi_image_free(buffer);
	return;
}

__declspec(naked) void _bcpObtainData()
{
	__asm
	{
		MOV EAX, dword ptr [EDI+0x10]
		MOV tex_struct, EAX
		MOV langIdent_ESI, ESI

		PUSH EAX
		CALL _bcpObtainTextureDatas
		POP EAX

		PUSH 0
		PUSH 0
		PUSH 0
		PUSH [height]
		PUSH [width]
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x160b670 //createGLTexture
		CALL EAX

		JMP backAdd1
	}
}

__declspec(naked) void _bcpPushHeightOffsetY()
{
	__asm
	{
		//assembler already pushed eax, so we can use it
		MOV EAX, [height]
		SHR EAX, 1
		PUSH EAX
		JMP backAdd2
	}
}

__declspec(naked) void _bcpPushHeightOffsetY_minusHeight()
{
	__asm
	{
		//assembler already pushed eax, so we can use it
		MOV EAX, [height]
		SHR EAX, 3
		NEG EAX //we now have -24
		ADD EAX, [height] //-24 + height = correct Y


		PUSH EAX
		JMP backAdd2
	}
}

void ApplyBattleCharacterPatch()
{
	//step  D* creates glTexture 768x768 and by then produces paths for chara + weapon if needed-
	//		-this is bad, because we need to know the resolutions and then create gl for mixed res textures
	//		therefore we replace the segment of only loading the texture and it should work fine - less data to work with

	/*
	==				ORIGINAL DISSASEMBLY PSEUDOCODE			==

	if ( !glTexturePointer )
		{
1		  v72 = CreateGLTexture(768, 768, 0, 0, 0);			<-- this should be called at the end- or we should just gather the resolutions before that
2		  glTexturePointer = v72;
3		  *(_DWORD *)langIdentifier = v72;		<-- this is important as it tells ff8system that we got the texture, later it's used for glsubtex
4		  strappend(&filePath, (int)&Dst, "BATTLE.FS\\hd");
5		  bShouldLoadBattleCharTex = *(_DWORD *)(tex_struct + 204);
6		  LOBYTE(v128) = 33;
7		  if ( bShouldLoadBattleCharTex )
8		  {
9			v74 = LoadBattleCharaTexture((int *)_this, glTexturePointer, bShouldLoadBattleCharTex, (int)&filePath);
10			*(_BYTE *)(height + 17) = v74;
11		  }
12		  bShouldLoadWeapon = *(_DWORD *)(tex_struct + 208);
13		  if ( bShouldLoadWeapon )
14			loadWeaponTexture(_this, glTexturePointer, bShouldLoadWeapon, &filePath);
15		  v76 = height;
16		  LOBYTE(v128) = 0;
17		  *(_DWORD *)height = glTexturePointer;
18		  *(_DWORD *)(v76 + 8) = *(_DWORD *)(tex_struct + 204);
19		  delete(&filePath);
20		}
	
	
	*/

	//step 1. obtain needed data for tex_struct and etc.
	backAdd1 = InjectJMP(IMAGE_BASE + 0x16066BD, (DWORD)_bcpObtainData, 18);

	//step 2. we now have correct dynamic glTexture, but we need to change the LoadBattleCharaTexture and WeaponTexture
	//			because they are still using static resolutions for getting the images (actually only the yoffsets)
	//	see at LoadBattleCharaTexture we have something like this:

	/*
	==DISASSEMBLY PSEUDOCODE==

	  texture__loadFromArchive(param_2,0,0,puVar4,(int **)pppppiVar5,puVar3,ppiVar6); <-- here we have 0,0 for x and y offsets
  (...)
	  texture__loadFromArchive(param_2,0,0x180,puVar4,(int **)pppppiVar5,puVar3,ppiVar6); <-- see how it's 0x180 const y offset? that's bad ;-;

											^
											|
											|> here, we have to do JMP to PUSH height/2 instead of const 0x180

	*/


	backAdd2 = InjectJMP(IMAGE_BASE + 0x1604087, (DWORD)_bcpPushHeightOffsetY, 5);


	//step 3. The last thing we need is replacing the weapon texture. The Y offset is 0x2a0 (768-96)
	
	backAdd3 = InjectJMP(IMAGE_BASE + 0x1605040, (DWORD)_bcpPushHeightOffsetY_minusHeight, 5);


	//VOILA! Battle characters finished without replacing whole functions!
	
}