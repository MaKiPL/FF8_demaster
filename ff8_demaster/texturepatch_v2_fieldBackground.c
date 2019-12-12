#include "coreHeader.h"
#include <gl/GL.h>

BYTE* fbgBackAdd1;
BYTE* fbgBackAdd2;
BYTE* fbgBackAdd3;
BYTE* fbgBackAdd4;


DWORD* tex_header;
DWORD* tex_struct;
DWORD* gl_textures;

DWORD thisGlSegment;

void _fbgVoid()
{
	int textureType = tex_struct[48];
	int palette = tex_header[52];
	UINT unknownDword = tex_struct[65];
	BOOL bHaveHD = tex_struct[49];
	DWORD unknownCheckHD = tex_struct[47];

	char n[256];
	if (textureType != 57) { //I don't want field chara tex update spam
		sprintf(n, "common_load_texture: tex_type: %d, pal: %d, unk: %08x, bHaveHD: %s, unkCheckHD: %d\n", textureType, palette, unknownDword, bHaveHD > 0 ? "TRUE" : "FALSE", unknownCheckHD);
		OutputDebugStringA(n);
	}
	return;
}



char maplist[65535];

char* GetFieldBackgroundFile()
{
	DWORD* dc = IMAGE_BASE + 0x189559C;
	char* c = *dc + 0x118;

	strcpy(maplist, c);

	int fieldId = *(DWORD*)(IMAGE_BASE + 0x1782140);
	char* del = strtok(maplist, "\n");
	int currField = 0;

	while (del != NULL)
	{
		if (currField == fieldId)
			break;
		currField++;
		del = strtok(NULL, "\n");
	}



	char dirName[3];
	memcpy(dirName, del, 2);
	dirName[2] = '\0';

	char n[256];
	n[0] = '\0';
	sprintf(n, "field_bg\\%s\\%s\\%s_", dirName, del, del);
	return n;
}

char* _fbgHdInjectVoid()
{
	char n[256];
	n[0] = '\0';
	strcat(n, GetFieldBackgroundFile());
		OutputDebugStringA(n);
		OutputDebugStringA("\n");
		strcat(n, "%u");
		
		return n;
}

__declspec(naked) void _fbgHdInject()
{
	__asm
	{
		PUSH EBX
		PUSH ECX
		PUSH EDX

		CALL _fbgHdInjectVoid

		POP EDX
		POP ECX
		POP EBX

		MOV ECX, dword ptr[edi + 0xC]
		SUB ECX, 16
		PUSH ECX
		
		PUSH EAX
		LEA EAX, [EBP - 0x58]
		PUSH EAX

		JMP fbgBackAdd3
	}
}

DWORD _fbgCheckHdAvailableVoid()
{
	char n[256];
	n[0] = '\0';
	strcat(n, DIRECT_IO_EXPORT_DIR);
	strcat(n, "textures\\");
	strcat(n, GetFieldBackgroundFile());
	strcat(n, "0.png");
	DWORD attrib = GetFileAttributesA(n);
	if (attrib == INVALID_FILE_ATTRIBUTES)
		return 0;
	else return 1;
}

__declspec(naked) void _fbgCheckHdAvailable()
{
	__asm 
	{
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX

		CALL _fbgCheckHdAvailableVoid
		MOV DWORD PTR ds:0x11782080, EAX

		POP EDX
		POP ECX
		POP EBX
		POP EAX
		JMP fbgBackAdd4
	}
}

__declspec(naked) void _fbgObtainTexHeader()
{
	__asm
	{
		MOV tex_header, ESI
		MOV gl_textures, EDI

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
	//we need to open the field.fs/mapdata.fs/maplist

	fbgBackAdd1 = InjectJMP(IMAGE_BASE + 0x155CD05, (DWORD)_fbgObtainTexHeader, 5);
	fbgBackAdd2 = InjectJMP(IMAGE_BASE + 0x155CD7A, (DWORD)_fbgObtainTexStruct, 7);

	//this forces all fields to be replaced with HD function
	// ORIG
	//	some_fieldID+3E5  66 83 3D 40 21 78 11 4B cmp     fieldId, 4Bh ; 'K'
	//	some_fieldID+3ED  74 1A                   jne      short loc_11591B99

	fbgBackAdd4 = InjectJMP(IMAGE_BASE + 0x1591B75, (DWORD)_fbgCheckHdAvailable, 20);
		/*modPage(IMAGE_BASE + 0x1591B75, 10);
		*(DWORD*)(IMAGE_BASE + 0x1591B75) = 0x90909090;
		*(DWORD*)(IMAGE_BASE + 0x1591B79) = 0x90909090;
		*(WORD*)(IMAGE_BASE + 0x1591B7D) = 0x9090;*/


		modPage(IMAGE_BASE + 0x1606540, 1);
		*(BYTE*)(IMAGE_BASE + 0x1606540) = 0xEB;

		//disable tpage 16&17 limit
		modPage(IMAGE_BASE + 0x1606595, 1); 
		*(BYTE*)(IMAGE_BASE + 0x1606595) = 0xEB;

	//we now inject JMP when CMP fieldIfd, gover and do out stuff, then return to glSegment
		fbgBackAdd3 = InjectJMP(IMAGE_BASE + 0x1606540, (DWORD)_fbgHdInject, 42);//169-11); 


	//should return at 116065D2 and should have eax at 


	//fbgBackAdd3 = InjectJMP(IMAGE_BASE + 0x155CD87, (DWORD)_fbgTest, 7);
}
