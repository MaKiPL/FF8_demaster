#include <stdio.h>
#include <Windows.h>
#include "SOIL.h"
#include <gl/GL.h>

#define EXPORT __declspec(dllexport)
#pragma warning(disable:4996)

//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest()
{
	OutputDebugStringA("DEMASTER ENGINE LOADED!- we will try to repair what some company screwed up... ;-;\n");
	return;
}

static __int64 IMAGE_BASE;

BOOL modPage(DWORD address, int size)
{
	PDWORD lastProtect = 0;
	DWORD failure = VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &lastProtect);
	if (failure == 0)
	{
		DWORD myError = GetLastError();
		printf("err %08X", myError);
		return FALSE;
	}
	return TRUE;
}

#pragma region WM_UV_PATCH

BOOL uvPatchModMnemonic(DWORD address)
{
	if (!modPage(address, 4))
		return FALSE;
	*((BYTE*)address + 3) = 0;
	return TRUE;
}

unsigned int uvPatchAddresses[] =
{
	0x8A2CB2,
	0x8A2CB8,
	0x8A2D17,
	0x8A2D1D,
	0x8A2D6D,
	0x8A2D73,
	0x8A2DC5,
	0x8A2DCB,
	0x8A2E1B,
	0x8A2E21,
	0x8A2E6F,
	0x8A2E75
};

void ApplyUVPatch()
{
	BYTE* firstMnemonic = IMAGE_BASE + 0x8A2CB2;
	DWORD addMnemonic = *(DWORD*)firstMnemonic & 0xFFFFFF; //get mnemonic without parameter
	if (addMnemonic != 0x084680)
	{
		OutputDebugStringA("DEMASTER::ApplyUvPatch::The addresses are wrong! We found no ADD byte ptr[esi+8] at given rel call\n");
		return;
	}
	for (int i = 0; i < 12; i++)
		if (!uvPatchModMnemonic(IMAGE_BASE + uvPatchAddresses[i]))
			return;
}
#pragma endregion


#pragma region DirectIO

const char* DIRECT_IO_EXPORT_DIR = "EXP\\";
DWORD DIRECT_IO_EXPORT_DIR_LEN;

DWORD IO_backAddress = 0;
DWORD IO_backAddress2 = 0;
DWORD IO_backAddress3 = 0;
DWORD filePathBuffer, filePathStrlen;
char IO_backlogFilePath[256];

__declspec(naked) void directIO_fopenReroute()
{
	__asm
	{
		//let's save our regs, because VS compiler will probably destroy them
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX

		MOV filePathBuffer, EDX //save buffer
		MOV filePathStrlen, ECX //and strlen
	}
	
	strcpy(IO_backlogFilePath, DIRECT_IO_EXPORT_DIR); //VS automatically does the ESP math
	strcpy(IO_backlogFilePath + DIRECT_IO_EXPORT_DIR_LEN, filePathBuffer); //same for this, no local vars so no ESP--
	OutputDebugStringA(IO_backlogFilePath);
	OutputDebugStringA("\n");


	__asm
	{
		//restore our regs
		POP EDX
		POP ECX
		POP EBX
		POP EAX
		//original code here
		PUSH ECX
		PUSH EDX
		LEA ECX, [ebp - 2Ch]
		JMP IO_backAddress
	}
}

const char rb[] = "rb";

__declspec(naked) void directIO_fopenReroute2()
{
	__asm
	{
		PUSH OFFSET rb //too much hustle to get original value based on calcs without using any regs
		PUSH OFFSET IO_backlogFilePath
		JMP IO_backAddress2
	}
}
int loc00 = 0;
FILE* fd;
__declspec(naked) void directIO_fopenReroute3()
{
	__asm
	{
		MOV EAX, ESI
		MOV ECX, [EBP - 0x0C]
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX
	}
	fd = fopen(IO_backlogFilePath, "rb");
	fseek(fd, 0, 2); //back
	loc00 = ftell(fd);
	fclose(fd);

	__asm
	{
		POP EDX
		POP ECX
		POP EBX
		POP EAX

		PUSH EBX
		MOV EBX, [loc00]
		ADD EAX, 0x10
		MOV [EAX], EBX
		SUB EAX, 0x10
		POP EBX

		JMP IO_backAddress3
	}
}


void ApplyDirectIO()
{
	//to not f-up things let's first see if the exp dir exists
	DIRECT_IO_EXPORT_DIR_LEN = strlen(DIRECT_IO_EXPORT_DIR);
	DWORD attrib = GetFileAttributesA(DIRECT_IO_EXPORT_DIR);
	if (attrib == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebugStringA("There is no export directory, so it looks like you didn't export the files from zzz files. Not applying patch\n");
		return;
	}


		//Patch fopen method
	//patch JMP to directIO_fopenReroute
	BYTE* fopenPatchMnemonic = IMAGE_BASE + 0x15D41EB;
	IO_backAddress = fopenPatchMnemonic + 5;
	DWORD jmpParam = (DWORD)directIO_fopenReroute - (DWORD)fopenPatchMnemonic - 5;
	modPage(fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;

	//patch JMP to directIO_fopenReroute2
	fopenPatchMnemonic = IMAGE_BASE + 0x15D4797;
	IO_backAddress2 = fopenPatchMnemonic + 5;
	jmpParam = (DWORD)directIO_fopenReroute2 - (DWORD)fopenPatchMnemonic - 5;
	modPage(fopenPatchMnemonic, 5);
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
	*(fopenPatchMnemonic + 5) = 0x90; //NOP

		//patch FSEEKs
	//First fseek
	fopenPatchMnemonic = IMAGE_BASE + 0x15D47D8;
	modPage(fopenPatchMnemonic, 3); //PUSH + NOP
	*fopenPatchMnemonic = 0x6A;		//PUSH
	*(fopenPatchMnemonic+1) = 0x00;	//	PUSH->0
	*(fopenPatchMnemonic+2) = 0x90;	//NOP

	//Second fseek
	fopenPatchMnemonic = IMAGE_BASE + 0x15D4947;
	modPage(fopenPatchMnemonic, 3); //XOR EAX, EAX + NOP
	*fopenPatchMnemonic = 0x31;		//XOR EAX
	*(fopenPatchMnemonic + 1) = 0xc0;	//	XOR EAX->EAX
	*(fopenPatchMnemonic + 2) = 0x90;	//NOP

	//Now we need to fix fd struct filelen for modifications of RAW files
	//see fopen_archivePrepareSeek- we need to update struct before return
	//EAX contains struct for FD open archive
	fopenPatchMnemonic = IMAGE_BASE + 0x15D42B7; //MOV EAX, ESI; MOV ECX, [EBP-0ch]   [//8BC6 8B4DF4]
	IO_backAddress3 = fopenPatchMnemonic + 5;
	modPage(fopenPatchMnemonic, 5); //JMP
	jmpParam = (DWORD)directIO_fopenReroute3 - (DWORD)fopenPatchMnemonic - 5;
	*fopenPatchMnemonic = 0xE9; //JMP [DW]
	*(DWORD*)(fopenPatchMnemonic + 1) = jmpParam;
}

#pragma endregion

#pragma region TextureUpscaleMod

const int battleTextureUpscale = 0x300; //768 [600=2048]

void ApplyTextureUpscaleMod()
{
	int mnemonicPatchPlace = IMAGE_BASE + 0x15AABAB;
	modPage(mnemonicPatchPlace, 5);
	BYTE* b = mnemonicPatchPlace;
	*b = 0xB9; //MOV ECX
	*(DWORD*)(b + 1) = battleTextureUpscale;
}



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
char texPath[256];
char* langIdentifier = "EN";
char* dinput = "_dinput";
char* xinput = "_xinput";
int textureIndex = -1;
int v147;
DWORD attr;

DWORD* tex_struct; //arg+4
DWORD parm1; //arg+8

DWORD OPENGL_HANDLE;
DWORD OPENGL_METHOD;
UINT OPENGL_TEXTURES;

byte* texture_buffer_open;
unsigned char* rgbBuffer;
void* textureRepTable;
UINT v27;
char* tex_getFileOpening;

DWORD* v4;
int v131;

__declspec(naked) void LoadGameTexture()
{
	__asm
	{
		MOV _thisFF8, ECX //get this:: //FF8 should be always the same for session
		ADD ESP, 4
		POP EAX
		MOV tex_struct, EAX //this is naked, so EBP shit I'll be doing manually here
		POP EAX
		MOV parm1, EAX
		XOR EAX, EAX
		SUB ESP, 0x0C //now get back to ESP which would point to CALLE+5 (RET)

		PUSH ESI
	}
	*(DWORD*)tex_paletteIndex = parm1;
	width = (UINT)(tex_struct + 47);
	v4 = tex_struct + 47;
	if (tex_struct[63] == 1)
	{
		height = parm1 + (tex_struct[48] << 8);
		__asm
		{
			MOV ECX, _thisFF8 //this
			ADD ECX, 0x684
			//MOV ECX, [ECX]
			LEA EAX, height
			PUSH EAX
			LEA EAX, langIdentifier
			PUSH EAX
			MOV EAX, OFFSET IMAGE_BASE
			MOV EAX, [EAX]
			ADD EAX, 0x15AE068
			CALL EAX

		}
	}
	currentLanguage = *(_thisFF8 + 0x3F);

	texMode = tex_struct[48];
	v147 = 0;

	memset(texPath, 0, 256); //clear path
	strcat(texPath, DIRECT_IO_EXPORT_DIR);
	strcat(texPath, "textures\\");
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
			sprintf(tex_paletteIndex, "%u", *(USHORT*)(IMAGE_BASE + 0x1768678));
			strcat(texPath, tex_paletteIndex);
			strcat(texPath, "_evn_");
			sprintf(tex_paletteIndex, "%u", parm1 >> 1);
			strcat(texPath, tex_paletteIndex);
			break;
		case 23:
			strcat(texPath, "FIELD.FS\\jp_add_font_hd\\");
			sprintf(tex_paletteIndex, "%u", *(USHORT*)(IMAGE_BASE + 0x1768678));
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
	}


	//BELOW IS TEST SEGMENT - it means it's basically only conditions to display shit I want fastest way

	if (tex_struct[48] > 0x19) //762
	{
		if (tex_struct[48] != 28)
		{
			if (tex_struct[48] == 35)
			{

			}
		}
	}
	if (tex_struct[48] == 25)
	{
		//GOVER
	}

	if (tex_struct[48] - 13 == 0)
	{
		if (tex_struct[51] < 0x41 || tex_struct[51] > 0x4E)
		{
			if (tex_struct[51] == 0x3F)
			{
				__asm
				{
					MOV EAX, OFFSET IMAGE_BASE
					MOV EAX, [EAX]
					ADD EAX, 0x15376B9 //tex_getFileOpening(int)

					MOV ECX, 0x3F
					CALL EAX
					MOV tex_getFileOpening, EAX
				}

				strcat(texPath, "squarelogo\\");
				strcat(texPath, tex_getFileOpening);
				strcat(texPath, "_");
				sprintf(tex_paletteIndex, "%u", tex_struct[50]);
				strcat(texPath, tex_paletteIndex);
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
				ADD EAX, 0x15376B9 //tex_getFileOpening(int)

				MOV ECX, tex_struct[48] - 13
				CALL EAX
				MOV tex_getFileOpening, EAX
			}

			strcat(texPath, "opening\\");
			strcat(texPath, tex_getFileOpening);
			strcat(texPath, "_");
			sprintf(tex_paletteIndex, "%u", tex_struct[50]);
			strcat(texPath, tex_paletteIndex);
		}
	}

	//END OF TEST SEGMENT
	

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
	rgbBuffer = SOIL_load_image(texPath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbBuffer);
	SOIL_free_image_data(rgbBuffer);

	textureRepTable = calloc(52, sizeof(byte));
	__asm
	{
		PUSH 0
		PUSH 1
		PUSH height
		PUSH width
		PUSH height
		PUSH width
		PUSH OPENGL_TEXTURES
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x15BA402 //sets default to struct
		MOV ECX, [textureRepTable]
		CALL EAX
	}

	__asm
	{
		MOV ECX, [textureRepTable]
		ADD ECX, 0x24
		MOV BYTE PTR [ECX], 1

		MOV ECX, [textureRepTable]
		ADD ECX, 0x28
		ADD ECX, 4
		PUSH [ECX]
		SUB ECX, 4
		
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x15BB62F //settextpointer -> vector<T>
		CALL EAX

		MOV EAX, [textureRepTable]
		ADD EAX, 0x28
		ADD DWORD PTR [EAX+4], 0x24
	}
	//glTexture here
	*(DWORD*)langIdentifier = textureRepTable;

	//CREATEGLTEXTURE END

	//This one below crashes IDK why, same function, same parse and shit, looks like the v131 should point to some more data or it's some sort of struct idk
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

			MOV ECX, [_thisFF8]
			ADD ECX, 0x650

			PUSH ESI

			MOV EAX, OFFSET IMAGE_BASE
			MOV EAX, [EAX]
			ADD EAX, 0x15ADB50
			CALL EAX

			MOV ECX, dword ptr[langIdentifier]
			MOV[EAX], ECX
			MOV ESI, [ESI]
			MOV EAX, ESI

			POP ESI
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
	int textureFunction = IMAGE_BASE + 0x15A9920;
	tex_returnAddress = IMAGE_BASE + 0x15AA3EB;
	modPage(textureFunction, 5);
	*(BYTE*)(textureFunction) = 0xE9;
	DWORD jmpparm = (DWORD)LoadGameTexture - textureFunction - 5;
	*(DWORD*)(textureFunction + 1) = jmpparm;

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
#pragma endregion



BOOL WINAPI DllMain(

	HINSTANCE hinstDLL, // handle to DLL module

	DWORD fdwReason, // reason for calling function

	LPVOID lpReserved) // reserved

{
	if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
		return 0;

	IMAGE_BASE = GetModuleHandleA("FFVIII_EFIGS");
	OPENGL_HANDLE = GetModuleHandleA("OPENGL32");
	

	InitTest();


	//LET'S GET THE HACKING DONE
	ApplyUVPatch();
	ApplyDirectIO();
	ApplyTextureUpscaleMod();
	ReplaceTextureFunction();


	//HACKING DONE, WE CAN GTFO
	return 1; //all success
}

