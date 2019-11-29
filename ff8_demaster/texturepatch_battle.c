#include "coreHeader.h"
#include <gl/GL.h>
#include "texturepatch_battle_data.h"

signed int texRep_monster_switchBegin(int v77) //I DON'T KNOW
{
	signed int result;
	if (v77 >= 10 && v77 <= 12)
		return 1;
	else if (v77 >= 13 && v77 <= 15)
		return 2;
	return -1;
}

BYTE TexFuncBattleMonsterSegment(int this_, DWORD* v4)
{
	int v81,v82 = 1,v83,v84,v85=0,v99; //v82 is true to conform the disassembly JUMPING routines

	int v77 = tex_struct[50]; //0x0d
	int v78 = 52 * v77 + this_ + 2984; //looks like pointer to TexStruct by v77 in FF8System
	int v79 = tex_struct[51] | tex_struct[52] << 16; //v79 now holds the texId
	int ArgList = v79;
	int v80 = *(DWORD*)v78;
	if (!*(DWORD*)v78 || (v99 = 1, *(DWORD*)(v78 + 8) == v79))
		v99 = 0;

	if (v80 && texRep_monster_switchBegin(v77) == 2)
	{
		v81 = *(DWORD*)(tex_struct + 212);
		v82 = 0;
		v83 = 0;
		if (v81 >= 0x3E8)
			v82 = *(DWORD*)(v78 + 20) != v81;
		v84 = *(DWORD*)(tex_struct + 220);
		if (v84 >= 0x3E8)
			v83 = *(DWORD*)(v78 + 24) != v84;
		if (!v82)
			v85 = v83 == 0;
	}
	else
	{
		v85 = v99 == 0;
	}
	if (v82 && !v85) //;
	{
		//sub_1004BBF0((_DWORD*)(_this + 1620), &height, (unsigned __int8*)(*(_DWORD*)v78 + 8));
		//if (height != *(_DWORD*)(_this + 1620))
		//	sub_11601560((void**)(_this + 1620), (int)&height, (void*)height);
		//if (*(_DWORD*)v78)
		//	(***(void(__stdcall****)(signed int))v78)(1);
		//*(_DWORD*)v78 = 0;
		//*(_BYTE*)(v78 + 4) = 0;
		//*(_DWORD*)(v78 + 12) = 0;
		//*(_BYTE*)(v78 + 16) = 0;
		//*(_DWORD*)(v78 + 44) = 0;
		//if (*(_DWORD*)(tex_struct + 212) >= 0x3E8u)
		//{
		//	*(_DWORD*)(v78 + 28) = 0;
		//	*(_DWORD*)(v78 + 36) = 0;
		//}
		//if (*(_DWORD*)(tex_struct + 220) >= 0x3E8u)
		//{
		//	*(_DWORD*)(v78 + 32) = 0;
		//	*(_DWORD*)(v78 + 40) = 0;
		//}
	}


//LABEL_227:
	//[0x1605c50 / textureFunction:661]

	//they create 768x768 texbuffer first
	//they call battleMonster_setProperTexResolution- based on that weird texId value they
	//either create 768x768 or different sized textures

	//now they call casual CreateGLTexture with obtained width and height


	int requestedTexId = v4[6]; //this should be our texId [yeah]

	struct TexFuncMonsterTex selectedStruct;
	BOOL bFound = FALSE;
	for (int i = 0; i < TexFuncMonsterTexProviderLength; i++)
	{
		if (requestedTexId == TexFuncMonsterTexProvider[i].fileIndex)
		{
			selectedStruct = TexFuncMonsterTexProvider[i];
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
		return -1;

	glGenTextures(1, &OPENGL_TEXTURES);
	glBindTexture(GL_TEXTURE_2D, OPENGL_TEXTURES);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	char n[256];
	sprintf(n, "%stextures\\battle.fs\\hd_new\\%s.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);

	int width, height, channels;
	unsigned char* buffer = stbi_load(n, &width, &height, &channels, 0); //chara 0

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas
	stbi_image_free(buffer);
	OutputDebugStringA(selectedStruct.filename);
	return 0;
}

BYTE TexFuncBattleSegment(int this_, int aIndex, int bIndex)
{
	glGenTextures(1, &OPENGL_TEXTURES);
	glBindTexture(GL_TEXTURE_2D, OPENGL_TEXTURES);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	char n[256];
	char nb[256];
	char nc[256];
	sprintf(n, "%stextures\\battle.fs\\hd_new\\d%xc%03u_", DIRECT_IO_EXPORT_DIR, (aIndex - 4097) / 100, (aIndex - 4097) % 100);
	sprintf(nc, "%stextures\\battle.fs\\hd_new\\d%xw%03u_0.png", DIRECT_IO_EXPORT_DIR, (bIndex - 5197) / 100, (bIndex - 5197) % 100);
	strcpy(nb, n);
	strcat(n, "0.png");
	strcat(nb, "1.png");



	int width, height, channels;
	unsigned char* buffer = stbi_load(n, &width, &height, &channels, 0); //chara 0

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width * 2, height * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas

	int width2 = 0, height2 = 0, width3 = 0, height3 = 0, channels2= 0, channels3 = 0;
	unsigned char* buffertwo = stbi_load(nb, &width2, &height2, &channels2, 0); //chara 0
	unsigned char* bufferthree = stbi_load(nc, &width3, &height3, &channels3, 0); //chara 0


	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, channels==4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, height, width2, height2, channels2 == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffertwo);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, height + height2, width3, height3, channels3 == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, bufferthree);

	stbi_image_free(bufferthree);
	stbi_image_free(buffertwo);
	stbi_image_free(buffer);
	strcat(n, "\n");
	strcat(nb, "\n");
	strcat(nc, "\n");
	OutputDebugStringA(n);
	OutputDebugStringA(nb);
	OutputDebugStringA(nc);

	return 0;
}