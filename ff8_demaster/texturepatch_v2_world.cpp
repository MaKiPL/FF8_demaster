#include "coreHeader.h"
#define CRASHLOG OutputDebug("%s::Line %d\n", __func__, __LINE__)
DWORD _asm_WorldRetAddr1;
DWORD _asm_WorldRetAddr2;
DWORD _asm_WorldRetAddr3;
DWORD _asm_WorldRetAddr4;


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
	{"", 23, NULL, -1,-1,-1,FALSE}, //MAP
	{"", 24, NULL, -1,-1,-1,FALSE}, //moon texture + fx
	{"", 25, NULL, -1,-1,-1,FALSE}, //WATER
	{"", 26, NULL, -1,-1,-1,FALSE}, //15
	{"", 27, NULL, -1,-1,-1,FALSE}, //16
};

int GetTextureIndex();

//you can't just create new folder because >WEEP< - too lazy to find the cause

DWORD lastKnownTextureId{0};
void LoadImageIntoWorldStruct(size_t texIndex, const char* const localn, int tPage, int palette)
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

		OutputDebug("\t%s::localEAX: %d, Tpage: %d, Palette: %d, TexIndex: %d\n", __func__, *(DWORD*)(IMAGE_BASE + 0x1780f88), tPage, palette, texIndex);
		OutputDebug("\t%s::w: %d; h: %d; channels: %d\n", __func__, ws[texIndex].width, ws[texIndex].height, ws[texIndex].channels);
	}
	else if (strcmp(ws[texIndex].localPath, localn) != 0)
	{
		OutputDebug("%s::WARNIING texture:\n\t%s\n\t!=\n\t%s\n", __func__, localn, ws[texIndex].localPath);
	}
}
void _wtpGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	char localn[256]{ 0 };

	DWORD unk = *(DWORD*)(IMAGE_BASE + 0x17424B4);
	size_t texIndex = lastKnownTextureId;

	int getTexIndex = GetTextureIndex();
	if (getTexIndex <= 0) return;

	if (texIndex != getTexIndex
		/*getTexIndex == 13 ||
		getTexIndex == 14 ||
		getTexIndex == 15 ||
		getTexIndex == 16 ||
		getTexIndex == 24 ||
		getTexIndex == 28 ||
		getTexIndex == 29 ||
		texIndex == 0*/
		)
	{

		if (DDSorPNG(localn, 256, "%stextures\\world\\dat\\wmset\\wmset_%03d_0", DIRECT_IO_EXPORT_DIR, getTexIndex))
		{
			OutputDebug("%s::%d::Failed to Load: %s\n\tgetTexIndex: %d\n", __func__, __LINE__, localn, getTexIndex);
			return;
		}

		int wmStructPointer = 21;
		switch (getTexIndex)
		{
		case 13: //moon texture + fx
			wmStructPointer = 24;
			break;
		case 14: //clouds
			wmStructPointer = 22;
			break;
		case 15: //
			wmStructPointer = 26;
			break;
		case 16: //
			wmStructPointer = 27;
			break;
		case 24:
			wmStructPointer = 23;
			break;
		case 28: // water
			wmStructPointer = 25;
			break;
		case 29: //train tracks
			wmStructPointer = 21;
			break;
		}

		LoadImageIntoWorldStruct(wmStructPointer, localn, tPage, palette);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		RenderTexture(ws[wmStructPointer].buffer);
	}
	else  //if (getTexIndex < 20 && (tPage > 14 && tPage < 26) )
	{

		if (DDSorPNG(localn, 256, "%stextures\\world\\dat\\texl\\texl_%03d_0", DIRECT_IO_EXPORT_DIR, /*tPage*/texIndex))
		{
			OutputDebug("%s::%d::Failed to Load: %s\n\tgetTexIndex: %d, tPage: %d, texIndex: %d", __func__, __LINE__, localn, getTexIndex, tPage, texIndex);
			return;
		}

		LoadImageIntoWorldStruct(texIndex, localn, tPage, palette);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		RenderTexture(ws[texIndex].buffer);
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
		lastKnownTextureId = 0;
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
		case 13: //moon texture + fx
		case 14: //clouds
		case 28: //water textures
		case 29: //tracks, bridge, vehicle
		//case 16: //??? I see it trying to load this and don't see anything using it.
		//case 15: //vehicles + character
				 //this one is special the game mixes different textures into one and they overlap.
			// character texture is top left. some cars are top left.
			return tPage;
		}
		//OutputDebug("%s::tPage: %d\n", __func__, tPage);
	}
	else if (textureType == 196)
	{
		//tPage 13, 15, 28
	}
	else if (textureType == 201)
	{
		//tPage 13, 15, 28
	}
	else if (textureType == 0) //map
	{
		//OutputDebug("%s::%d::tPage: %d, textureType: %d, palette: %d\n", __func__, __LINE__, tPage, textureType, palette);
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

	if (textureIndex <= 0) //disabled textures are set to -1;
		return 0;
	if (lastKnownTextureId == textureIndex)//(textureIndex < 20 && (tPage > 14 && tPage < 26))
	{
		DDSorPNG(localn, 256, "%stextures\\world\\dat\\texl\\texl_%03d_0", DIRECT_IO_EXPORT_DIR, textureIndex);
	}
	else
	{
		DDSorPNG(localn, 256, "%stextures\\world\\dat\\wmset\\wmset_%03d_0", DIRECT_IO_EXPORT_DIR, textureIndex);
	}

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		//OutputDebug("%s::Failed to Load: %s\n", __func__, localn); //spamming message!
		return 0;
	}
	return 1;
}

void ApplyWorldPatch()
{
}
#undef CRASHLOG