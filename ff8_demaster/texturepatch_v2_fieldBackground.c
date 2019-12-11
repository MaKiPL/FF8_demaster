#include "coreHeader.h"
#include <gl/GL.h>

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
	if (textureType != 57) { //I don't want field chara tex update spam
		sprintf(n, "common_load_texture: tex_type: %d, pal: %d, unk: %08x, bHaveHD: %s, unkCheckHD: %d\n", textureType, palette, unknownDword, bHaveHD > 0 ? "TRUE" : "FALSE", unknownCheckHD);
		OutputDebugStringA(n);
	}
	return;
}

char maplist[65535];

void _fbgTESTvoid()
{
	int textureType = tex_struct[48];

	return; ///DEBUG OUT - you probably don't have that path below so just return and make it go as always

	if (textureType == 25)
	{
		//is maplist available?
		DWORD* dc = IMAGE_BASE + 0x189559C;
		char* c = *dc + 0x118;
		if (strncmp(c, "wm00", 4))
			return;

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

		int testStruct = tex_struct[50];

		char n[256];
		sprintf(n, "D:\\FFVIII\\Project Angelwing\\%s\\%s\\%s_%d.png", dirName, del, del, testStruct-16);


		int width, height, channel;
		char* buffer = stbi_load(n, &width, &height, &channel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,channel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, buffer);
		stbi_image_free(buffer);
		strcat(n, "\n");
		OutputDebugStringA(n);
	}
	return; //I like cats;
}

//this is just a test to manually inject glTexture of field
__declspec(naked) void _fbgTest()
{
	__asm
	{
		PUSH EAX 
		PUSH EBX
		PUSH ECX
		PUSH EDX
		
		CALL _fbgTESTvoid

		POP EDX
		POP ECX
		POP EBX
		POP EAX


		//original code
		CMP dword ptr [edx+0xC4], 0

		JMP fbgBackAdd3
	}
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
	//we need to open the field.fs/mapdata.fs/maplist

	fbgBackAdd1 = InjectJMP(IMAGE_BASE + 0x155CD05, (DWORD)_fbgObtainTexHeader, 5);
	fbgBackAdd2 = InjectJMP(IMAGE_BASE + 0x155CD7A, (DWORD)_fbgObtainTexStruct, 7);
	fbgBackAdd3 = InjectJMP(IMAGE_BASE + 0x155CD87, (DWORD)_fbgTest, 7);
}
