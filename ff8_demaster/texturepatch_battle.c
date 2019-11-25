#include "coreHeader.h"
#include <gl/GL.h>

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
	sprintf(n, "%stextures\\battle.fs\\hd_new\\d%xc%03u_", DIRECT_IO_EXPORT_DIR, (aIndex - 4097) / 100, (aIndex - 4097) % 100);
	sprintf(nc, "%stextures\\battle.fs\\hd_new\\d%xw%03u_0.png", DIRECT_IO_EXPORT_DIR, (bIndex - 5197) / 100, (bIndex - 5197) % 100);
	strcpy(nb, n);
	strcat(n, "0.png");
	strcat(nb, "1.png");



	int width, height, channels;
	unsigned char* buffer = stbi_load(n, &width, &height, &channels, 0); //chara 0

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width * 2, height * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //sets the atlas

	int width2, height2, width3, height3;
	unsigned char* buffertwo = stbi_load(nb, &width2, &height2, &channels, 0); //chara 0
	unsigned char* bufferthree = stbi_load(nc, &width3, &height3, &channels, 0); //chara 0


	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, height, width2, height2, GL_RGBA, GL_UNSIGNED_BYTE, buffertwo);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, height + height2, width3, height3, GL_RGBA, GL_UNSIGNED_BYTE, bufferthree);

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