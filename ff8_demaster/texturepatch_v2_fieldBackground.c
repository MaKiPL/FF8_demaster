#include "coreHeader.h"
#include <gl/GL.h>

BYTE* fbgBackAdd1;

BYTE* fbgBackAdd3;
BYTE* fbgBackAdd4;


DWORD thisGlSegment;





char maplist[65535];

char* GetFieldBackgroundFile()
{
	char localn[256];
	OutputDebug("GetFieldBackgroundFile()\n");
	DWORD* dc = IMAGE_BASE + 0x189559C;
	char* c = *dc + 0x118;

	strcpy(maplist, c);

	int fieldId = *(DWORD*)(IMAGE_BASE + 0x1782140) & 0xFFFF;
	char* del = strtok(maplist, "\n");
	sprintf(localn, "GetFieldBackgroundFile()::ReadyMapList at?: %s\n", del);
	OutputDebug(localn);
	int currField = 0;

	while (del != NULL)
	{
		if (currField == fieldId)
			break;
		currField++;
		del = strtok(NULL, "\n");
	}

	if (del == NULL)
		return "ERROR";

	char dirName[3];
	memcpy(dirName, del, 2); //warning- yes, I know- but it doesn't matter. IO_func is set to load null.png if not found
	dirName[2] = '\0';

	char n[256];
	n[0] = '\0';
	sprintf(n, "field_bg\\%s\\%s\\%s_", dirName, del, del);
	OutputDebug(n);
	OutputDebug("\n");
	return n;
}

DWORD fbpRequestedTpage;

char* _fbgHdInjectVoid()
{
	char n[256];
	char localn[256];
	n[0] = '\0';
	strcat(n, GetFieldBackgroundFile());
	int palette = tex_header[52];
	sprintf(localn, "%stextures\\%s%u_%u.png", DIRECT_IO_EXPORT_DIR, n, fbpRequestedTpage-16, palette);
	DWORD attr = GetFileAttributesA(localn);
	sprintf(localn, "fbp_Requesting: %s\n", localn);
	OutputDebug(localn);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		strcat(n, "%u");
		return n;
	}
	else
	{
		strcat(n, "%u_");
		strcpy(localn, n);
		sprintf(n, "%s%u", localn, palette);
		return n;
	}
}

__declspec(naked) void _fbgHdInject()
{
	__asm
	{
		PUSH EBX
		PUSH EDX

		MOV EAX, dword ptr[edi+0xC]
		MOV fbpRequestedTpage, EAX
		CALL _fbgHdInjectVoid

		POP EDX
		POP EBX

		MOV ECX, fbpRequestedTpage
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
	OutputDebug("_fbgCheckHdAvailable()\n");
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
		MOV EDX, OFFSET IMAGE_BASE
		MOV EDX, [EDX]
		ADD EDX, 0x1782080
		MOV [EDX], EAX

		POP EDX
		POP ECX
		POP EBX
		POP EAX
		JMP fbgBackAdd4
	}
}





void ApplyFieldBackgroundPatch()
{
	fbgBackAdd4 = InjectJMP(IMAGE_BASE + 0x1591B75, (DWORD)_fbgCheckHdAvailable, 20);

		//disable tpage 16&17 limit
		modPage(IMAGE_BASE + 0x1606595, 1); 
		*(BYTE*)(IMAGE_BASE + 0x1606595) = 0xEB;

	//we now inject JMP when CMP fieldIfd, gover and do out stuff, then return to glSegment
		fbgBackAdd3 = InjectJMP(IMAGE_BASE + 0x1606540, (DWORD)_fbgHdInject, 42);//169-11);
}
