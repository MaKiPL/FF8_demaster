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
	{"", 20, NULL, -1,-1,-1,FALSE},
	{"", 21, NULL, -1,-1,-1,FALSE}, //TRAIN TRACKS
	{"", 22, NULL, -1,-1,-1,FALSE}, //CLOUDS
	{"", 23, NULL, -1,-1,-1,FALSE} //MAP
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
	int getTexIndex = GetTextureIndex();
	if (getTexIndex < 20 && (tPage > 14 && tPage < 26))
	{
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
	}
	else
	{
		sprintf(localn, "%stextures\\world\\dat\\wmset\\wmset_%03d_0.png", DIRECT_IO_EXPORT_DIR, getTexIndex);
		int wmStructPointer = -1;
		switch (getTexIndex)
		{
		case 14: //clouds
			wmStructPointer = 22;
			break;
		case 24:
			wmStructPointer = 23;
			break;
		case 29: //train tracks
			wmStructPointer = 21;
			break;
		default:
			wmStructPointer = 21;
			break;
		}
		int width_, height_, channels;
		if (!ws[wmStructPointer].bActive)
		{
		unsigned char* buffer = stbi_load(localn, &width_, &height_, &channels, 0);
		sprintf(localn, "\tstbi::w: %d; h: %d; channels: %d\n", width_, height_, channels);
		OutputDebugStringA(localn);
		ws[wmStructPointer].buffer = buffer;
		ws[wmStructPointer].channels = channels;
		ws[wmStructPointer].height = height_;
		ws[wmStructPointer].width = width_;
		ws[wmStructPointer].bActive = true;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ws[wmStructPointer].width, ws[wmStructPointer].height, 0, ws[wmStructPointer].channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ws[wmStructPointer].buffer);
	}
	return;
}

int GetTextureIndex()
{
	void* pixels = (void*)pixelsPtr;
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	int textureType = gl_textures[48];
	DWORD texA = *(DWORD*)(IMAGE_BASE + 0x17424B0) + 1;
	DWORD texB = *(DWORD*)(IMAGE_BASE + 0x17424B4) + 1;
	DWORD texC = *(DWORD*)(IMAGE_BASE + 0x17424B8) + 1;
	DWORD texD = *(DWORD*)(IMAGE_BASE + 0x17424BC) + 1;
	if (textureType == 18)
	{
		switch (tPage)
		{
		case 13: //moon texture + fx
			return -1;
		case 14: //clouds
			return 14;
		case 15: //vehicles + character
		case 28: //water textures
			return -1;
		case 29: //tracks, bridge, vehicle
			return 29;
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
	}
	else if (textureType == 0) //map
	{
		return -1;
		//return 24;
	}
	return -1;
}

DWORD LOCALDEB = 0;

DWORD _wtpCheck()
{
	if (!WORLD_TEXTURES)
		return 0;

	int textureType = gl_textures[48];
	if (textureType != 18 && textureType != 0) //we want only world textures
		return 0;

	DWORD tPage = gl_textures[50];

	char localn[256];
	int textureIndex = GetTextureIndex();
	if (textureIndex < 20 && (tPage > 14 && tPage < 26))
		sprintf(localn, "%stextures\\world\\dat\\texl\\texl_%03d_0.png", DIRECT_IO_EXPORT_DIR, textureIndex);
	else
		sprintf(localn, "%stextures\\world\\dat\\wmset\\wmset_%03d_0.png", DIRECT_IO_EXPORT_DIR, textureIndex);

	DWORD attr = GetFileAttributesA(localn);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		sprintf(localn, "_wtpCheck FAILED ON TEXTURE!; Expected: a0stg%03d_%d.png", currentStage, tPage);
		return 0;
	}
	return 1;
}

void ApplyWorldPatch()
{
}