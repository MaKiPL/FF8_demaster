#include "coreHeader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <gl/GL.h>


//for second release their compiler embeded SEH PROLOGUE!
const DWORD TEXPATCH1 = 0x16054FE; //first: 0x15A992F;
const DWORD TEXPATCH2 = 0x1606A1C; //first: 0x15AA3E6;		//0x15AA3EB; [as above]
const DWORD TEXPATCH3 = 0x1603900; //first: 0x15A9842;
const DWORD TEXPATCH4 = 0x1601E4B; //first: 0x15AC4A4;



DWORD* _thisFF8 = 0; //__thiscall, so this is ECX
DWORD tex_returnAddress = 0;

//tex_struct is some way of texturing structure that is quite huge
//parm1 is unknown, maybe pointer?

enum languages
{
	English,
	French,
	German,
	Italian,
	Spanish,
	Japanese
};
enum languages currentLanguage = English;
DWORD test;

char* archiveMockup = "main.zzz";

UINT width;
UINT height;
char tex_paletteIndex[4];
int texMode;
char* langIdentifier = "EN";
char* dinput = "_dinput";
char* xinput = "_xinput";
int textureIndex = -1;
int v147;

DWORD OPENGL_METHOD;

byte* texture_buffer_open;
unsigned char* rgbBuffer;
void* textureRepTable;
char* tex_getFileOpening;

DWORD* v4;

int v38;
int v39;
int v40;

int channels;

void InitTextureRepTable(void* _THIS, int zero, int one, int height, int width, int height2, int width2, int OPENGL_TEXTURES)
{
	int TextureRGBA_vftable = IMAGE_BASE + 0x16A4E4C;
	*(DWORD*)_THIS = TextureRGBA_vftable;
	*(((BYTE*)_THIS) + 4) = 1;
	*(((DWORD*)_THIS) + 2) = OPENGL_TEXTURES; //0x08
	*(((DWORD*)_THIS) + 3) = height; //0x0C
	*(((DWORD*)_THIS) + 4) = width; //0x10
	*(((DWORD*)_THIS) + 5) = height2; //0x14
	*(((DWORD*)_THIS) + 6) = width2; //0x18
	*(((DWORD*)_THIS) + 7) = 0x20; //0x1C
//	*(((DWORD*)_THIS) + 8) = 0x00; //0x20   //already 0 due to calloc
//	*(((BYTE*)_THIS) + 33) = 0; //see above
	//three null DWORDs- already 0 due to calloc
}

void TexFuncGlSegment()
{
	strcat(texPath, ".png");
	OutputDebugStringA(texPath);
	OutputDebugStringA("\n");

	//CREATEGLTEXTURE START
	glGenTextures(1, &OPENGL_TEXTURES);
	glBindTexture(GL_TEXTURE_2D, OPENGL_TEXTURES);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	if (height != 0)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F); //GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	attr = GetFileAttributesA(texPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		memset(texPath, 0, 256);
		strcat(texPath, "EXP\\textures\\null.png");
	}
	rgbBuffer = stbi_load(texPath, &width, &height, &channels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbBuffer);
	stbi_image_free(rgbBuffer);
}

__declspec(naked) void LoadGameTexture()
{
	//it works now under C++ SEH
	__asm
	{
		MOV _thisFF8, ECX
		MOV EBX, [EBP + 8]
		MOV tex_struct, EBX
		MOV EBX, [EBP + 0xC]
		MOV parm1, EBX
	}

	v40 = 0; //RESET FOR DEMASTER FOR FIELD CHARACTER RELOADING

	*(DWORD*)tex_paletteIndex = parm1;
	width = (UINT)(tex_struct + 47);
	v4 = tex_struct + 47;
	if (tex_struct[63] == 1)
	{
		height = parm1 + (tex_struct[48] << 8);
		__asm
		{
			MOV ECX, _thisFF8 //this
			ADD ECX, 0x688 //first 0x684
			//MOV ECX, [ECX]
			LEA EAX, height
			PUSH EAX
			LEA EAX, langIdentifier
			PUSH EAX
			MOV EAX, OFFSET IMAGE_BASE
			MOV EAX, [EAX]
			ADD EAX, 0x4BBF0; //first: 0x15AE068
			CALL EAX

		}
	}
	currentLanguage = *(_thisFF8 + 0x3F);

	texMode = tex_struct[48];
	v147 = 0;

	memset(texPath, 0, 256); //clear path
	strcat(texPath, DIRECT_IO_EXPORT_DIR);
	strcat(texPath, "textures\\");


	//============================                   [BEGIN]                           ==============================\\\\\\
	//============================       RICH MENU SWITCHES, DATA, ICONS, SYSTEM       ==============================\\\\\\
	//============================                                                     ==============================\\\\\\

	if (*(tex_struct + 47) == 1)
	{
		memset(langIdentifier, 0, 3);
		switch (currentLanguage)
		{
		case English:
			strcat(langIdentifier, "EN");
			break;
		case French:
			strcat(langIdentifier, "FR");
			break;
		case German:
			strcat(langIdentifier, "DE");
			break;
		case Italian:
			strcat(langIdentifier, "IT");
			break;
		case Spanish:
			strcat(langIdentifier, "ES");
			break;
		case Japanese:
			strcat(langIdentifier, "JP");
			break;
		}
		textureIndex = -1;
		switch (texMode) //224
		{
		case 1: //zero sysfld
			textureIndex = 0;
		case 2: //non zero sysfld
		{
			if (textureIndex == -1)
				textureIndex = 1;
			if (*(DWORD*)tex_paletteIndex > 7)
			{
				//missing pallete for sysfld.tex
				sprintf(tex_paletteIndex, "%i", textureIndex);
				strcat(texPath, "hires\\sysfld0");
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\7");
			}
			else
			{
				sprintf(tex_paletteIndex, "%u", textureIndex);
				strcat(texPath, "hires\\sysfld0");
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\");
				sprintf(tex_paletteIndex, "%i", parm1);
				strcat(texPath, tex_paletteIndex);
				break;
			}
		}
		case 3: //sysfnt00
		{
			if (*(DWORD*)tex_paletteIndex <= 7)
			{
				sprintf(tex_paletteIndex, "%u", *((DWORD*)tex_paletteIndex));
				strcat(texPath, "hires\\sysfnt00.tex\\");
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, "_hd");
				break;
			}
			else
			{
				strcat(texPath, "hires\\sysfnt00.tex\\7_hd");
				break;
			}
		}
		case 4:
			textureIndex = 0;
			goto LABEL_65;
		case 5:
		LABEL_65:
			if (textureIndex != -1)
			{
				goto LABEL_74;
			}
			textureIndex = 1;
			goto LABEL_67;
		case 6:
			textureIndex = 2;
			goto LABEL_76;
		case 7:
			textureIndex = 3;
		LABEL_74:
			if (textureIndex == 1 || textureIndex == 3)
			{
			LABEL_67:
				strcat(texPath, "hires\\iconfl0");
				sprintf(tex_paletteIndex, "%i", textureIndex);
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\");
				strcat(texPath, langIdentifier);
				strcat(texPath, "\\");
				sprintf(tex_paletteIndex, "%u", parm1);
				strcat(texPath, tex_paletteIndex);
				if (*(DWORD*)tex_paletteIndex == 4)
				{
					switch (_thisFF8[295])
					{
					case 1:
						if (_thisFF8[294] == 2)
						{
							strcat(texPath, xinput);
							break;
						}
						if (_thisFF8[294] == 3)
						{
							strcat(texPath, dinput);
							break;
						}
						strcat(texPath, xinput);
						break;
					case 2:
						strcat(texPath, xinput);
						break;
					case 3:
						strcat(texPath, dinput);
						break;
					}
				}
			}
			else
			{
			LABEL_76:
				strcat(texPath, "hires\\iconfl0");
				sprintf(tex_paletteIndex, "%i", textureIndex);
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\");
				strcat(texPath, langIdentifier);
				strcat(texPath, "\\");
				sprintf(tex_paletteIndex, "%u", parm1);
				strcat(texPath, tex_paletteIndex);
				break;
			}
			break;

		case 8:
			textureIndex = 0;
			goto LABEL_33;
		case 9:
		LABEL_33:
			if (textureIndex != -1)
				goto LABEL_39;
			textureIndex = 1;
			goto LABEL_35;
		case 10:
		LABEL_35:
			if (textureIndex != -1)
				goto LABEL_39;
			textureIndex = 2;
			goto LABEL_37;
		case 11:
		LABEL_37:
			if (textureIndex == -1)
				textureIndex = 3;
		LABEL_39:
			if (*(DWORD*)tex_paletteIndex > 7u)
			{
				strcat(texPath, "hires\\sysevn0");
				sprintf(tex_paletteIndex, "%i", textureIndex);
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\7");
				break;
			}
			else
			{
				strcat(texPath, "hires\\sysevn0");
				sprintf(tex_paletteIndex, "%i", textureIndex);
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\");
				sprintf(tex_paletteIndex, "%u", parm1);
				strcat(texPath, tex_paletteIndex);
				break;
			}
		case 12:
			textureIndex = 0;
			goto LABEL_43;
		case 13:
		LABEL_43:
			if (textureIndex != -1)
				goto LABEL_49;
			textureIndex = 1;
			goto LABEL_45;
		case 14:
		LABEL_45:
			if (textureIndex != -1)
				goto LABEL_49;
			textureIndex = 2;
			goto LABEL_47;
		case 15:
		LABEL_47:
			if (textureIndex == -1)
				textureIndex = 3;
		LABEL_49:
			if (*(DWORD*)tex_paletteIndex > 7u)
			{
				strcat(texPath, "hires\\sysodd0");
				sprintf(tex_paletteIndex, "%i", textureIndex);
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\7");
				break;
			}
			else
			{
				strcat(texPath, "hires\\sysodd0");
				sprintf(tex_paletteIndex, "%i", textureIndex);
				strcat(texPath, tex_paletteIndex);
				strcat(texPath, ".tex\\");
				sprintf(tex_paletteIndex, "%u", parm1);
				strcat(texPath, tex_paletteIndex);
				break;
			}
		case 16:
			if (*(DWORD*)tex_paletteIndex <= 7u)
			{
				strcat(texPath, "font8_even.tex\\");
				sprintf(tex_paletteIndex, "%u", parm1);
				strcat(texPath, tex_paletteIndex);
				break;
			}
			else
			{
				strcat(texPath, "font8_even.tex\\0");
				break;
			}
		case 17:
			if (*(DWORD*)tex_paletteIndex > 7u)
			{
				strcat(texPath, "font8_odd.tex\\0");
				break;
			}
			else
			{
				strcat(texPath, "font8_odd.tex\\");
				sprintf(tex_paletteIndex, "%u", parm1);
				strcat(texPath, tex_paletteIndex);
				break;
			}
		case 18:
		case 19:
		case 20:
		case 21:
			OutputDebugStringA("something bla bla must be removed bla bla");
			__asm
			{
				INT 3
			}
		case 22:
			strcat(texPath, "FIELD.FS\\jp_add_font_hd\\");
			sprintf(tex_paletteIndex, "%u", *(USHORT*)(IMAGE_BASE + 0x351A0)); //first: 0x1768678));
			strcat(texPath, tex_paletteIndex);
			strcat(texPath, "_evn_");
			sprintf(tex_paletteIndex, "%u", parm1 >> 1);
			strcat(texPath, tex_paletteIndex);
			break;
		case 23:
			strcat(texPath, "FIELD.FS\\jp_add_font_hd\\");
			sprintf(tex_paletteIndex, "%u", *(USHORT*)(IMAGE_BASE + 0x351A0)); //first: 0x1768678));
			strcat(texPath, tex_paletteIndex);
			strcat(texPath, "_odd_");
			sprintf(tex_paletteIndex, "%u", parm1 >> 1);
			strcat(texPath, tex_paletteIndex);
			break;
		case 24:
			strcat(texPath, "hires\\face_b00.tex");
			break;
		case 25:
			strcat(texPath, "hires\\face_b01.tex");
			break;
		case 26:
			strcat(texPath, "hires\\gf_big00.tex");
			break;
		case 27:
			strcat(texPath, "hires\\gf_big01.tex");
			break;
		case 28:
			strcat(texPath, "start00.tex");
			break;
		case 29:
			strcat(texPath, "start01.tex");
			break;
		case 30:
			strcat(texPath, "icon.tex\\");
			strcat(texPath, langIdentifier);
			strcat(texPath, "\\");
			sprintf(tex_paletteIndex, "%u", parm1);
			strcat(texPath, tex_paletteIndex);


			if (*(DWORD*)tex_paletteIndex == 4)
			{
				if (_thisFF8[295] != 1)
				{
					if (_thisFF8[295] == 2)
					{
						strcat(texPath, xinput);
					}
					else if (_thisFF8[295] == 3)
					{
						strcat(texPath, dinput);
					}
				}
				if (_thisFF8[294] == 2)
				{
					strcat(texPath, xinput);
				}
				if (_thisFF8[294] == 3)
				{
					strcat(texPath, dinput);
				}
			}
			break;
		case 31:
			strcat(texPath, "mc00.tex");
			break;
		case 32:
			strcat(texPath, "mc01.tex");
			break;
		case 33:
			strcat(texPath, "mc02.tex");
			break;
		case 34:
			strcat(texPath, "mc03.tex");
			break;
		case 35:
			strcat(texPath, "mc04.tex");
			break;
		case 36:
			strcat(texPath, "mc05.tex");
			if (_thisFF8[63] != 5)
				strcat(texPath, "_EN");
			break;
		case 37:
			strcat(texPath, "mc06.tex");
			break;
		case 38:
			strcat(texPath, "mc07.tex");
			break;
		case 39:
			strcat(texPath, "mc08.tex");
			break;
		case 40:
			strcat(texPath, "mc09.tex");
			break;
		case 41:
			strcat(texPath, "mag00.tex");
			break;
		case 42:
			strcat(texPath, "mag01.tex");
			break;
		case 43:
			strcat(texPath, "mag02.tex");
			break;
		case 44:
			strcat(texPath, "mag03.tex");
			break;
		case 45:
			strcat(texPath, "mag04.tex");
			break;
		case 46:
			strcat(texPath, "mag05.tex");
			break;
		case 47:
			strcat(texPath, "mag06.tex");
			break;
		case 48:
			strcat(texPath, "mag07.tex");
			break;
		case 49:
		case 50:
		case 56:
		case 57:
		case 59:
		case 60:
			strcat(texPath, "null");
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 58:
			strcat(texPath, "mag");
			sprintf(tex_paletteIndex, "%u", 0); //PLACEHOLDER- to reverse the v14 - 41;
			strcat(texPath, "_");
			strcat(texPath, langIdentifier);
			break;
		case 61:
			strcat(texPath, "magita.tex");
		}
		TexFuncGlSegment();
	}

	//============================                   [BEGIN]                           ==============================\\\\\\
	//============================      BATTLE, FIELD, 3D TEX REPLACE                  ==============================\\\\\\
	//============================                                                     ==============================\\\\\\

	if (tex_struct[48] > 0x19) //762
	{
		if (tex_struct[48] != 28)
		{
			if (tex_struct[48] == 35) //BATTLE - this is all about characters only
			{
				OutputDebugStringA("REQUESTED BATTLE tex_struct[48] == 35\n");
				TexFuncBattleSegment(_thisFF8, tex_struct[51], tex_struct[52]);
			}
			else if (tex_struct[48] == 57) //FIELD
			{
				v38 = tex_struct[50] + 4 * tex_struct[50] + 213;
				v40 = (BYTE*)_thisFF8 + 8 * v38; //
				v39 = *(DWORD*)v40;

				//tweak below for non-infinite texture upload to VRAM
				//if (v39
				//	&& (*(DWORD*)(v40 + 8) != (*(DWORD*)(tex_struct + 208) | (*(DWORD*)(tex_struct + 204) << 8))
				//		|| *(DWORD*)(v40 + 12) != (*(DWORD*)(tex_struct + 232) | (*(DWORD*)(tex_struct + 228) << 8))))
				//{
				//	//sub_1004BBF0((DWORD*)(_thisFF8 + 405), &height, (unsigned __int8*)(v39 + 8));
				//	//if (height != *(DWORD*)(_thisFF8 + 405))
				//	//	sub_11601560((void**)(_thisFF8 + 405), (int)&height, (void*)height);
				//	if (*(DWORD*)v40)
				//		(***(void(__stdcall****)(signed int))v40)(1);
				//	*(DWORD*)v40 = 0;
				//	*(BYTE*)(v40 + 4) = 0;
				//	*(BYTE*)(v40 + 28) = 0;
				//	*(BYTE*)(v40 + 36) = 0;
				//	if (*(BYTE*)(v40 + 30) == 0)
				//		*(DWORD*)(v40 + 24) = 0;
				//	if (!*(BYTE*)(v40 + 38))
				//		*(DWORD*)(v40 + 32) = 0;
				//}
				//if (*(DWORD*)(v40 + 24))
				//{
				//	*(DWORD*)(v40 + 24) = 0;
				//	*(BYTE*)(v40 + 30) = 0;
				//	*(BYTE*)(v40 + 4) = 0;
				//	*(BYTE*)(v40 + 28) = *(DWORD*)(v40 + 24) != 1;
				//}
				//if (*(DWORD*)(v40 + 32))
				//{
				//	*(DWORD*)(v40 + 32) = 0;
				//	*(BYTE*)(v40 + 38) = 0;
				//	*(BYTE*)(v40 + 4) = 0;
				//	*(BYTE*)(v40 + 36) = *(DWORD*)(v40 + 32) != 1;
				//}

				if (!*(DWORD*)v40)
				{
					lastrgbBuffer = NULL;

					strcat(texPath, "FIELD.FS\\field_hd");

					/*glTexSubImage2D(GL_TEXTURE_2D, 0, 0, row == 0 ? 384 : 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgbBuffer);
					SOIL_free_image_data(rgbBuffer);*/

					//I am creating the texture by height*2, because every texture has 384 but it has to fit two textures
					//therefore 384*2 = 768, but there are some cases when 384x384 tex happens, but it's still 768x768
					//so if we upscaled the tex by 2, then we would het 768*2, but width will always stay the same
					//no matter what even if we just read 384x384 on row 2

					if (tex_struct[51]) //row 1 texture
					{
						TexFuncCharaSegment(_thisFF8, 1, tex_struct[51], tex_struct[52] - 1);
						glGenTextures(1, &OPENGL_TEXTURES);
						glBindTexture(GL_TEXTURE_2D, OPENGL_TEXTURES);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lastHeight * 2, lastHeight * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas
						glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lastWidth, lastHeight, GL_RGBA, GL_UNSIGNED_BYTE, lastrgbBuffer);
						stbi_image_free(lastrgbBuffer);
					}
					if (tex_struct[57]) //row 0 texture
					{
						if (lastrgbBuffer == NULL)
						{
							glGenTextures(1, &OPENGL_TEXTURES);
							glBindTexture(GL_TEXTURE_2D, OPENGL_TEXTURES);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
							TexFuncCharaSegment(_thisFF8, 0, tex_struct[57], tex_struct[58] - 1);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lastHeight * 2, lastHeight * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas
							glTexSubImage2D(GL_TEXTURE_2D, 0, 0, lastHeight, lastWidth, lastHeight, GL_RGBA, GL_UNSIGNED_BYTE, lastrgbBuffer);
							stbi_image_free(lastrgbBuffer);
						}
						else
						{
							TexFuncCharaSegment(_thisFF8, 0, tex_struct[57], tex_struct[58] - 1);
							glTexSubImage2D(GL_TEXTURE_2D, 0, 0, lastHeight, lastWidth, lastHeight, GL_RGBA, GL_UNSIGNED_BYTE, lastrgbBuffer);
							stbi_image_free(lastrgbBuffer);
						}
					}
				}
			}
			else
			{
				OutputDebugStringA("REQUESTED CARDS"); //not sure about this one- it requests cards, but they tend to work without anything done here
				//CARDS
			}
		}
	}

	if (tex_struct[48] - 11 == 0) //BATTLE MONSTER
	{
		OutputDebugStringA("REQUESTED BATTLE FS UNKNOWN_ tex_Struct[48] - 11 == 0");
		TexFuncBattleMonsterSegment(_thisFF8, v4);
	}

	//============================                   [BEGIN]                           ==============================\\\\\\
	//============================       FIELD REPLACEMENT TEXTURE CODE                ==============================\\\\\\
	//============================                                                     ==============================\\\\\\


	if (tex_struct[48] == 25)
	{
		OutputDebugStringA("REQUESTED FIELD/GOVER BG replacement");
		//GOVER
	}


	//============================                   [BEGIN]                           ==============================\\\\\\
	//============================			OPENINGS, OVERTURE, SPLASH                 ==============================\\\\\\
	//============================                                                     ==============================\\\\\\

	if (tex_struct[48] - 13 == 0) //OPENINGS
	{
		if (tex_struct[51] < 0x41 || tex_struct[51] > 0x4E)
		{
			if (tex_struct[51] == 0x3F)
			{
				__asm
				{
					MOV EAX, OFFSET IMAGE_BASE
					MOV EAX, [EAX]
					ADD EAX, 0x1560AB0; //first: x15376B9 //tex_getFileOpening(int)

					MOV ECX, 0x3F
					CALL EAX
					MOV tex_getFileOpening, EAX
				}

				strcat(texPath, "squarelogo\\");
				strcat(texPath, tex_getFileOpening);
				strcat(texPath, "_");
				sprintf(tex_paletteIndex, "%u", tex_struct[50]);
				strcat(texPath, tex_paletteIndex);
				TexFuncGlSegment();
			}
			else
			{
				; //is this even used?
			}
		}
		else //else opening names
		{
			__asm
			{
				MOV EAX, OFFSET IMAGE_BASE
				MOV EAX, [EAX]
				ADD EAX, 0x1560AB0; //first: x15376B9 //tex_getFileOpening(int)

				MOV ECX, tex_struct
				ADD ECX, 0xCC
				MOV ECX, [ECX]
				CALL EAX
				MOV tex_getFileOpening, EAX
			}

			strcat(texPath, "opening\\");
			strcat(texPath, tex_getFileOpening);
			strcat(texPath, "_");
			sprintf(tex_paletteIndex, "%u", tex_struct[50]);
			strcat(texPath, tex_paletteIndex);
			TexFuncGlSegment();
		}
	}

	//============================                                                     ==============================\\\\\\
	//============================       OPENINGS, OVERTURE, SPLASH                    ==============================\\\\\\
	//============================                       [END]                         ==============================\\\\\\


	textureRepTable = calloc(52, sizeof(byte));
	InitTextureRepTable(textureRepTable,0, 1, height, width, height, width, OPENGL_TEXTURES);
	//__asm  //In second release this function was embedeed in glFunc- therefore we can't just call it ;-;
	//{
	//	PUSH 0
	//	PUSH 1
	//	PUSH height
	//	PUSH width
	//	PUSH height
	//	PUSH width
	//	PUSH OPENGL_TEXTURES
	//	MOV EAX, OFFSET IMAGE_BASE
	//	MOV EAX, [EAX]
	//	ADD EAX, 0x15BA402 //sets default to struct
	//	MOV ECX, [textureRepTable]
	//	CALL EAX
	//}

	__asm
	{
		MOV ECX, [textureRepTable]
		ADD ECX, 0x24
		MOV BYTE PTR[ECX], 1

		MOV ECX, [textureRepTable]
		ADD ECX, 0x28
		ADD ECX, 4
		PUSH[ECX]
		SUB ECX, 4

		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x160BE50 //first: 0x15BB62F //settextpointer -> vector<T>
		CALL EAX

		MOV EAX, [textureRepTable]
		ADD EAX, 0x28
		ADD DWORD PTR[EAX + 4], 0x24
	}
	//glTexture here
	*(DWORD*)langIdentifier = textureRepTable;
	if (v40)
		*(DWORD*)v40 = textureRepTable;

	//CREATEGLTEXTURE END

	//This one below crashes IDK why, same function, same parse and shit, looks like the v131 should point to 
	//some more data or it's some sort of struct idk
	//====================It's not that important anyway, game works fine without it
	//if (v4[16] == 1)
	//{
	//		v131 = parm1 + (v4[1] << 8);
	//	__asm
	//	{
	//		MOV ECX, _thisFF8
	//		ADD ECX, 0x684 // 417

	//		LEA EAX, v131
	//		PUSH EAX

	//		MOV EAX, OFFSET IMAGE_BASE
	//		MOV EAX, [EAX]
	//		ADD EAX, 0x15ADB50
	//		CALL EAX
	//		MOV ESI, dword ptr textureRepTable
	//		MOV [EAX], ESI
	//		POP ESI
	//	}
	//}


	__asm
	{
		MOV ESI, [textureRepTable]
		ADD ESI, 8

		MOV ECX, [_thisFF8] //THIS
		ADD ECX, 0x654		//THIS //first: 0x650

		PUSH ESI

		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x15FF0F0 //first: 0x15ADB50
		CALL EAX

		//we should now have EAX accepting pointer to our tex2D array which we do have at langIdentifier

		MOV ECX, dword ptr[langIdentifier]
		MOV ECX, [ECX]
		MOV[EAX], ECX //EAX should be pointer, ECX should be a pointer to TextureRGBA
		MOV ESI, [ESI] //ESI is textureRGBA+8; so it should be texIndex like 0x39
		MOV EAX, ESI

		//POP ESI //esi is probably tex_struct
		POP ECX
		POP EDI
		POP ESI
		POP EBX
	}

	//OPENGL create tex and load here

	//out- get back to end
	__asm
	{
		JMP tex_returnAddress;
	}

}

//replace LoadGameTexture first mnemonic to JMP and calculate the out value
void ReplaceTextureFunction()
{
	int textureFunction = IMAGE_BASE + TEXPATCH1;	//0x15A9920; [comment for no SEH/rewind]
	tex_returnAddress = IMAGE_BASE + TEXPATCH2;		//0x15AA3EB; [as above]
	modPage(textureFunction, 5);
	*(BYTE*)(textureFunction) = 0xE9;
	DWORD jmpparm = (DWORD)LoadGameTexture - textureFunction - 5;
	*(DWORD*)(textureFunction + 1) = jmpparm;

	//replace some strtesting- probably not the best way- we should find the place where the string is copied to buffer
	//this is clearly an error with filepath buffers- it tests for dinput and xinput in last opened files PNGs
	//more or less- currently I'm always returning 0 from the function as the string it tests is 0xBAADF00D unitialized HEAP
	//-it indeed may break some things (by filepath I think it might break the Xbox/PlayStation switching, but I didn't test it)
	int patchDinputXinput = IMAGE_BASE + TEXPATCH3;
	modPage(patchDinputXinput, 1);
	*(BYTE*)(patchDinputXinput) = 0xEB; //JMP relative 8bit


	int patchUnwantedTexStructFree = IMAGE_BASE + TEXPATCH4;
	modPage(patchUnwantedTexStructFree, 1);
	*(BYTE*)(patchUnwantedTexStructFree) = 0xEB; //JMP relative 8bit

	////replace EH_prologues and epilogues
	//textureFunction = IMAGE_BASE + 0x15BB79C;
	//modPage(textureFunction, 5);
	//*((DWORD*)textureFunction) = 0x90909090; //4 NOPs
	//*((BYTE*)(textureFunction+4)) = 0x90; //4 NOPs

	//textureFunction = IMAGE_BASE + 0x15BB7CB;
	//modPage(textureFunction, 5);
	//*((DWORD*)textureFunction) = 0x90909090; //4 NOPs
	//*((BYTE*)(textureFunction + 4)) = 0x90; //4 NOPs

}