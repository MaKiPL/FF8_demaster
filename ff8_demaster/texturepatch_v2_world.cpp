#include "coreHeader.h"

DWORD _wtpBackAdd1;
DWORD _wtpBackAdd2;
DWORD _wtpBackAdd3;
DWORD _wtpBackAdd4;


struct worldTextureStructure
{
	char localPath[256]{ 0 };
	DWORD tpage;
	bimg::ImageContainer* buffer;
	int width;
	int height;
	int channels;
	bool bActive;
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
void LoadImageIntoStruct(size_t texIndex, const char *const localn)
{
	if (ws[texIndex].bActive == FALSE || ws[texIndex].buffer == NULL)
	{
		bimg::ImageContainer* img = LoadImageFromFile(localn);
		ws[texIndex].width = img->m_width;
		ws[texIndex].height = img->m_height;
		ws[texIndex].channels = img->m_hasAlpha ? 4 : 3;
		ws[texIndex].buffer = img;
		ws[texIndex].bActive = true;
		strcpy(ws[texIndex].localPath, localn);
		OutputDebug("\tstbi::w: %d; h: %d; channels: %d\n", ws[texIndex].width, ws[texIndex].height, ws[texIndex].channels);
	}
}
void _wtpGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	char localn[256]{ 0 };

	DWORD unk = *(DWORD*)(IMAGE_BASE + 0x17424B4);
	size_t texIndex = lastKnownTextureId;
	if(!(tPage == 29 && (palette == 0 || palette==2))) // prevents log from filling up.
		OutputDebug("_wtpGl()::localEAX: %d, Tpage: %d, Palette: %d, TexIndex: %d\n", *(DWORD*)(IMAGE_BASE + 0x1780f88), tPage, palette, texIndex);

	int getTexIndex = GetTextureIndex();
	if (getTexIndex < 20 && (tPage > 14 && tPage < 26))
	{
		DDSorPNG(localn, 256,"%stextures\\world\\dat\\texl\\texl_%03d_0", DIRECT_IO_EXPORT_DIR, /*tPage*/texIndex);
		LoadImageIntoStruct(texIndex, localn);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		
		RenderTexture(ws[texIndex].buffer);
	}
	else
	{
		DDSorPNG(localn, 256, "%stextures\\world\\dat\\wmset\\wmset_%03d_0", DIRECT_IO_EXPORT_DIR, getTexIndex);
		int wmStructPointer = 21;
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
		}
		LoadImageIntoStruct(wmStructPointer, localn);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		RenderTexture(ws[wmStructPointer].buffer);
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
			return -1;
		case 28: //water textures
			return 28;
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

	char localn[256]{ 0 };
	int textureIndex = GetTextureIndex();
	if (textureIndex < 20 && (tPage > 14 && tPage < 26))
	{
		DDSorPNG(localn, 256, "%stextures\\world\\dat\\texl\\texl_%03d_0", DIRECT_IO_EXPORT_DIR, textureIndex);
	}
	else
	{
		DDSorPNG(localn, 256,"%stextures\\world\\dat\\wmset\\wmset_%03d_0", DIRECT_IO_EXPORT_DIR, textureIndex);
	}
	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("%s: %s, Failed to load!", __func__, localn);
		return 0;
	}
	
	return 1;
}

void ApplyWorldPatch()
{
}