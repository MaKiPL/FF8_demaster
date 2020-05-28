#include "coreHeader.h"

int width_fcp = 768;
int height_fcp = 768;

BYTE* fcpBackAdd1;
BYTE* fcpBackAdd2;
BYTE* fcpBackAdd3;

//casual is 384x384 or 768x768, therefore the final should be 1st height * 2
void _fcpObtainTextureDatas(int bIndex, int aIndex)
{
	char texPath[256]{ 0 };
	char tempPath[256]{ 0 };
	char tempSprint[256]{ 0 };
	sprintf(texPath, "%stextures\\field.fs\\field_hd", DIRECT_IO_EXPORT_DIR);

	if (aIndex >= 0xC19)
		sprintf(tempSprint, "\\%s%03u_%u", "p", aIndex - 3097, bIndex);
	else if (aIndex < 0x831)
	{
		if (aIndex < 0x449)
			sprintf(tempSprint, "\\%s%03u_%u", "d", aIndex - 97, bIndex);
		else
			sprintf(tempSprint, "\\%s%03u_%u", "n", aIndex - 1097, bIndex);
	}
	else
		sprintf(tempSprint, "\\%s%03u_%u", "o", aIndex - 2097, bIndex);

	BOOL bNonHdParent = FALSE;

	char testPath[256]{ 0 };
	sprintf(testPath, "%s%s.dds", texPath, tempSprint);
	if (GetFileAttributesA(testPath) == INVALID_FILE_ATTRIBUTES)
		sprintf(testPath, "%s%s.png", texPath, tempSprint);
	if (GetFileAttributesA(testPath) == INVALID_FILE_ATTRIBUTES)
	{
		sprintf(testPath, "%s_new%s.dds", texPath, tempSprint);
		if (GetFileAttributesA(testPath) == INVALID_FILE_ATTRIBUTES)
			sprintf(testPath, "%s_new%s.png", texPath, tempSprint);
	}
	else
		bNonHdParent = TRUE;

	if (GetFileAttributesA(testPath) == INVALID_FILE_ATTRIBUTES)
	{
		sprintf(testPath, "%s_new\\d000_0.dds", tempPath); //ERROR !!!!
		if (GetFileAttributesA(testPath) == INVALID_FILE_ATTRIBUTES)
			sprintf(testPath, "%s_new\\d000_0.png", tempPath); //ERROR !!!!
	}

	strcpy(texPath, testPath); //establish path


	bimg::ImageContainer* img = LoadImageFromFile(texPath);

	//the most important is height here
	height_fcp = img->m_height * 2;
	int scale = img->m_height / 384; //normally should be always 1
	


	bimg::imageFree(img);
	OutputDebug("_fcpObtainTextureDatas:: width=%d, height=%d, filename=%s\n", width_fcp, height_fcp, texPath);
	return;
}


__declspec(naked) void _fcpObtainData()
{
	__asm
	{
		MOV EAX, dword ptr[EDI + 0x10] //yes, once again we get tex_struct + 0xd0 as edi+10h
		MOV tex_struct, EAX
		TEST EAX, EAX
		JZ _fcpSecondRow

		PUSH EAX
		MOV EAX, dword ptr[EDI+0x14]
		DEC EAX
		PUSH EAX
		JMP _fcpCallTexture

		_fcpSecondRow:
		MOV EAX, dword ptr[EDI+0x28]
		PUSH EAX
		MOV EAX, dword ptr [EDI+0x2C]
		DEC EAX
		PUSH EAX

		_fcpCallTexture:
		CALL _fcpObtainTextureDatas
		POP EAX
		POP EAX

		PUSH 0
		PUSH 0
		PUSH 0
		PUSH[height_fcp]
		PUSH[height_fcp]
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x160b670 //createGLTexture
		CALL EAX

		JMP fcpBackAdd1
	}
}

DWORD _fcpCurrentTexMode;

__declspec(naked) void _fcpSetYoffset()
{
	__asm
	{
		CMP [TEX_TYPE], 57
		JNE originalcode
		CMP[EBP + 0x0C], 0
		JE originalcode
		MOV EAX, [height_fcp]
		SHR EAX, 1
		PUSH EAX
		PUSH 0
		JMP returnhere

		originalcode :
		push[ebp + 0x0C]
		push[ebp + 0x08]
		returnhere:
		JMP fcpBackAdd2
	}
}

void ApplyFieldEntityPatch()
{
	//step 1. obtain needed data for tex_struct and etc.
	fcpBackAdd1 = InjectJMP(IMAGE_BASE + 0x16061CC, (DWORD)_fcpObtainData, 18);


	//step 2. disable out of bounds error- we know that, but we are using new, bigger buffers
	modPage(IMAGE_BASE + 0x160C43A, 1);
	*(BYTE*)(IMAGE_BASE + 0x160C43A) = 0xEB; //JBE -> JMP

	modPage(IMAGE_BASE + 0x160C467, 1);
	*(BYTE*)(IMAGE_BASE + 0x160C467) = 0xEB; //JBE -> JMP

	//1160545A - set
	fcpBackAdd2 = InjectJMP(IMAGE_BASE + 0x160C4AD, (DWORD)_fcpSetYoffset, 6);
}