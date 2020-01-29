#include "coreHeader.h"
#include <gl/GL.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

DWORD _wtpBackAdd1;
DWORD _wtpBackAdd2;
DWORD _wtpBackAdd3;


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
	{"", 0, -1, -1,-1,-1,FALSE},
	{"", 1, -1, -1,-1,-1,FALSE},
	{"", 2, -1, -1,-1,-1,FALSE},
	{"", 3, -1, -1,-1,-1,FALSE},
	{"", 4, -1, -1,-1,-1,FALSE},
	{"", 5, -1, -1,-1,-1,FALSE},
	{"", 6, -1, -1,-1,-1,FALSE},
	{"", 7, -1, -1,-1,-1,FALSE},
	{"", 8, -1, -1,-1,-1,FALSE},
	{"", 9, -1, -1,-1,-1,FALSE},
	{"", 10, -1, -1,-1,-1,FALSE},
	{"", 11, -1, -1,-1,-1,FALSE},
	{"", 12, -1, -1,-1,-1,FALSE},
	{"", 13, -1, -1,-1,-1,FALSE},
	{"", 14, -1, -1,-1,-1,FALSE},
	{"", 15, -1, -1,-1,-1,FALSE},
	{"", 16, -1, -1,-1,-1,FALSE},
	{"", 17, -1, -1,-1,-1,FALSE},
	{"", 18, -1, -1,-1,-1,FALSE},
	{"", 19, -1, -1,-1,-1,FALSE},
	{"", 20, -1, -1,-1,-1,FALSE}
};

//you can't just create new folder because >WEEP< - too lazy to find the cause

void _wtpGl()
{
	DWORD tPage = gl_textures[50];
	int palette = tex_header[52];
	char localn[256];

	DWORD unk = *(DWORD*)(IMAGE_BASE + 0x17424B4);
	int texIndex = unk;//GetTextureIndex();
	sprintf(localn, "_wtpGl()::localEAX: %d, Tpage: %d, Palette: %d, TexIndex: %d\n", *(DWORD*)(IMAGE_BASE + 0x1780f88), tPage, palette, texIndex);
	OutputDebugStringA(localn);
	sprintf(localn, "%stextures\\world\\dat\\texl\\texl_%03d_0.png", DIRECT_IO_EXPORT_DIR, /*tPage*/texIndex);

	if (ws[texIndex].bActive == FALSE || ws[texIndex].buffer == -1)
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ws[unk].width, ws[unk].height, 0, ws[texIndex].channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ws[texIndex].buffer);
	return;
}

int GetTextureIndex()
{
	switch (gl_textures[36])
	{
	case 0x46B8570:
		return 1;
	case 0x46CB850:
		return 2;
	case 0x28D6E70:
	case 0x46A4D95:
		return 3;
	case 0x469EC18:
	case 0x46C9878:
		return 4;
	default:
		return 1;
	}
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
	//if (textureSizeComparer == 1024)
	//	return 0;
		
	int palette = gl_textures[39]; //tpage is 50
	int unk36 = gl_textures[36];
	int unk38 = gl_textures[38];
	int unk40 = gl_textures[40];
	int unk46 = gl_textures[46];
	int unk67 = gl_textures[67];
	int unk68 = gl_textures[68];

	//if (GetTextureIndex() == -1)
	//{
		//char debugLog[2048];
		//debugLog[0] = '\0';
		//sprintf(debugLog, "%d,%d,%d,%d,%08X,%08X,%08X,%08X,%08X,%08X,%d,%d\n", LOCALDEB, textureType, testVar,
		//	palette, unk36, unk38, unk40, unk46, unk67, unk68, gl_textures[70], gl_textures[72]);
		//FILE* fd = fopen("D:\\DBG\\DBG.csv", "a+");
		//fwrite(debugLog, sizeof(char), strlen(debugLog), fd);
		//fclose(fd);

		//sprintf(debugLog, "D:\\DBG\\DBG_%d.png", LOCALDEB++);
		//stbi_write_png(debugLog, 256, 256, 4, pixelsPtr, 256 * 4);
	//}

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

void ApplyWorldPatch()
{
	//_wtpBackAdd1 = InjectJMP(IMAGE_BASE + 0x1591847, (DWORD)_wtp, 6); //this is to force bHdAvailable on wm tex
	//_wtpBackAdd2 = InjectJMP(IMAGE_BASE + 0x16065F4, (DWORD)_wtpReplace, 5); //this is to pre-select PNG if available
	//_wtpBackAdd3 = InjectJMP(IMAGE_BASE + 0x155F504, (DWORD)_wtpGetEax, 6); //I don't know which texId to load due to the way wm is constructed
}