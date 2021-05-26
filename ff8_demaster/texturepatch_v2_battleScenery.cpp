#include "coreHeader.h"

DWORD _bspBackAdd1;
DWORD _bspBackAdd2;



DWORD bAlreadyFreed;

unsigned char* buffer16;
unsigned char* buffer17;
unsigned char* buffer18;
unsigned char* buffer19;
unsigned char* buffer20;
unsigned char* buffer21;
unsigned char* buffer22;
DWORD lastStage;

struct battleSceneryStructure
{
	char localPath[256]{ 0 };
	DWORD tpage;
	bimg::ImageContainer* buffer;
	int width;
	int height;
	int channels;
	BOOL bActive;
};

struct battleSceneryStructure bss[] =
{
	{"", 16, NULL, -1,-1,-1,FALSE},
	{"", 17, NULL, -1,-1,-1,FALSE},
	{"", 18, NULL, -1,-1,-1,FALSE},
	{"", 19, NULL, -1,-1,-1,FALSE},
	{"", 20, NULL, -1,-1,-1,FALSE},
	{"", 21, NULL, -1,-1,-1,FALSE},
	{"", 22, NULL, -1,-1,-1,FALSE}
};

void bssInvalidateTexPath(DWORD tPage);

void _bspGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	char localn[256]{ 0 };

	OutputDebug("_bspGl()::Stage: %d, Tpage: %d, Palette: %d\n", currentStage, tPage, palette);

	DDSorPNG(localn,256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage);

	if (bss[tPage - 16].buffer == NULL) //texture never loaded
	{
		bimg::ImageContainer* img = LoadImageFromFile(localn);
		bss[tPage - 16].bActive = TRUE;
		bss[tPage - 16].width = img->m_width;
		bss[tPage - 16].height = img->m_width;
		bss[tPage - 16].channels = img->m_hasAlpha ? 4 : 3;
		bssInvalidateTexPath(tPage);
		strcpy(bss[tPage - 16].localPath, localn);
		bss[tPage - 16].buffer = img;
		bss[tPage - 16].tpage = tPage;
		OutputDebug("\tbspGl():bss - First time init battle texture for page: %d", tPage);
		OutputDebug("\tstbi::w: %d; h: %d; channels: %d\n", bss[tPage - 16].width, bss[tPage - 16].height, bss[tPage - 16].channels);
	}
	else
	{
		if (strcmp(bss[tPage - 16].localPath, localn)) //there is new texture- the filepath are not the same
		{
			//first step is to invalidate last buffer
			bimg::imageFree(bss[tPage - 16].buffer); //let's release memory from unused texture
			bssInvalidateTexPath(tPage);
			strcpy(bss[tPage - 16].localPath, localn);
			bimg::ImageContainer* img = LoadImageFromFile(localn);
			bss[tPage - 16].width = img->m_width;
			bss[tPage - 16].height = img->m_height;
			bss[tPage - 16].channels = img->m_hasAlpha ? 4 : 3;
			bss[tPage - 16].buffer = img;
			OutputDebug("\tbspGl():bss - replacing cached texture with new one for tPage: %d", tPage);
			OutputDebug("\tstbi::w: %d; h: %d; channels: %d\n", bss[tPage - 16].width, bss[tPage - 16].height, bss[tPage - 16].channels);
		}
		else
		{
			 //uncomment for old behaviour laggy
			//bimg::imageFree(bss[tPage - 16].buffer); //let's release memory from unused texture
			//bssInvalidateTexPath(tPage);
			//strcpy(bss[tPage - 16].localPath, localn);
			//bimg::ImageContainer* img = LoadImageFromFile(localn);
			//bss[tPage - 16].width = img->m_width;
			//bss[tPage - 16].height = img->m_height;
			//bss[tPage - 16].channels = img->m_hasAlpha ? 4 : 3;
			//bss[tPage - 16].buffer = img;
			//OutputDebug("\tbspGl():bss - what happens during force-reload?: %d", tPage);
			//OutputDebug("\tstbi::w: %d; h: %d; channels: %d\n", bss[tPage - 16].width, bss[tPage - 16].height, bss[tPage - 16].channels);
		}
	}
	RenderTexture(bss[tPage - 16].buffer);
	//we no longer free the buffer here
	return;
}

void bssInvalidateTexPath(DWORD tPage)
{
	for (int i = 0; i < 256; i++) //reinventing wheel with strset/memset
		bss[tPage - 16].localPath[i] = 0x00;
}

DWORD _bspCheck()
{
	int textureType = gl_textures[48];
	if (textureType != 11) //we want only battle textures
		return 0;
	if (currentStage == -1)
		return 0;
	DWORD tPage = gl_textures[50];
	if (tPage < 16)
		return 0;
	if (tPage > 21)
		return 0;
	char localn[256]{ 0 };
	DDSorPNG(localn,256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage);
	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("_bspCheck FAILED ON TEXTURE!; Expected: a0stg%03d_%d.(dds|png)\n", currentStage, tPage);
		return 0;
	}
	sprintf(localn, "_bspCheck: Happy at a0stg%03d_%d.(dds|png)\n", currentStage, tPage);
	return 1;
}

DWORD** ds_free;
DWORD** ds_teximg;

//this function hooks the pre-upload to VRAM data. Add new cases below for other modules
GLint MINFILTER, MAGFILTER;
__declspec(naked) void _bsp()
{
	//glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &MINFILTER);
	//glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &MAGFILTER);
	__asm
	{
		MOV DWORD PTR[ebp + 0x8], EAX
		MOV texturesPtr, EAX
		MOV EAX, [EBP + 0x10]
		MOV pixelsPtr, EAX
		MOV bAlreadyFreed, 0

		CALL _bspCheck
		TEST EAX, EAX
		JNZ _bspOk

		CALL _wtpCheck //WORLD MODULE
		TEST EAX, EAX
		JNZ _wtpOk



	_original:
		PUSH DWORD PTR[EBP + 0x10]
			PUSH 0x1401
			PUSH 0x80E1
			PUSH 0
			PUSH ESI
			PUSH EDI
			PUSH 0x8058
			PUSH 0
			PUSH 0xDE1
			MOV EAX, ds_teximg
			MOV EAX, [EAX]
			CALL EAX
			_out :
		JMP _bspBackAdd1

			_wtpOk:
			MOV EAX, OFFSET IMAGE_BASE
			MOV EAX, [EAX]
			ADD EAX, 0x166B2A8
			PUSH DWORD PTR[EBP+0x10]
			CALL [EAX]
			//CALL DWORD PTR DS:0x1166B2A8
			MOV bAlreadyFreed, 1
			CALL _wtpGl
			JMP _out

			JMP _original

			_bspOk :
		PUSH DWORD PTR[EBP + 0x10]
			MOV EAX, ds_free
			MOV EAX, [EAX]
			CALL EAX
			MOV bAlreadyFreed, 1

			CALL _bspGl
			JMP _out

	}
}

__declspec(naked) void _bspFree()
{
	__asm
	{
		CMP bAlreadyFreed, 1
		JE _out
		push   DWORD PTR[ebp + 0x10]
		MOV EAX, ds_free
		MOV EAX, [EAX]
		CALL EAX
		_out :
		JMP _bspBackAdd2
	}

}

void ApplyBattleFieldPatch()
{
	OutputDebug("Applying battle field patch\n");
	ds_free = (DWORD**)(IMAGE_BASE + 0x166b2a8);
	ds_teximg = (DWORD**)(IMAGE_BASE + 0x166b4a0);
	OutputDebug("ApplyBattleFieldPatch():ds_free is at: %08X and ds_teximg is at: %08X\n", ds_free, ds_teximg);
	_bspBackAdd1 = (DWORD)InjectJMP(IMAGE_BASE + 0x1573AFF, (DWORD)_bsp, 38);
	_bspBackAdd2 = (DWORD)InjectJMP(IMAGE_BASE + 0x1573B54, (DWORD)_bspFree, 9);

	//this disables textureLimit for resolution
	InjectJMP(IMAGE_BASE + 0x156CED4, (DWORD)(IMAGE_BASE + 0x156D30B), 6);
}