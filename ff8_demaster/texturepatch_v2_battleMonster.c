#include "coreHeader.h"
#include "texturepatch_battle_data.h"

DWORD* tex_struct;
DWORD* langIdent_ESI;

int width_bmp = 768;
int height_bmp = 768;

BYTE* bmpBackAdd1;
BYTE* bmpBackAdd2;
BYTE* bmpBackAdd3;

void battleSetProperResolution(int texId, int secondParam, int* width, int* height)
{
	if (texId < 0x387) {
		if (texId != 0x386) {
			if (texId == 0x5f) {
				*width = 0x600;
				*height = 0x300;
				return;
			}
			if (texId != 99) {
				if (texId != 0xb8) {
					return;
				}
				*width = 0x480;
				*height = 0x300;
				return;
			}
			if (secondParam != 1) {
				return;
			}
			*width = 0x480;
			*height = 0x300;
			return;
		}
	}
	else {
		if (texId != 0x446) {
			if (texId != 0x461) {
				return;
			}
			if (secondParam != texId - 0x461) {
				return;
			}
		}
	}
	*width = 0x300;
	*height = 0x300;
	return;
}

//casual is 384x384, therefore the final should be 1st texture *2
void _bmpObtainTextureDatas(int aIndex)
{
	//char n[256];
	//int width_, height_, channels;
	//sprintf(n, "%stextures\\battle.fs\\hd_new\\d%xc%03u_0.png", DIRECT_IO_EXPORT_DIR, (aIndex - 4097) / 100, (aIndex - 4097) % 100);
	//unsigned char* buffer = stbi_load(n, &width_, &height_, &channels, 0); //chara 0
	//width_bmp = width_ * 2;
	//height_bmp = height_ * 2;

	//char out[256];
	//sprintf(out, "_bcpObtainTextureDatas:: width=%d, height=%d, filename=%s", width_bcp, height_bcp, n);
	//OutputDebugStringA(out);
	//stbi_image_free(buffer);
	return;
}

__declspec(naked) void _bmpObtainData()
{
	__asm
	{
		LEA EAX, dword ptr[EDI+0x10]
		MOV tex_struct, EAX
		MOV EAX, dword ptr[EDI + 0x10]

		PUSH EAX
		CALL _bmpObtainTextureDatas
		POP EAX

		PUSH 0
		PUSH 0
		PUSH 0
		PUSH[height_bmp]
		PUSH[width_bmp]
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x160b670 //createGLTexture
		CALL EAX

		JMP bmpBackAdd1
	}
}

void ApplyBattleMonsterPatch()
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
	bmpBackAdd1 = InjectJMP(IMAGE_BASE + 0x16068E4, (DWORD)_bmpObtainData, 18);



}