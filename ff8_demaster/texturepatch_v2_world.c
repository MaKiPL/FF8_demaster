#include "coreHeader.h"
#include <gl/GL.h>

DWORD _wtpBackAdd1;
DWORD _wtpBackAdd2;
DWORD _wtpBackAdd3;


DWORD _wtpSaveTpage = -1;
DWORD texlSaveESI = -1;
DWORD texlSaveEAX = -1;

//you can't just create new folder because >WEEP< - too lazy to find the cause
char* _wtpReplaceVoid();

BOOL bWtpHdAvailable(int tPage)
{
	_wtpSaveTpage = tPage; //wtp tpage doesn't matter- however I found texl getting via pointer so...
	//texlSaveESI = *(DWORD*)(IMAGE_BASE + 0x17424B0);
	char localn[256];
	sprintf(localn, "%stextures\\", DIRECT_IO_EXPORT_DIR);
	char* wtprep = _wtpReplaceVoid();
	strcat(localn, wtprep);
	strcat(localn, ".png");
	DWORD attr = GetFileAttributesA(localn);
	if (attr == INVALID_FILE_ATTRIBUTES)
		return FALSE;
	else return TRUE;
}

void wtpVoid()
{
	char localn[256];
	sprintf(localn, "_wtp::Load world module: %d at TPage: %d ESI at: %d\n", *(DWORD*)(IMAGE_BASE + 0x178207C), *(DWORD*)(IMAGE_BASE + 0x1782084), *(DWORD*)(IMAGE_BASE + 0x17424B0));
	OutputDebugStringA(localn);

	int currentTpage = *(DWORD*)(IMAGE_BASE + 0x1782084);
	if (currentTpage < 16 || currentTpage>27)
		return;
	if (!bWtpHdAvailable(currentTpage))
		return;
	*(DWORD*)(IMAGE_BASE + 0x1782080) = 1; //HDavailable DWORD 0-vanilla tex 1- use texture_handler by given tex type
	return;
}


char* _wtpReplaceVoid()
{
	char localn[256];
	sprintf(localn, "field.fs\\field_hd_new\\hp_texpg_%d", texlSaveESI);
	return localn;
}


DWORD _wtpSaveEax = 0;

__declspec(naked) void _wtpReplace()
{
	__asm 
	{
		MOV _wtpSaveEax, EAX
		CALL _wtpReplaceVoid
		PUSH EAX

		MOV EAX, _wtpSaveEax
		JMP _wtpBackAdd2
	}
}

__declspec(naked) void _wtp()
{
	__asm
	{
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x1782084 //currentTpage dword
		MOV [EAX], EDI

		CMP EDX, 0x12
		JE _worldHack
		//CMP ...
		//JE  ...

		_out:
		JMP _wtpBackAdd1

			_worldHack:
		PUSH EAX
			PUSH EBX
			PUSH ECX
			PUSH EDX
		CALL wtpVoid
			POP EDX
			POP ECX
			POP EBX
			POP EAX
		JMP _out
	}
}

__declspec(naked) void _wtpGetEax()
{
	__asm
	{
		MOV texlSaveEAX, EAX
		//orig
		MOV ecx, DWORD PTR ds : 0x11780f88
		MOV texlSaveESI, ECX
		JMP _wtpBackAdd3
	}
}

//void _wtpGl()
//{
//	DWORD tPage = gl_textures[50];
//	int palette = tex_header[52];
//	char localn[256];
//
//	sprintf(localn, "_wtpGl()::localEAX: %d, Tpage: %d, Palette: %d\n", *(DWORD*)(IMAGE_BASE + 0x1780f88), tPage, palette);
//	OutputDebugStringA(localn);
//
//	sprintf(localn, "%stextures\\field.fs\\field_hd_new\\hp_texpg_%d.png", DIRECT_IO_EXPORT_DIR, *(DWORD*)(IMAGE_BASE + 0x1780f88));
//	int width_, height_, channels;
//	unsigned char* buffer = stbi_load(localn, &width_, &height_, &channels, 0); //chara 0
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
//	sprintf(localn, "\tstbi::w: %d; h: %d; channels: %d\n", width_, height_, channels);
//	OutputDebugStringA(localn);
//	stbi_image_free(buffer);
//	return;
//}
//
//DWORD _wtpCheck()
//{
//	if (!WORLD_TEXTURES)
//		return 0;
//
//	int textureType = gl_textures[48];
//	if (textureType != 18) //we want only battle textures
//		return 0;
//
//	DWORD tPage = gl_textures[50];
//	if (tPage < 16)
//		return 0;
//	if (tPage > 26)
//		return 0;
//
//	DWORD testVar = *(DWORD*)(IMAGE_BASE + 0x1780f88);
//
//	char localn[256];
//
//	sprintf(localn, "%stextures\\field.fs\\field_hd_new\\hp_texpg_%d.png", DIRECT_IO_EXPORT_DIR, testVar);
//	DWORD attr = GetFileAttributesA(localn);
//	if (attr == INVALID_FILE_ATTRIBUTES)
//	{
//		sprintf(localn, "_wtpCheck FAILED ON TEXTURE!; Expected: a0stg%03d_%d.png", currentStage, tPage);
//		return 0;
//	}
//	return 1;
//}

void ApplyWorldPatch()
{
	_wtpBackAdd1 = InjectJMP(IMAGE_BASE + 0x1591847, (DWORD)_wtp, 6); //this is to force bHdAvailable on wm tex
	_wtpBackAdd2 = InjectJMP(IMAGE_BASE + 0x16065F4, (DWORD)_wtpReplace, 5); //this is to pre-select PNG if available
	_wtpBackAdd3 = InjectJMP(IMAGE_BASE + 0x155F504, (DWORD)_wtpGetEax, 6); //I don't know which texId to load due to the way wm is constructed
}