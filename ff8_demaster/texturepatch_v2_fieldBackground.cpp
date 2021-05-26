#include "coreHeader.h"

BYTE* fbgBackAdd1;
BYTE* fbgBackAdd3;
BYTE* fbgBackAdd4;
DWORD thisGlSegment;

char maplist[65535];

void GetFieldBackgroundFile(char* buffer)
{
	DWORD* dc = (DWORD*)(IMAGE_BASE + 0x189559C);
	char* c = (char*)(*dc + 0x118);

	strcpy(maplist, c);

	int fieldId = *(DWORD*)(IMAGE_BASE + 0x1782140) & 0xFFFF;
	char* del = strtok(maplist, "\n");
	OutputDebug("%s()::ReadyMapList at?: %s\n", __func__, del);
	int currField = 0;

	while (del != NULL)
	{
		if (currField == fieldId)
			break;
		currField++;
		del = strtok(NULL, "\n");
	}

	if (del == NULL)
		return;

	char dirName[3]{ 0 };
	memcpy(dirName, del, 2); //warning- yes, I know- but it doesn't matter. IO_func is set to load null.png if not found

	sprintf(buffer, "field_bg\\%s\\%s\\%s_", dirName, del, del);
	OutputDebug("%s\n", buffer);
}

DWORD fbpRequestedTpage;

char* _fbgHdInjectVoid()
{
	//directIO_fopenReroute: DEMASTER_EXP\textures\DEMASTER_EXP\textures\field_bg\bv\bvtr_1\bvtr_1_0_2.png, file not found
	static char n[256]{ 0 };
	char n2[256]{ 0 };
	char localn[256]{ 0 };
	static char localn2[256]{ 0 };
	int palette = tex_header[52];

	GetFieldBackgroundFile(n);
	
	sprintf(localn, "%stextures\\%s%u_%u.dds", DIRECT_IO_EXPORT_DIR, n, fbpRequestedTpage-16, palette/2);
	sprintf(localn2, "%s%u_%u", n, fbpRequestedTpage - 16, palette/2);
	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		sprintf(localn, "%stextures\\%s%u_%u.png", DIRECT_IO_EXPORT_DIR, n, fbpRequestedTpage - 16, palette/2);
	}
	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("%s: %s, %s\n", __func__, localn, "palette not found");
		strcat(n, "%u");
		sprintf(n2, n, fbpRequestedTpage - 16);
		OutputDebug("%s: %s\n", __func__, n2);
		return n2;
	}
	else
	{
		OutputDebug("%s: %s\n", __func__, localn);
		return localn2;
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
	char n[256]{ 0 };
	char localn[256]{ 0 };

	GetFieldBackgroundFile(n);
	
	sprintf(localn, "%stextures\\%s0.dds", DIRECT_IO_EXPORT_DIR, n);
	

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("%s: %s, %s\n", __func__, localn, "not found");
		sprintf(localn, "%stextures\\%s0.png", DIRECT_IO_EXPORT_DIR, n);
	}


	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("%s: %s, %s\n", __func__, localn, "not found");
		return 0;
	}

	OutputDebug("%s: %s\n", __func__, localn);
	
	return 1;
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
