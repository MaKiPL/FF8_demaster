#include "coreHeader.h"
#include <gl/GL.h>
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

DWORD _wtpBackAdd1;
DWORD _wtpBackAdd2;
DWORD _wtpBackAdd3;
DWORD _wtpBackAdd4;


struct worldTextureStructure
{
	char localPath[256];
	DWORD tpage;
	unsigned char* buffer;
	int width;
	int height;
	int channels;
	BOOL bActive;
};

struct worldTextureStructure ws[] =
{
	{"", 0, NULL, -1,-1,-1,FALSE},
	{"", 1, NULL, -1,-1,-1,FALSE},
	{"", 2, NULL, -1,-1,-1,FALSE},
	{"", 3, NULL, -1,-1,-1,FALSE},
	{"", 4, NULL, -1,-1,-1,FALSE},
	{"", 5, NULL, -1,-1,-1,FALSE},
	{"", 6, NULL, -1,-1,-1,FALSE},
	{"", 7, NULL, -1,-1,-1,FALSE},
	{"", 8, NULL, -1,-1,-1,FALSE},
	{"", 9, NULL, -1,-1,-1,FALSE},
	{"", 10, NULL, -1,-1,-1,FALSE},
	{"", 11, NULL, -1,-1,-1,FALSE},
	{"", 12, NULL, -1,-1,-1,FALSE},
	{"", 13, NULL, -1,-1,-1,FALSE},
	{"", 14, NULL, -1,-1,-1,FALSE},
	{"", 15, NULL, -1,-1,-1,FALSE},
	{"", 16, NULL, -1,-1,-1,FALSE},
	{"", 17, NULL, -1,-1,-1,FALSE},
	{"", 18, NULL, -1,-1,-1,FALSE},
	{"", 19, NULL, -1,-1,-1,FALSE},
	{"", 20, NULL, -1,-1,-1,FALSE}
};

int GetTextureIndex();

//you can't just create new folder because >WEEP< - too lazy to find the cause

DWORD lastKnownTextureId;

void _wtpGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	char localn[256];

	DWORD unk = *(DWORD*)(IMAGE_BASE + 0x17424B4);
	int texIndex = lastKnownTextureId;
	sprintf(localn, "_wtpGl()::localEAX: %d, Tpage: %d, Palette: %d, TexIndex: %d\n", *(DWORD*)(IMAGE_BASE + 0x1780f88), tPage, palette, texIndex);
	OutputDebugStringA(localn);
	sprintf(localn, "%stextures\\world\\dat\\texl\\texl_%03d_0.png", DIRECT_IO_EXPORT_DIR, /*tPage*/texIndex);

	if (ws[texIndex].bActive == FALSE || ws[texIndex].buffer == NULL)
	{
		int width_, height_, channels;
		unsigned char* buffer = stbi_load(localn, &width_, &height_, &channels, 0);
		ws[texIndex].width = width_; ws[texIndex].height = height_; ws[texIndex].channels = channels;
		strcpy(ws[texIndex].localPath, localn);
		ws[texIndex].buffer = buffer;
		ws[texIndex].bActive = TRUE;
		sprintf(localn, "\tstbi::w: %d; h: %d; channels: %d\n", width_, height_, channels);
		OutputDebugStringA(localn);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ws[texIndex].width, ws[texIndex].height, 0, ws[texIndex].channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ws[texIndex].buffer);
	return;
}

int GetTextureIndex()
{
	void* pixels = (void*)pixelsPtr;
	DWORD tPage = gl_textures[50];
	DWORD texA = *(DWORD*)(IMAGE_BASE + 0x17424B0) + 1;
	DWORD texB = *(DWORD*)(IMAGE_BASE + 0x17424B4) + 1;
	DWORD texC = *(DWORD*)(IMAGE_BASE + 0x17424B8) + 1;
	DWORD texD = *(DWORD*)(IMAGE_BASE + 0x17424BC) + 1;
	switch (tPage)
	{
	case 20:
		lastKnownTextureId = texA;
		return texA;
	case 22:
		lastKnownTextureId = texB;
		return texB;
	case 24:
		lastKnownTextureId = texC;
		return texC;
	case 26:
		lastKnownTextureId = texD;
		return texD;
	}
	return -1;
}

DWORD LOCALDEB = 0;

DWORD _wtpCheck()
{
	if (!WORLD_TEXTURES)
		return 0;

	int textureType = gl_textures[48];
	if (textureType != 18) //we want only world textures
		return 0;

	DWORD tPage = gl_textures[50];
	if (tPage < 16)
		return 0;
	if (tPage > 26)
		return 0;

	DWORD testVar = *(DWORD*)(IMAGE_BASE + 0x1780f88);
	int textureSizeComparer = gl_textures[66];
	if (textureSizeComparer == 1024)
		return 0;
		
	int palette = gl_textures[39]; //tpage is 50
	int pixelPtr_ = pixelsPtr;
	int unk36 = gl_textures[36];
	int unk38 = gl_textures[38];
	int unk40 = gl_textures[40];
	int unk46 = gl_textures[46];
	int unk67 = gl_textures[67];
	int unk68 = gl_textures[68];

	char localn[256];
	sprintf(localn, "%stextures\\world\\dat\\texl\\texl_%03d_0.png", DIRECT_IO_EXPORT_DIR, GetTextureIndex());

	DWORD attr = GetFileAttributesA(localn);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		sprintf(localn, "_wtpCheck FAILED ON TEXTURE!; Expected: a0stg%03d_%d.png", currentStage, tPage);
		return 0;
	}
	return 1;
}

DWORD _wtp00_SizeConfirm;
DWORD _wtp00_OffsetTexl;

void _wtp00()
{
	char localn[256];
	sprintf(localn, "WorldModule::GetTexl.obj- Offset: %08X\tSize: %08X\t ID: %d\n", _wtp00_OffsetTexl, _wtp00_SizeConfirm, _wtp00_OffsetTexl/0x12800);
	OutputDebug(localn);
}

__declspec(naked) void _wtp()
{
	__asm
	{
		MOV EAX, [ESP+8]
		MOV _wtp00_SizeConfirm, EAX
		MOV EAX, [ESP+4]
		MOV _wtp00_OffsetTexl, EAX
		CALL _wtp00
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x155F3F0
		CALL EAX
		JMP _wtpBackAdd1
	}
}

__declspec(naked) void _wtp01()
{
	__asm
	{
		MOV EAX, [ESP + 8]
		MOV _wtp00_SizeConfirm, EAX
		MOV EAX, [ESP + 4]
		MOV _wtp00_OffsetTexl, EAX
		CMP _wtp00_SizeConfirm, 0x12800
		JNE _notTexl
		CALL _wtp00
		_notTexl:
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x155F3F0
		CALL EAX
		JMP _wtpBackAdd2
	}
}

__declspec(naked) void _wtp02()
{
	__asm
	{
		PUSH ECX
		PUSH EDX
		PUSH ESI
		CALL _wtp03
		POP ESI
		POP EDX
		POP ECX
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x16CB5A0
		MOV EAX, [EAX]
		JMP _wtpBackAdd3
	}
}

void ApplyWorldPatch()
{
	_wtpBackAdd1 = (DWORD)InjectJMP(IMAGE_BASE + 0x09107BD, (DWORD)_wtp, 5); //
	_wtpBackAdd2 = (DWORD)InjectJMP(IMAGE_BASE + 0x09101F1, (DWORD)_wtp01, 5); //
	_wtpBackAdd3 = (DWORD)InjectJMP(IMAGE_BASE + 0x09108C4, (DWORD)_wtp02, 5); //imgui
}