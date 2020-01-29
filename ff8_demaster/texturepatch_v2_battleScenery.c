#include "coreHeader.h"
#include <gl/GL.h>

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
	char localPath[256];
	DWORD tpage;
	unsigned char* buffer;
	int width;
	int height;
	int channels;
	BOOL bActive;
};

struct battleSceneryStructure bss[] =
{
	{"", 16, -1, -1,-1,-1,FALSE},
	{"", 17, -1, -1,-1,-1,FALSE},
	{"", 18, -1, -1,-1,-1,FALSE},
	{"", 19, -1, -1,-1,-1,FALSE},
	{"", 20, -1, -1,-1,-1,FALSE},
	{"", 21, -1, -1,-1,-1,FALSE},
	{"", 22, -1, -1,-1,-1,FALSE}
};

void bssInvalidateTexPath(DWORD tPage);

void _bspGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	char localn[256];

	sprintf(localn, "_bspGl()::Stage: %d, Tpage: %d, Palette: %d\n", currentStage, tPage, palette);
	OutputDebug(localn);

	sprintf(localn, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d.png", DIRECT_IO_EXPORT_DIR, currentStage, tPage);
	int width_, height_, channels;
	unsigned char* buffer;
	if (bss[tPage - 16].buffer == -1) //texture never loaded
	{
		buffer = stbi_load(localn, &width_, &height_, &channels, 0);
		bss[tPage - 16].bActive = TRUE;
		bss[tPage - 16].width = width_;
		bss[tPage - 16].height = height_;
		bss[tPage - 16].channels = channels;
		bssInvalidateTexPath(tPage);
		strcpy(bss[tPage - 16].localPath, localn);
		bss[tPage - 16].buffer = buffer;
		bss[tPage - 16].tpage = tPage;
		sprintf(localn, "\tbspGl():bss - First time init battle texture for page: %d", tPage);
		OutputDebug(localn);
		sprintf(localn, "\tstbi::w: %d; h: %d; channels: %d\n", width_, height_, channels);
		OutputDebug(localn);
	}
	else
	{
		if (strcmp(bss[tPage - 16].localPath, localn)) //there is new texture- the filepath are not the same
		{
			//first step is to invalidate last buffer
			stbi_image_free(bss[tPage - 16].buffer); //let's release memory from unused texture
			bssInvalidateTexPath(tPage);
			strcpy(bss[tPage - 16].localPath, localn);
			buffer = stbi_load(localn, &width_, &height_, &channels, 0);
			bss[tPage - 16].width = width_;
			bss[tPage - 16].height = height_;
			bss[tPage - 16].channels = channels;
			bss[tPage - 16].buffer = buffer;
			sprintf(localn, "\tbspGl():bss - replacing cached texture with new one for tPage: %d", tPage);
			OutputDebug(localn);
			sprintf(localn, "\tstbi::w: %d; h: %d; channels: %d\n", width_, height_, channels);
			OutputDebug(localn);
		}
		else
		{
			 //uncomment for old behaviour laggy
			//stbi_image_free(bss[tPage - 16].buffer); //let's release memory from unused texture
			//bssInvalidateTexPath(tPage);
			//strcpy(bss[tPage - 16].localPath, localn);
			//buffer = stbi_load(localn, &width_, &height_, &channels, 0);
			//bss[tPage - 16].width = width_;
			//bss[tPage - 16].height = height_;
			//bss[tPage - 16].channels = channels;
			//bss[tPage - 16].buffer = buffer;
			//sprintf(localn, "\tbspGl():bss - what happens during force-reload?: %d", tPage);
			//OutputDebug(localn);
			//sprintf(localn, "\tstbi::w: %d; h: %d; channels: %d\n", width_, height_, channels);
			//OutputDebug(localn);
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bss[tPage - 16].width, bss[tPage - 16].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bss[tPage - 16].buffer);
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
	if (tPage > 28)
		return 0;
	char localn[256];
	sprintf(localn, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d.png", DIRECT_IO_EXPORT_DIR, currentStage, tPage);
	DWORD attr = GetFileAttributesA(localn);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		sprintf(localn, "_bspCheck FAILED ON TEXTURE!; Expected: a0stg%03d_%d.png\n", currentStage, tPage);
		OutputDebug(localn);
		return 0;
	}
	sprintf(localn, "_bspCheck: Happy at a0stg%03d_%d.png\n", currentStage, tPage);
	return 1;
}

DWORD** ds_free;
DWORD** ds_teximg;

//this function hooks the pre-upload to VRAM data. Add new cases below for other modules
__declspec(naked) void _bsp()
{
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
			PUSH DWORD PTR[EBP+0x10]
			CALL DWORD PTR DS:0x1166B2A8
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
	ds_free = IMAGE_BASE + 0x166b2a8;
	ds_teximg = IMAGE_BASE + 0x166b4a0;
	char localn[256];
	sprintf(localn, "ApplyBattleFieldPatch():ds_free is at: %08X and ds_teximg is at: %08X", ds_free, ds_teximg);
	OutputDebug(localn);
	_bspBackAdd1 = InjectJMP(IMAGE_BASE + 0x1573AFF, (DWORD)_bsp, 38);
	_bspBackAdd2 = InjectJMP(IMAGE_BASE + 0x1573B54, (DWORD)_bspFree, 9);

	//this disables textureLimit for resolution
	InjectJMP(IMAGE_BASE + 0x156CED4, (DWORD)(IMAGE_BASE + 0x156D30B), 6);
}