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
	OutputDebug("%s::%d\n", __func__, __LINE__);
	fieldReplacementFound = 0;
	static char fieldName[256] {0};
	static char replacementFieldTextureFilename[256]{ 0 };
	int palette = tex_header[52];

	GetFieldBackgroundFilename(fieldName);

	OutputDebug("%s: %s\n", __func__, fieldName);
	
	sprintf(replacementFieldTextureFilename, "%stextures\\%s%u_%u.dds", DIRECT_IO_EXPORT_DIR, fieldName, fieldBackgroundRequestedTPage-16, palette);
	if (GetFileAttributesA(replacementFieldTextureFilename) == INVALID_FILE_ATTRIBUTES)
		sprintf(replacementFieldTextureFilename, "%stextures\\%s%u_%u.png", DIRECT_IO_EXPORT_DIR, fieldName, fieldBackgroundRequestedTPage - 16, palette);

	OutputDebug("%s: %s\n", __func__, replacementFieldTextureFilename);
	
	//no replacement texture found- fallback to original
	if (GetFileAttributesA(replacementFieldTextureFilename) == INVALID_FILE_ATTRIBUTES)
		return strcat(fieldName, "%u");
	
	fieldReplacementFound = 1;
	return replacementFieldTextureFilename;
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
	char n[256]{ 0 };
	char localn[256]{ 0 };

	GetFieldBackgroundFilename(n);

	sprintf(localn, "%stextures\\%s0.dds", DIRECT_IO_EXPORT_DIR, n);

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
		sprintf(localn, "%stextures\\%s0.png", DIRECT_IO_EXPORT_DIR, n);

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
		return 0;

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
