#include "coreHeader.h"
#include <gl/GL.h>

DWORD _bspBackAdd1;
DWORD _bspBackAdd2;

DWORD pixelsPtr;
DWORD texturesPtr;

DWORD bAlreadyFreed;

void _bspGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	char localn[256];

	sprintf(localn, "_bspGl()::Stage: %d, Tpage: %d, Palette: %d\n", currentStage, tPage, palette);
	OutputDebug(localn);

	sprintf(localn, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d.png", DIRECT_IO_EXPORT_DIR, currentStage, tPage);
	int width_, height_, channels;
	unsigned char* buffer = stbi_load(localn, &width_, &height_, &channels, 0); //chara 0
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	sprintf(localn, "\tstbi::w: %d; h: %d; channels: %d\n", width_, height_, channels);
	OutputDebug(localn);
	stbi_image_free(buffer);
	return;
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

		//CALL _wtpCheck
		//TEST EAX, EAX
		//JNZ _wtpOk



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
		_out:
		JMP _bspBackAdd1

		//	_wtpOk:
		//PUSH DWORD PTR[EBP+0x10]
		//CALL DWORD PTR DS:0x1166B2A8
		//MOV bAlreadyFreed, 1
		//CALL _wtpGl
		//JMP _out

		JMP _original

		_bspOk:
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
		_out:
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
}