#include <stdio.h>
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "SOIL.h" - deprecated/ no PNG interlace support
#include <gl/GL.h>

/*
KURSE ALL SEEDS!
*/

#define EXPORT __declspec(dllexport)
#pragma warning(disable:4996)

//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest()
{
	OutputDebugStringA("DEMASTER ENGINE LOADED!- we will try to repair the lazy remaster... ;-;\n");
	return;
}

static __int64 IMAGE_BASE;
DWORD attr;

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


//CREATES PATH
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

	attr = GetFileAttributesA(IO_backlogFilePath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		sprintf(IO_backlogFilePath, "%stextures\\null.png", DIRECT_IO_EXPORT_DIR);


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

//PUSHES THE PATH TO READER
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

//SETS FILESIZE BASED ON REAL HDD ENTRY INSTEAD OF VTABLE OF WEEP ARCHIVE
__declspec(naked) void directIO_fopenReroute3()
{
	__asm
	{
		//JNE $+0xC //I can't do JE IO_backAddress3, probably because VS treats it's a DWORD PTR
		MOV EAX, ESI
		MOV ECX, [EBP - 0x0C]
		TEST ESI, ESI //
		JNZ validEsi
		JMP IO_backAddress3 
		validEsi:
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX
	}
	attr = GetFileAttributesA(IO_backlogFilePath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		loc00 = -1;
	else
	{
		fd = fopen(IO_backlogFilePath, "rb");
		fseek(fd, 0, 2); //back
		loc00 = ftell(fd);
		fclose(fd);
	}

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

//const int battleTextureUpscale = 0x300; //768 [600=2048]
//
//void ApplyTextureUpscaleMod()
//{
//	int mnemonicPatchPlace = IMAGE_BASE + 0x15AABAB;
//	modPage(mnemonicPatchPlace, 5);
//	BYTE* b = mnemonicPatchPlace;
//	*b = 0xB9; //MOV ECX
//	*(DWORD*)(b + 1) = battleTextureUpscale;
//}



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

DWORD* tex_struct; //arg+4
DWORD parm1; //arg+8

DWORD OPENGL_HANDLE;
DWORD OPENGL_METHOD;
UINT OPENGL_TEXTURES;

byte* texture_buffer_open;
unsigned char* rgbBuffer;
void* textureRepTable;
char* tex_getFileOpening;

DWORD* v4;


///
	///no table param! it's created after, no need it
	///filepath is texPath static
///

int lastWidth;
int lastHeight;
UCHAR* lastrgbBuffer;

struct TexFuncMonsterStruct
{
	char* filename; //DWORD PTR
	int pad; //in asm it's align 8
	UINT fileIndex; //this is that weird shit that don't conform the battlefilesarray from vanilla
	int unk; //initialized with -1
	int pad2; //not pad, but maybe unused?
	USHORT wUnk;  //in mag i.e. 832- is VRAM?
	USHORT wUnk2; //see above, now it's 256 so VRAM y?
	USHORT wUnk3; //i.e. 64- width?
	USHORT wUnk4; //see above, but probably height?
	UINT uUnk;
};

//This is stupid design approach- for the process of development of monster replacement 
//I'm just copying their structures, but yeah- this is bad, hardcoding stuff is bad
//awful, just awful
struct TexFuncMonsterStruct TexFuncMonsterTexProvider[] = { 
	{"mag900", 0, 0x41F, -1, 0,0,0,0,0}, 
	{"mag089", 0, 0x420, -1, 0,0,0,0,0}, 
	{"mag200", 0, 0x425, -1, 0,0,0,0,0}, 
	{NULL}/*CONTINUE AT 1175B950*/ };

//bad design struct array


BYTE TexFuncBattleMonsterSegment(int this_, DWORD* v4)
{
	int v7 = v4[6]; //this should be our texId [yeah]
	char* test = TexFuncMonsterTexProvider[0].filename; //test
	//monsterTexs[0].filename;
}

BYTE TexFuncCharaSegment(int this__, int row, int aIndex, int bIndex)
{
	//THIS IS NON-NAKED FUNCTION, SO WE CAN UTILIZE NEW LOCALS
	char tempPath[256];
	char tempSprint[256];
	sprintf(texPath, "%stextures\\field.fs\\field_hd", DIRECT_IO_EXPORT_DIR);
	strcpy(tempPath, texPath); //exp\\tex\\fieldfs\\fielhd


	if (aIndex - 97 > 0xF9F)
	{
		//bla bla not field model texture bla bla
	}
	if (aIndex >= 0xC19)
		sprintf(tempSprint, "\\%s%03u_%u", "p", aIndex - 3097, bIndex);
	else if (aIndex < 0x831)
	{
		if(aIndex < 0x449)
			sprintf(tempSprint, "\\%s%03u_%u", "d", aIndex - 97, bIndex);
		else		
			sprintf(tempSprint, "\\%s%03u_%u", "n", aIndex - 1097, bIndex);
	}
	else
		sprintf(tempSprint, "\\%s%03u_%u", "o", aIndex - 2097, bIndex);


	char testPath[256];
	sprintf(testPath, "%s%s.png", tempPath,tempSprint);
	attr = GetFileAttributesA(testPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		sprintf(testPath, "%s_new%s.png",tempPath, tempSprint);
	attr = GetFileAttributesA(testPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
		sprintf(testPath, "%s_new\\d000_0.png",tempPath); //ERROR !!!!

	strcpy(texPath, testPath); //establish path


	//we now need to create the atlas- it's normally one tex on top and one below
	//but of course we can easily tweak it because I ported whole texture function
	//- vanilla dev (that company starting at D) made every size hardcoded
	//like create gl tex with 768x768 or subtex at 384
	int width, height, channels;
	char n[256];
	lastrgbBuffer = stbi_load(texPath, &width, &height, &channels, 4);
	sprintf(n, "TEX::CHARAFIELD- %s %dx%d\n", texPath, width, height);
	OutputDebugStringA(n);
	lastWidth = width;
	lastHeight = height;

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
	sprintf(n, "%stextures\\battle.fs\\hd_new\\d%xc%03u_",DIRECT_IO_EXPORT_DIR, (aIndex - 4097) / 100, (aIndex - 4097) % 100);
	sprintf(nc, "%stextures\\battle.fs\\hd_new\\d%xw%03u_0.png",DIRECT_IO_EXPORT_DIR, (bIndex - 5197) / 100, (bIndex - 5197) % 100);
	strcpy(nb, n);
	strcat(n, "0.png");
	strcat(nb, "1.png");



	int width, height, channels;
	unsigned char* buffer = stbi_load(n, &width, &height, &channels, 0); //chara 0

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width*2, height*2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas

	int width2, height2, width3, height3;
	unsigned char * buffertwo = stbi_load(nb, &width2, &height2, &channels, 0); //chara 0
	unsigned char * bufferthree = stbi_load(nc, &width3, &height3, &channels, 0); //chara 0


	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, height, width2, height2, GL_RGBA, GL_UNSIGNED_BYTE, buffertwo);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, height+height2, width3, height3, GL_RGBA, GL_UNSIGNED_BYTE, bufferthree);

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

int channels;

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
	//__asm
	//{
	//	MOV _thisFF8, ECX //get this:: //FF8 should be always the same for session
	//	ADD ESP, 4
	//	POP EAX
	//	MOV tex_struct, EAX //this is naked, so EBP shit I'll be doing manually here
	//	POP EAX
	//	MOV parm1, EAX
	//	XOR EAX, EAX
	//	SUB ESP, 0x0C //now get back to ESP which would point to CALLE+5 (RET)

	//	PUSH ESI
	//}

	//it works now under C++ SEH
	__asm 
	{
		MOV _thisFF8, ECX
		MOV EBX, [EBP+8]
		MOV tex_struct, EBX
		MOV EBX, [EBP+0xC]
		MOV parm1, EBX
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
					TexFuncCharaSegment(_thisFF8, 1, tex_struct[51], tex_struct[52]-1);
					glGenTextures(1, &OPENGL_TEXTURES);
					glBindTexture(GL_TEXTURE_2D, OPENGL_TEXTURES);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lastHeight*2,lastHeight*2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas
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
						TexFuncCharaSegment(_thisFF8, 0, tex_struct[57], tex_struct[58]-1);
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lastHeight*2,lastHeight*2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas
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
		TexFuncBattleMonsterSegment(_thisFF8,v4);
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
				ADD EAX, 0x15376B9 //tex_getFileOpening(int)

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
			ADD ECX, 0x650		//THIS

			PUSH ESI

			MOV EAX, OFFSET IMAGE_BASE
			MOV EAX, [EAX]
			ADD EAX, 0x15ADB50
			CALL EAX

			//we should now have EAX accepting pointer to our tex2D array which we do have at langIdentifier

			MOV ECX, dword ptr[langIdentifier]
			MOV ECX, [ECX]
			MOV[EAX], ECX //EAX should be pointer, ECX should be a pointer to TextureRGBA
			MOV ESI, [ESI] //ESI is textureRGBA+8; so it should be texIndex like 0x39
			MOV EAX, ESI

			//POP ESI //esi is probably tex_struct
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
	int textureFunction = IMAGE_BASE + 0x15A992F;	//0x15A9920; [comment for no SEH/rewind]
	tex_returnAddress = IMAGE_BASE + 0x15AA3E6;		//0x15AA3EB; [as above]
	modPage(textureFunction, 5);
	*(BYTE*)(textureFunction) = 0xE9;
	DWORD jmpparm = (DWORD)LoadGameTexture - textureFunction - 5;
	*(DWORD*)(textureFunction + 1) = jmpparm;

	//replace some strtesting- probably not the best way- we should find the place where the string is copied to buffer
	//this is clearly an error with filepath buffers- it tests for dinput and xinput in last opened files PNGs
	//more or less- currently I'm always returning 0 from the function as the string it tests is 0xBAADF00D unitialized HEAP
	//-it indeed may break some things (by filepath I think it might break the Xbox/PlayStation switching, but I didn't test it)
	int patchDinputXinput = IMAGE_BASE + 0x15A9842;
	modPage(patchDinputXinput, 1);
	*(BYTE*)(patchDinputXinput) = 0xEB; //JMP relative 8bit


	int patchUnwantedTexStructFree = IMAGE_BASE + 0x15AC4A4;
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
	ReplaceTextureFunction();


													//ApplyTextureUpscaleMod();
	//HACKING DONE, WE CAN GTFO
	return 1; //all success
}

