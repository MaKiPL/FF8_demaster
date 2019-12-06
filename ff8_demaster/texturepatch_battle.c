//#include "coreHeader.h"
//#include <gl/GL.h>
//#include "texturepatch_battle_data.h"
//
//signed int texRep_monster_switchBegin(int v77) //I DON'T KNOW
//{
//	signed int result;
//	if (v77 >= 10 && v77 <= 12)
//		return 1;
//	else if (v77 >= 13 && v77 <= 15)
//		return 2;
//	return -1;
//}
//
//void battleSetProperResolution(int texId, int secondParam, int* width, int* height)
//{
//	if (texId < 0x387) {
//		if (texId != 0x386) {
//			if (texId == 0x5f) {
//				*width = 0x600;
//				*height = 0x300;
//				return;
//			}
//			if (texId != 99) {
//				if (texId != 0xb8) {
//					return;
//				}
//				*width = 0x480;
//				*height = 0x300;
//				return;
//			}
//			if (secondParam != 1) {
//				return;
//			}
//			*width = 0x480;
//			*height = 0x300;
//			return;
//		}
//	}
//	else {
//		if (texId != 0x446) {
//			if (texId != 0x461) {
//				return;
//			}
//			if (secondParam != texId - 0x461) {
//				return;
//			}
//		}
//	}
//	*width = 0x300;
//	*height = 0x300;
//	return;
//}
//
//BYTE TexFuncBattleMonsterSegment(int this_, DWORD* v4)
//{
//	int v81,v82 = 1,v83,v84,v85=0,v99; //v82 is true to conform the disassembly JUMPING routines
//
//	int v77 = tex_struct[50]; //0x0d
//	int v78 = 52 * v77 + this_ + 2984; //looks like pointer to TexStruct by v77 in FF8System
//	int v79 = tex_struct[51] | tex_struct[52] << 16; //v79 now holds the texId
//	int ArgList = v79;
//	int v80 = *(DWORD*)v78;
//	if (!*(DWORD*)v78 || (v99 = 1, *(DWORD*)(v78 + 8) == v79))
//		v99 = 0;
//
//	if (v80 && texRep_monster_switchBegin(v77) == 2)
//	{
//		v81 = *(DWORD*)(tex_struct + 212);
//		v82 = 0;
//		v83 = 0;
//		if (v81 >= 0x3E8)
//			v82 = *(DWORD*)(v78 + 20) != v81;
//		v84 = *(DWORD*)(tex_struct + 220);
//		if (v84 >= 0x3E8)
//			v83 = *(DWORD*)(v78 + 24) != v84;
//		if (!v82)
//			v85 = v83 == 0;
//	}
//	else
//	{
//		v85 = v99 == 0;
//	}
//	if (v82 && !v85) //; //this is probably clearing the texStruct vars
//	{
//		DWORD* test1 = (DWORD*)(this_ + 1620);
//		__int8* test2 = (unsigned __int8*)(*(DWORD*)v78 + 8);
//
//		//sub_1004BBF0((_DWORD*)(_this + 1620), &height, (unsigned __int8*)(*(_DWORD*)v78 + 8));
//		//if (height != *(_DWORD*)(_this + 1620))
//		//	sub_11601560((void**)(_this + 1620), (int)&height, (void*)height);
//		//if (*(_DWORD*)v78)
//		//	(***(void(__stdcall****)(signed int))v78)(1);
//		//*(_DWORD*)v78 = 0;
//		//*(_BYTE*)(v78 + 4) = 0;
//		//*(_DWORD*)(v78 + 12) = 0;
//		//*(_BYTE*)(v78 + 16) = 0;
//		//*(_DWORD*)(v78 + 44) = 0;
//		//if (*(_DWORD*)(tex_struct + 212) >= 0x3E8u)
//		//{
//		//	*(_DWORD*)(v78 + 28) = 0;
//		//	*(_DWORD*)(v78 + 36) = 0;
//		//}
//		//if (*(_DWORD*)(tex_struct + 220) >= 0x3E8u)
//		//{
//		//	*(_DWORD*)(v78 + 32) = 0;
//		//	*(_DWORD*)(v78 + 40) = 0;
//		//}
//	}
//
//
////LABEL_227:
//	//[0x1605c50 / textureFunction:661]
//
//	char n[256];
//	char nb[256];
//	*n = 0x00;
//	*nb = 0x00;
//	if (*(int*)v78 == 0)
//	{
//		int width = 0x300;
//		int height = 0x300;
//
//		unsigned int var1 = tex_struct[51];
//		int var2 = tex_struct[52];
//		battleSetProperResolution(var1, var2, &width, &height);
//
//
//		glGenTextures(1, &OPENGL_TEXTURES);
//		glBindTexture(GL_TEXTURE_2D, OPENGL_TEXTURES);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//		*(int*)(v78+2) = this_;
//
//
//				struct TexFuncMonsterTex selectedStruct;
//		int testSwitch = *(v4 + 3); //0xD
//		switch (*(v4+3))
//		{
//		default:
//			break;
//		case 0xD:
//		case 0xE:
//		case 0xF:
//		{
//			int getTexId = v4[6];
//			if (getTexId > 0x3E7) //base texture
//			{
//				BOOL bFound = FALSE;
//				for (int i = 0; i < TexFuncMonsterTexProviderLength; i++)
//				{
//					if (getTexId == TexFuncMonsterTexProvider[i].fileIndex)
//					{
//						selectedStruct = TexFuncMonsterTexProvider[i];
//						bFound = TRUE;
//						break;
//					}
//				}
//				if (!bFound)
//					return -1;
//				int numOfPalettes = v4[7];
//				switch (numOfPalettes)
//				{
//				case 0:
//					sprintf(n, "%stextures\\battle.fs\\hd_new\\%s_0.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 1:
//					sprintf(n, "%stextures\\battle.fs\\hd_new\\%s_1.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 2:
//					sprintf(n, "%stextures\\battle.fs\\hd_new\\%s_2.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 3:
//					sprintf(n, "%stextures\\battle.fs\\hd_new\\%s_3.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 4:
//					sprintf(n, "%stextures\\battle.fs\\hd_new\\%s_4.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 5:
//					sprintf(n, "%stextures\\battle.fs\\hd_new\\%s_5.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				}
//			}
//			getTexId = v4[8];
//			if (getTexId > 0x3E7) //second texture
//			{
//				BOOL bFound = FALSE;
//				for (int i = 0; i < TexFuncMonsterTexProviderLength; i++)
//				{
//					if (getTexId == TexFuncMonsterTexProvider[i].fileIndex)
//					{
//						selectedStruct = TexFuncMonsterTexProvider[i];
//						bFound = TRUE;
//						break;
//					}
//				}
//				if (!bFound)
//					return -1;
//				int numOfPalettes = v4[9];
//				switch (numOfPalettes)
//				{
//				case 0:
//					sprintf(nb, "%stextures\\battle.fs\\hd_new\\%s_0.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 1:
//					sprintf(nb, "%stextures\\battle.fs\\hd_new\\%s_1.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 2:
//					sprintf(nb, "%stextures\\battle.fs\\hd_new\\%s_2.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 3:
//					sprintf(nb, "%stextures\\battle.fs\\hd_new\\%s_3.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 4:
//					sprintf(nb, "%stextures\\battle.fs\\hd_new\\%s_4.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				case 5:
//					sprintf(nb, "%stextures\\battle.fs\\hd_new\\%s_5.png", DIRECT_IO_EXPORT_DIR, selectedStruct.filename);
//					break;
//				}
//			}
//			break;
//		}
//		}
//
//
//	}
//
//
//
//
//
//
//	if (*n != 0x00) {
//		int width = 0, height = 0, channels = 0;
//		unsigned char* buffer = stbi_load(n, &width, &height, &channels, 0); //chara 0
//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffer);
//		stbi_image_free(buffer);
//	}
//	if (*nb != 0x00) {
//		int width = 0, height = 0, channels = 0;
//		unsigned char* buffer = stbi_load(nb, &width, &height, &channels, 0); //chara 0
//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0x180, width, height, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffer); //0x180 is the texture size!!
//		stbi_image_free(buffer);
//	}
//
//
//	OutputDebugStringA(n);
//	OutputDebugStringA(nb);
//	return 0;
//}