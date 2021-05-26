#include "coreHeader.h"

/*
KURSE ALL SEEDS!
*/


/*
*
*
*/
DWORD IMAGE_BASE = 0;
DWORD OPENGL_HANDLE = 0;
const char * DIRECT_IO_EXPORT_DIR = "DEMASTER_EXP\\";
FILE* logFile = NULL;
DWORD* tex_header = 0;
DWORD attr = -1;
DWORD DIRECT_IO_EXPORT_DIR_LEN = -1;
DWORD* tex_struct = 0;
DWORD* gl_textures= 0;
DWORD pixelsPtr = 0;
DWORD texturesPtr = 0;
DWORD TEX_TYPE = 0;
int currentStage = -1;
DWORD* langIdent_ESI;
BOOL UVPATCH, DIRECT_IO, TEXTURE_PATCH, DEBUG_PATCH, LOG;
BOOL BATTLE_CHARA, FIELD_ENTITY, BATTLE_HOOK, FIELD_BACKGROUND, WORLD_TEXTURES;
BOOL LINEAR_PATCH, OPENGL_HOOK;

// REQUIRED BY BIMG FILE DECODING
bx::DefaultAllocator texAllocator;

void OutputDebug(const char* fmt, ...)
{
	va_list args;
	char tmp_str[1024];

	va_start(args, fmt);
	vsnprintf(tmp_str, sizeof(tmp_str), fmt, args);
	va_end(args);

	printf(tmp_str);

	if (logFile != NULL)
	{
		fwrite(tmp_str, sizeof(char), strlen(tmp_str), logFile);
		fflush(logFile);
	}
}

DWORD _dllmainBackAddr1;
DWORD _dllmainBackAddr2;

const char* windowTitle = "FINAL FANTASY VIII Remastered - Demaster patch by Maki [1.2.8] BETA";

__declspec(naked) void _dllMain2()
{
	__asm
	{
		PUSH windowTitle
		JMP _dllmainBackAddr2
	}
}


void GetWindow()
{
	_dllmainBackAddr2 = (DWORD)InjectJMP(IMAGE_BASE + 0x1601065, (DWORD)_dllMain2, 5);
}

//DO NOT DELETE- it acts as an anchor for EFIGS.dll import
EXPORT void InitTest()
{
	OutputDebug("DEMASTER ENGINE LOADED!\n");
	return;
}

DWORD lastJMP;

void DEB_JMP(char* c, DWORD a, DWORD b, DWORD cc, DWORD d, DWORD e)
{
	if (c < (char*)IMAGE_BASE)
		return;
	char localD[32];
	localD[0] = '\0';
	sprintf(localD, "Wrong address at: %08x\n", (unsigned int)c);
	if (IsBadReadPtr(c, 4)) 
	{
		__asm
		{
			//INT 3
		}
		return;
	}
	if (*c == 183)
	{
		__asm
		{
			INT 3
		}
	}
	OutputDebug(c, a, b, cc, d, e);
	return;
}

__declspec(naked) void nullsub()
{
	__asm
	{
		ret
	}
}

DWORD DEB_backAdd;

void ApplyDebugOutputPatch()
{
	DEB_backAdd = (DWORD)InjectJMP(IMAGE_BASE + 0x33DC0, (DWORD)DEB_JMP, 5);
	//critical sections are null here, but they point to common_fieldEC, that's not cool
	*(DWORD*)(IMAGE_BASE + 0x16EDF68) = (DWORD)nullsub;
	*(DWORD*)(IMAGE_BASE + 0x16EDF7C) = (DWORD)nullsub;
	*(DWORD*)(IMAGE_BASE + 0x16EDA7C) = (DWORD)nullsub;
	*(DWORD*)(IMAGE_BASE + 0x16EDD74) = (DWORD)nullsub;
	modPage(IMAGE_BASE + 0x1696058, 4);
	*(DWORD*)(IMAGE_BASE + 0x1696058) = (DWORD)nullsub;
	modPage(IMAGE_BASE + 0x169605C, 4);
	*(DWORD*)(IMAGE_BASE + 0x169605C) = (DWORD)nullsub;
	ReplaceCALLWithNOP(0x159F3BD);
	ReplaceCALLWithNOP(0x4485FF);
	ReplaceCALLWithNOP(0x15824B6);
}

DWORD _deb00_ECX;
DWORD _deb00_EAX;

bool DEBUG_reverseWm = true;

void DEB_JMPv2_00()
{
	const char* format = "FSArchive:: %s - %s\n";
	char* path = (char*)_deb00_EAX;
	if (DEBUG_reverseWm) 
	{
		std::string pathStr(path);
		std::size_t fnd = pathStr.find("wmsetus.obj", 0);
		//if (fnd != std::string::npos)
		//	DebugBreak();
	}
	OutputDebug(format, path, _deb00_ECX);
}

__declspec(naked) void DEB_JMPv2()
{
	__asm
	{
		MOV _deb00_ECX, ECX
		MOV _deb00_EAX, EAX
		CALL DEB_JMPv2_00
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, 0x1692994
		PUSH EAX
		JMP DEB_backAdd
	}
}

void ApplyDebugOutputPatchV2()
{
	DEB_backAdd = (DWORD)InjectJMP(IMAGE_BASE + 0x1581270, (DWORD)DEB_JMPv2, 5);

}

//force GL_LINEAR with non-fixed UV was a d*k move ...
void ApplyFilteringPatch()
{
	//InjectDWORD(IMAGE_BASE + 0x1564D97 + 1, 0x2600); //main indirect
	//InjectDWORD(IMAGE_BASE + 0x1564D86 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x1564F23 + 1, 0x2600); //main indirect
	//InjectDWORD(IMAGE_BASE + 0x1564F34 + 1, 0x2600);


	//InjectDWORD(IMAGE_BASE + 0x156505E + 1, 0x2600); //unknown glDrawElements
	//InjectDWORD(IMAGE_BASE + 0x1565073 + 1, 0x2600);
	//
	//InjectDWORD(IMAGE_BASE + 0x1566ED5 + 1, 0x2600); //as above fxaa2
	//InjectDWORD(IMAGE_BASE + 0x1566EE6 + 1, 0x2600);



	//InjectDWORD(IMAGE_BASE + 0x15686D3 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x15686E4 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x1568805 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x1568816 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x15689C9 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x15689DA + 1, 0x2600);

	InjectDWORD(IMAGE_BASE + 0x15693EF + 1, 0x2600);
	InjectDWORD(IMAGE_BASE + 0x1569409 + 1, 0x2600);
	InjectDWORD(IMAGE_BASE + 0x156A348 + 1, 0x2600);
	InjectDWORD(IMAGE_BASE + 0x156A359 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x156D47F + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x156D490 + 1, 0x2600);

	//InjectDWORD(IMAGE_BASE + 0x156D830 + 1, 0x2600);
	//InjectDWORD(IMAGE_BASE + 0x156D841 + 1, 0x2600);
}

void ReadConfigFile()
{
	if (GetFileAttributesA(DEMASTER_CONF) == INVALID_FILE_ATTRIBUTES)
	{
		OutputDebug("File " DEMASTER_CONF " not found- all failed\n");
		return;
	}
	OutputDebug("Reading config file " DEMASTER_CONF "\n");
	//ini_t* conf = ini_load(DEMASTER_CONF);

	INIReader conf(DEMASTER_CONF);

	UVPATCH = conf.GetInteger("", "UV_PATCH", 0);
	DIRECT_IO = conf.GetInteger("", "DIRECT_IO", 0);
	LOG = conf.GetInteger("", "LOG", 0);
	DEBUG_PATCH = conf.GetInteger("", "MORE_DEBUG_OUTPUT_PATCH", 0);
	BATTLE_CHARA = conf.GetInteger("", "BATTLE_CHARACTER", 0);
	FIELD_ENTITY = conf.GetInteger("", "FIELD_ENTITY", 0);
	BATTLE_HOOK = conf.GetInteger("", "BATTLE_HOOK_MONSTER_FIELDS", 0);
	FIELD_BACKGROUND = conf.GetInteger("", "FIELD_BACKGROUND", 0);
	WORLD_TEXTURES = conf.GetInteger("", "WORLD_TEXTURES", 0);
	TEXTURE_PATCH = conf.GetInteger("", "TEXTURE_PATCH", 1); //this one lacks actual demaster.conf so default to 1
	LINEAR_PATCH = conf.GetInteger("", "LINEAR_PATCH", 1);
	OPENGL_HOOK = conf.GetInteger("", "OPENGL_HOOK", 0);
}

LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* ep)
{
	DemasteredStackWalker sw;

	OutputDebug("*** Exception 0x%x, address 0x%x ***\n", ep->ExceptionRecord->ExceptionCode, ep->ExceptionRecord->ExceptionAddress);
	sw.ShowCallstack(
		GetCurrentThread(),
		ep->ContextRecord
	);

	// show cursor in case it was hidden
	ShowCursor(true);

	MessageBoxA(0, "Oops! Something very bad happened.\n\nPlease provide a copy of demasterlog.txt when reporting this error at https://github.com/MaKiPL/FF8_demastered/issues.\n", "Error", MB_OK);

	// let OS handle the crash
	SetUnhandledExceptionFilter(0);
	return EXCEPTION_CONTINUE_EXECUTION;
}

bimg::ImageContainer* LoadImageFromFile(char* filename)
{
	static bool glewLoaded = false;

	if (!glewLoaded)
	{
		glewLoaded = true;

		// INIT GLEW - Add recent OpenGL extension support ( required for Texture Compression )
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			OutputDebug("%s - GLEW Error: %s\n", __func__, glewGetErrorString(err));
		}
	}

	bimg::ImageContainer* img = nullptr;
	char msg[1024]{ 0 };

	OutputDebug("%s - Opening File: %s\n", __func__, filename);

	FILE* file = fopen(filename, "rb");

	if (file)
	{
		size_t filesize = 0;
		char* buffer = nullptr;

		fseek(file, 0, SEEK_END);
		filesize = ftell(file);

		buffer = (char*)malloc(filesize + 1);
		fseek(file, 0, SEEK_SET);
		(void)fread(buffer, filesize, 1, file);

		fclose(file);

		// ==================================

		if (buffer != nullptr)
		{
			img = bimg::imageParse(&texAllocator, buffer, filesize + 1);

			free(buffer);
		}
	}

	return img;
}
void RenderTexture(bimg::ImageContainer* img, TextureFormatInfo& texInfo)
{
	if (bimg::isCompressed(img->m_format))
	{
		RenderCompressedTexture(img, texInfo);
	}
	else
	{
		RenderUncompressedTexture(img, texInfo);
	}
}

void RenderUncompressedTexture(bimg::ImageContainer* img, TextureFormatInfo& texInfo)
{
	uint32_t width = img->m_width;
	uint32_t height = img->m_height;
	uint32_t depth = img->m_depth;
	const uint8_t startLod = bx::min<uint8_t>(0, img->m_numMips - 1);

	for (uint8_t lod = 0, num = img->m_numMips; lod < num; ++lod)
	{
		width = bx::uint32_max(1, width);
		height = bx::uint32_max(1, height);
		depth = 1;

		bimg::ImageMip mip;
		if (bimg::imageGetRawData(*img, 0, lod + startLod, img->m_data, img->m_size, mip))
			glTexImage2D(GL_TEXTURE_2D, lod, texInfo.m_internalFmt, img->m_width, img->m_height, 0, texInfo.m_fmt, texInfo.m_type, mip.m_data);
	}
}

void RenderCompressedTexture(bimg::ImageContainer* img, TextureFormatInfo& texInfo)
{
	if (GLEW_ARB_texture_compression)
	{
		uint32_t width = img->m_width;
		uint32_t height = img->m_height;
		uint32_t depth = img->m_depth;
		const uint8_t startLod = bx::min<uint8_t>(0, img->m_numMips - 1);

		for (uint8_t lod = 0, num = img->m_numMips; lod < num; ++lod)
		{
			width = bx::uint32_max(1, width);
			height = bx::uint32_max(1, height);
			depth = 1;

			bimg::ImageMip mip;
			if (bimg::imageGetRawData(*img, 0, lod + startLod, img->m_data, img->m_size, mip))
				glCompressedTexImage2D(GL_TEXTURE_2D, lod, texInfo.m_internalFmt, img->m_width, img->m_height, 0, mip.m_size, mip.m_data);
		}
	}
	else
		OutputDebug("Texture is compressed, but compression is not supported on your GPU. Skipping draw.");
}

BOOL WINAPI DllMain(

	HINSTANCE hinstDLL, // handle to DLL module
	DWORD fdwReason, // reason for calling function
	LPVOID lpReserved) // reserved
{
	//MessageBoxA(0, "DLL loaded at DllMain- Maki", "test", MB_OK);
	if (fdwReason != DLL_PROCESS_ATTACH) //fail if not on app-init. Attaching is not recommended, should be loaded at startup by import
		return 0;

	SetUnhandledExceptionFilter(ExceptionHandler);

	AllocConsole();
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CON", "r", stdin);
	InitTest();
	ReadConfigFile();
	if (LOG) logFile = fopen("demasterlog.txt", "wb");

	HMODULE IMG_BASE = GetModuleHandleA("FFVIII_EFIGS");
	IMAGE_BASE = (long long)IMG_BASE;
	OPENGL_HANDLE = (long long)GetModuleHandleA("OPENGL32");
	OutputDebug("IMAGE_BASE at: %lX; OPENGL at: %lX\n", IMAGE_BASE, OPENGL_HANDLE);

	GetWindow();

	//LET'S GET THE HACKING DONE
	if(DIRECT_IO)
		ApplyDirectIO();
	if(UVPATCH)
		ApplyUVPatch();
	if(TEXTURE_PATCH && DIRECT_IO)
		ReplaceTextureFunction();
	if (DEBUG_PATCH)
		ApplyDebugOutputPatchV2();
	if (LINEAR_PATCH)
		ApplyFilteringPatch();
	if (OPENGL_HOOK)
		HookOpenGL();

	//if(DEBUG_PATCH)
	//	ApplyDebugOutputPatch(); //they have hella of debug info shit, but then function is nullsub-
							//quite usual- vanilla ff8 from 2000 had the same for harata battle debug
							//but worry not- we can write such function from scratch
													//ApplyTextureUpscaleMod();
	return 1; //all success
}
