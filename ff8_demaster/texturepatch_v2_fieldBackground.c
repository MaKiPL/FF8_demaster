#include "coreHeader.h"
#include <gl/GL.h>

BYTE* fbgBackAdd1;

BYTE* fbgBackAdd3;
BYTE* fbgBackAdd4;


DWORD thisGlSegment;





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
	memcpy(dirName, del, 2); //warning- yes, I know- but it doesn't matter. IO_func is set to load null.png if not found
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

//This is to make sure that user has PNGs- if not, then use original textures. You can use
//it to individually control fields- pretty cool
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





void ApplyFieldBackgroundPatch()
{
	//this forces all fields to be replaced with HD function
	// ORIG
	//	some_fieldID+3E5  66 83 3D 40 21 78 11 4B cmp     fieldId, 4Bh ; 'K'
	//	some_fieldID+3ED  74 1A                   jne      short loc_11591B99

	fbgBackAdd4 = InjectJMP(IMAGE_BASE + 0x1591B75, (DWORD)_fbgCheckHdAvailable, 20);
		/*modPage(IMAGE_BASE + 0x1591B75, 10);
		*(DWORD*)(IMAGE_BASE + 0x1591B75) = 0x90909090;
		*(DWORD*)(IMAGE_BASE + 0x1591B79) = 0x90909090;
		*(WORD*)(IMAGE_BASE + 0x1591B7D) = 0x9090;*/

		//disable tpage 16&17 limit
		modPage(IMAGE_BASE + 0x1606595, 1); 
		*(BYTE*)(IMAGE_BASE + 0x1606595) = 0xEB;

	//we now inject JMP when CMP fieldIfd, gover and do out stuff, then return to glSegment
		fbgBackAdd3 = InjectJMP(IMAGE_BASE + 0x1606540, (DWORD)_fbgHdInject, 42);//169-11);


	//should return at 116065D2 and should have eax at 


	//fbgBackAdd3 = InjectJMP(IMAGE_BASE + 0x155CD87, (DWORD)_fbgTest, 7);
}
