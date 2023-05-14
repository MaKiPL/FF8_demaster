#include "coreHeader.h"
#define CRASHLOG OutputDebug("%s::%d::%s\n", __FILE__, __LINE__,__func__)
BYTE* _asm_FieldBgRetAddr1;
BYTE* _asm_FieldBgRetAddr2;
BYTE* _asm_FieldBgRetAddr3;

#ifndef JAPANESE_PATCH
int BGFILENAME2_R = 0x118;
#else
int BGFILENAME2_R = 0x490;
#endif

/// <summary>
/// Gets field map name from maplist data
/// </summary>
/// <param name="buffer">[OUT] - places found mapname from maplist</param>
/// <param name="force_retry">if fieldId was not found reload maplist</param>
/// <returns>false if no issues</returns>
bool GetFieldBackgroundFilename(char* buffer, bool force_retry = false)
{
	static std::vector<std::string> maplistVector{};
	const int fieldId = *(DWORD*)(IMAGE_BASE + GetAddress(BGFILENAME1)) & 0xFFFF;
	if (maplistVector.empty() || force_retry)
	{
		const auto oldsize = maplistVector.size();
		static size_t cached_maplist_size{};
		if (maplistVector.capacity() < 982) 
			maplistVector.reserve(982);
		const char* const maplist_src = []() {
			const char* tmp_ptr = (const char*)(*(DWORD*)(IMAGE_BASE + GetAddress(BGFILENAME2)) + BGFILENAME2_R + cached_maplist_size);
			if (*tmp_ptr == '\n') //The \n tends to be left at the front on reloading.
				(void)++tmp_ptr, ++cached_maplist_size;
			return tmp_ptr;
		}();
		const std::string maplist = std::string{ maplist_src };
		cached_maplist_size += maplist.size(); // remember how much of the maplist we have read.
		{
			auto iss = std::istringstream(maplist, std::ios::in | std::ios::binary);
			std::string mapname{};
			while (std::getline(iss, mapname))
			{
				maplistVector.emplace_back(std::move(mapname));
			}
		}
		OutputDebug("%s::%d- %s Maplist!\toldsize: %d\tsize: %d\n", __func__, __LINE__, (force_retry ? "ReLoaded" : "Loaded"), oldsize, maplistVector.size());
	}

	if (maplistVector.size() <= static_cast<size_t>(fieldId))
	{
		if (!force_retry)
			return GetFieldBackgroundFilename(buffer, true);
		OutputDebug("%s::%d- Invalid fieldId: %d / %d\n", __func__, __LINE__, fieldId, maplistVector.size());
		{
			size_t i = 0;
			for (const auto map : maplistVector)
			{
				OutputDebug("\t%d - %s", i++, map.c_str());
			}
		}
		return true; //failed to read fieldId
	}

	const std::string& mapName(maplistVector[fieldId]);
	const std::string dirName(mapName.substr(0,2U)); //get only two chars
	sprintf(buffer, "field_bg\\%s\\%s\\%s_", dirName.c_str(), mapName.c_str(), mapName.c_str());
	OutputDebug("%s::%d- %s\n", __func__, __LINE__, buffer);
	return false; // no issues found.
}

DWORD fieldBackgroundRequestedTPage;
DWORD fieldReplacementFound;


static char* lastFieldName;
static int lastFieldnameSubstrPos = -1;
/// <summary>
/// Returns final field path with either DDS or PNG extension with either palettted or non-paletted instance/version
/// </summary>
/// <returns>char* that points to texture of DDS or PNG</returns>
char* GetFieldBackgroundReplacementTextureName()
{
	//directIO_fopenReroute: DEMASTER_EXP\textures\DEMASTER_EXP\textures\field_bg\bv\bvtr_1\bvtr_1_0_2.png, file not found
	char n[256]{ 0 };
	static char n2[256]{ 0 };
	static char localn[256]{ 0 }; //Maki: static so when assigned to lastFieldName it doesn't get fucked out
	static char localn2[256]{ 0 };
	int palette = tex_header[52];

	

	if (GetFieldBackgroundFilename(n))
	{
		n2[0] = '\0';
		return n2;
	}

	

	sprintf(localn2, "%s%u_%u", n, fieldBackgroundRequestedTPage - 16, palette);


	DDSorPNG(localn, 256, "%stextures\\%s%u_%u", DIRECT_IO_EXPORT_DIR, n, fieldBackgroundRequestedTPage - 16, palette);
	DDSorPNG(localn2, 256, "%stextures\\%s%u", DIRECT_IO_EXPORT_DIR, n, fieldBackgroundRequestedTPage - 16, palette);

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{

		OutputDebug("%s: %s, %s\n", __func__, localn, "palette not found");
		lastFieldName = localn2;
		//return localn2; //Maki: it's easier this way for me to replace whenn using no PNG
	}
	else
	{
		lastFieldName = localn; //Maki : localn2 was here instead of localn - probably debug
		OutputDebug("%s: %s\n", __func__, localn);
	}


	if (lastFieldnameSubstrPos == -1)
		lastFieldnameSubstrPos = strlen(DIRECT_IO_EXPORT_DIR) + strlen("textures\\");
	//Maki: below are various after-fixes because we tried to do DDS but this one needs special treatment for GL segment- pretty easy
	lastFieldName[strlen(lastFieldName) - 4] = '\0'; //Maki: This is to throw out the extension- yes, we need to do that so far
	lastFieldName = lastFieldName + lastFieldnameSubstrPos;

	
		return lastFieldName; //Maki: yeah, it's for rework, but returning anyway for assembler inject sake
}

__declspec(naked) void _asm_InjectFieldBackgroundModule()
{
	__asm
	{
		//save stack
		PUSH EBX
		PUSH EDX

		//get requested tPage
		MOV EAX, dword ptr[edi + 0xC]
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

//WIP
void FbgGl()
{
 		DWORD tPage = gl_textures[50];
		int palette = tex_header[52];

		char localn[256]{0};
		if (DDSorPNG(localn, 256, "%stextures\\%s%u_%u", DIRECT_IO_EXPORT_DIR, GetFieldBackgroundReplacementTextureName(), tPage - 16, palette))
		{
			SafeBimg texture = LoadImageFromFile(localn);
			if (texture)
				RenderTexture(texture.get());
		}
		else if (DDSorPNG(localn, 256, "%stextures\\%s%s%u", DIRECT_IO_EXPORT_DIR, GetFieldBackgroundReplacementTextureName(), tPage - 16))
		{
			SafeBimg texture = LoadImageFromFile(localn);
			if (texture)
				RenderTexture(texture.get());
		}

}



//Maki: This gets checked at first- so we have actually ready .PNG path here- change it so we use one method, not three times getting path lol
//			this checks if replacement exists for 0_0 for either DDS or PNG and sets fieldReplacementFound
/// <summary>
/// Passes the DWORD if either the replacement is found or not directly to the game engine
/// </summary>
/// <returns>0 when not found</returns>
DWORD GetFieldBackgroundReplacementExist()
{

	const size_t s = 256U;
	char n[s]{ 0 };
	char localn[s]{ 0 };

	if (GetFieldBackgroundFilename(n))
	{
		n[0] = '\0';
		return 0;
	}


	DDSorPNG(localn, s, "%stextures\\%s0", DIRECT_IO_EXPORT_DIR, n);

	if (GetFileAttributesA(localn) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("%s:%d: %s, %s\n", __func__, __LINE__, localn, "not found");
		return 0;
	}

	fieldReplacementFound = 1;


	OutputDebug("%s: fieldReplacementFound: %d %s\n", __func__, fieldReplacementFound, lastFieldName = localn);

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
		PUSH EAX //let's save it
		PUSH CHECKTEXREPAVAIL
		CALL GetAddress //EAX now holds what we are ment to do into EDX
		MOV EDX, OFFSET IMAGE_BASE
		MOV EDX, [EDX]
		ADD EDX, EAX
		POP EAX //we get EAX back with our repExist value
		MOV[EDX], EAX //pushes GetFieldBackgroundReplacementExist DWORD bool to [EDX]

		//restore stack
		POP EDX
		POP ECX
		POP EBX
		POP EAX
		JMP _asm_FieldBgRetAddr3
	}
}

//appends the correct padding - not force .PNG
//EAX-full path with padding
//ECX-work registry
__declspec(naked) void _asm_InjectExtensionHijack()
{
	__asm
	{
		//PUSH .png - drop from stack
		POP EAX
		MOV EAX, ECX

		MOV EAX, lastFieldName;

		//push final values and escape
		PUSH 0
		PUSH EAX //holds src
		JMP _asm_FieldBgRetAddr1
	}
}



void ApplyFieldBackgroundPatch()
{

	_asm_FieldBgRetAddr3 = InjectJMP(IMAGE_BASE + GetAddress(_ASM_FIELDBGRETADDR3), (DWORD)_asm_CheckTextureReplacementExists, 20);

	//disable tpage 16&17 limit
	ModPage(IMAGE_BASE + GetAddress(DISABLETPAGELIMIT), 1);
	*(BYTE*)(IMAGE_BASE + GetAddress(DISABLETPAGELIMIT)) = 0xEB;

	//we now inject JMP when CMP fieldIfd, gover and do out stuff, then return to glSegment
	_asm_FieldBgRetAddr2 = InjectJMP(IMAGE_BASE + GetAddress(_ASM_FIELDBGRETADDR2), (DWORD)_asm_InjectFieldBackgroundModule, 42);//169-11);

	//skips the .png adding
	//Maki: no, not this way- todo in reborn
	//_asm_FieldBgRetAddr1 = InjectJMP(IMAGE_BASE+0x16065C0, (DWORD)_asm_InjectExtensionHijack, 0x12);
}
#undef CRASHLOG