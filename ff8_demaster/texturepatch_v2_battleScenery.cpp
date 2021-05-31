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
	bool bActive;
	int nextFrame;
};

struct battleSceneryStructure bss[] =
{
	{"", 16, NULL, -1,-1,-1,FALSE,0},
	{"", 17, NULL, -1,-1,-1,FALSE,0},
	{"", 18, NULL, -1,-1,-1,FALSE,0},
	{"", 19, NULL, -1,-1,-1,FALSE,0},
	{"", 20, NULL, -1,-1,-1,FALSE,0},
	{"", 21, NULL, -1,-1,-1,FALSE,0},
	{"", 22, NULL, -1,-1,-1,FALSE,0}
};

void bssInvalidateTexPath(DWORD tPage);
void LoadImageIntoBattleSceneryStruct(const size_t index, const DWORD tPage, const char* const localn)
{
	int palette = tex_header[52];
	//spamming so i put it here so it only logs when loading.
	OutputDebug("_bspGl()::Stage: %d, Tpage: %d, Palette: %d\n", currentStage, tPage, palette);
	bimg::ImageContainer* img = LoadImageFromFile(localn);
	bss[index].bActive = true;
	bss[index].width = img->m_width;
	bss[index].height = img->m_width;
	bss[index].channels = img->m_hasAlpha ? 4 : 3;
	bssInvalidateTexPath(tPage);
	strcpy(bss[index].localPath, localn);
	bss[index].buffer = img;
	bss[index].tpage = tPage;
	OutputDebug("\tbspGl():bss - First time init battle texture for page: %d", tPage);
	OutputDebug("\tstbi::w: %d; h: %d; channels: %d\n", bss[index].width, bss[index].height, bss[index].channels);
}
void _bspGl()
{
	DWORD tPage = gl_textures[50];
	char localn[256]{ 0 };
	if(DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage, bss[tPage - 16].nextFrame++))
		if(bss[tPage - 16].nextFrame != 1 && !DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage, 0))
		{
			//nextFrame == 1 means there is no 0 frame so we can skip the heavy DDSorPNG;
			bss[tPage - 16].nextFrame = 1; //current nextFrame didn't exist so we went back to 0 and change nextFrame to 1.
		}
		else
			DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage);
	if (bss[tPage - 16].buffer == NULL)
	{
		LoadImageIntoBattleSceneryStruct(tPage - 16, tPage, localn);		
	}
	else
	{
		if (strcmp(bss[tPage - 16].localPath, localn) != 0)
		{
			bimg::imageFree(bss[tPage - 16].buffer); //let's release memory from unused texture
			LoadImageIntoBattleSceneryStruct(tPage - 16, tPage, localn);
		}
		
	}
	RenderTexture(bss[tPage - 16].buffer);
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
	if (DDSorPNG(localn, 256, "%stextures\\battle.fs\\hd_new\\a0stg%03d_%d", DIRECT_IO_EXPORT_DIR, currentStage, tPage))
	{
		OutputDebug("%s FAILED ON TEXTURE!; Expected: a0stg%03d_%d.(dds|png)\n", __func__ ,currentStage, tPage);
		return 0;
	}
	OutputDebug("%s: Happy at a0stg%03d_%d.(dds|png)\n", __func__, currentStage, tPage);
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