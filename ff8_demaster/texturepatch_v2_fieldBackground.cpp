#include "coreHeader.h"

BYTE* _asm_FieldBgRetAddr1;
BYTE* _asm_FieldBgRetAddr2;
BYTE* _asm_FieldBgRetAddr3;

static char maplist[65535] = {'\0'};
std::vector<std::string> maplistVector;

/// <summary>
/// Gets field map name from maplist data
/// </summary>
/// <param name="buffer">[OUT] - places found mapname from maplist</param>
void GetFieldBackgroundFilename(char* buffer)
{
	OutputDebug("%s::%d\n", __func__, __LINE__);

	char* gameMaplist = (char*)(*(DWORD*)(IMAGE_BASE + 0x189559C) + 0x118);

	//no need to copy maplist every time
	if(maplist[0] == '\0')
		strcpy(maplist, gameMaplist);

	int fieldId = *(DWORD*)(IMAGE_BASE + 0x1782140) & 0xFFFF;
	int currentFieldId = 0;
	if (maplistVector.size() == 0)
	{
		char* mapName = strtok(maplist, "\n");
		if(mapName != NULL)
			maplistVector.push_back(std::string(mapName));
		while (mapName != NULL)
		{
			mapName = strtok(NULL, "\n");
			if (mapName != NULL)
				maplistVector.push_back(std::string(mapName));
		}
	}

	std::string mapName(maplistVector[fieldId]);
	std::string dirName(mapName);
	dirName.erase(2, dirName.length()-2); //get only two chars


	sprintf(buffer, "field_bg\\%s\\%s\\%s_", dirName.c_str(), mapName.c_str(), mapName.c_str());
	OutputDebug("%s::%d- %s\n", __func__, __LINE__,buffer);
}

DWORD fieldBackgroundRequestedTPage;
DWORD fieldReplacementFound;

/// <summary>
///
/// </summary>
/// <returns>char* that points to texture of DDS or PNG</returns>
char* GetFieldBackgroundReplacementTextureName()
{
	//directIO_fopenReroute: DEMASTER_EXP\textures\DEMASTER_EXP\textures\field_bg\bv\bvtr_1\bvtr_1_0_2.png, file not found
	char n[256]{ 0 };
	static char n2[256]{ 0 };
	char localn[256]{ 0 };
	static char localn2[256]{ 0 };
	int palette = tex_header[52];

	GetFieldBackgroundFile(n);

	sprintf(localn2, "%s%u_%u", n, fbpRequestedTpage - 16, palette);
	DDSorPNG(localn,256, "%stextures\\%s%u_%u", DIRECT_IO_EXPORT_DIR, n, fbpRequestedTpage-16, palette);

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("%s: %s, %s\n", __func__, localn, "palette not found");
		sprintf(n2, "%s%u",n, fbpRequestedTpage - 16);
		OutputDebug("%s: %s\n", __func__, n2);
		return n2;
	}
	else
	{
		OutputDebug("%s: %s\n", __func__, localn);
		return localn2;
	}
}

__declspec(naked) void _asm_InjectFieldBackgroundModule()
{
	__asm
	{
	//save stack
		PUSH EBX
		PUSH EDX

		//get requested tPage
		MOV EAX, dword ptr[edi+0xC]
		MOV fieldBackgroundRequestedTPage, EAX


		CALL GetFieldBackgroundReplacementTextureName

		//restore stack
		POP EDX
		POP EBX

		//original code restore
		MOV ECX, fieldBackgroundRequestedTPage
		SUB ECX, 16
		PUSH ECX

		PUSH EAX
		LEA EAX, [EBP - 0x58]
		PUSH EAX

		JMP _asm_FieldBgRetAddr2
	}
}

/// <summary>
/// Passes the DWORD if either the replacement is found or not directly to the game engine
/// </summary>
/// <returns></returns>
DWORD GetFieldBackgroundReplacementExist()
{
	OutputDebug("%s::%d\n", __func__, __LINE__);
	const size_t s = 256U;
	char n[s]{ 0 };
	char localn[s]{ 0 };
	GetFieldBackgroundFilename(n);
	DDSorPNG(localn, s, "%stextures\\%s0", DIRECT_IO_EXPORT_DIR, n);

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("%s: %s, %s\n", __func__, localn, "not found");
		return 0;
	}

	fieldReplacementFound = 1;

	OutputDebug("%s: fieldReplacementFound: %d %s\n", __func__, fieldReplacementFound, localn);

	return fieldReplacementFound;
}

__declspec(naked) void _asm_CheckTextureReplacementExists()
{
	__asm
	{
	//save stack
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX

		CALL GetFieldBackgroundReplacementExist //returns EAX
		MOV EDX, OFFSET IMAGE_BASE
		MOV EDX, [EDX]
		ADD EDX, 0x1782080
		MOV [EDX], EAX //pushes GetFieldBackgroundReplacementExist DWORD bool to [EDX]

		//restore stack
		POP EDX
		POP ECX
		POP EBX
		POP EAX
		JMP _asm_FieldBgRetAddr3
	}
}





void ApplyFieldBackgroundPatch()
{
	_asm_FieldBgRetAddr3 = InjectJMP(IMAGE_BASE + 0x1591B75, (DWORD)_asm_CheckTextureReplacementExists, 20);

		//disable tpage 16&17 limit
		modPage(IMAGE_BASE + 0x1606595, 1);
		*(BYTE*)(IMAGE_BASE + 0x1606595) = 0xEB;

	//we now inject JMP when CMP fieldIfd, gover and do out stuff, then return to glSegment
		_asm_FieldBgRetAddr2 = InjectJMP(IMAGE_BASE + 0x1606540, (DWORD)_asm_InjectFieldBackgroundModule, 42);//169-11);
}
