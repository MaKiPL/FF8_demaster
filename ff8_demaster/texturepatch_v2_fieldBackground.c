#include "coreHeader.h"

BYTE* fbgBackAdd1;
BYTE* fbgBackAdd2;
BYTE* fbgBackAdd3;


DWORD* tex_header;
DWORD* tex_struct;

void _fbgVoid()
{
	int textureType = tex_struct[48];
	int palette = tex_header[52];
	UINT unknownDword = tex_struct[65];
	BOOL bHaveHD = tex_struct[49];
	DWORD unknownCheckHD = tex_struct[47];

	char n[256];
	sprintf(n, "common_load_texture: tex_type: %d, pal: %d, unk: %08x, bHaveHD: %s, unkCheckHD: %d\n", textureType, palette, unknownDword, bHaveHD > 0 ? "TRUE":"FALSE", unknownCheckHD);
	OutputDebugStringA(n);
	return;
}

__declspec(naked) void _fbgObtainTexHeader()
{
	__asm
	{
		MOV tex_header, ESI

//ORIGINAL CODE
MOV EAX, OFFSET IMAGE_BASE
MOV EAX, [EAX]
ADD EAX, 0x1573A40 //createGLTexture
CALL EAX

JMP fbgBackAdd1
	}
}

__declspec(naked) void _fbgObtainTexStruct()
{
	__asm
	{
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX
		MOV tex_struct, EDX
		CALL _fbgVoid

		POP EDX
		POP ECX
		POP EBX
		POP EAX

//ORIGINAL CODE
		
		CMP dword ptr [edx+0x0BC], 0
		JMP fbgBackAdd2
	}
}

void ApplyFieldBackgroundPatch()
{
	fbgBackAdd1 = InjectJMP(IMAGE_BASE + 0x155CD05, (DWORD)_fbgObtainTexHeader, 5);
	fbgBackAdd2 = InjectJMP(IMAGE_BASE + 0x155CD7A, (DWORD)_fbgObtainTexStruct, 7);
}
