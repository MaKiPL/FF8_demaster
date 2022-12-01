#include "coreHeader.h"
#include "texture.h"
#include <chrono>
#include <filesystem>
#define XXH_INLINE_ALL
#include <xxhash.h>


void* __stdcall HookGlBindTexture(GLenum target, GLuint texture)
{
	return static_cast<void* (__stdcall*)(GLenum, GLuint)>(ogl_bind_texture)(target, texture);
}

//TODO - this is to handle params lol
void* __stdcall HookGlTextParameteri(GLenum target, GLenum name, GLint param)
{
	// Commenting because forcing filtering in whole game is not good

	/*
	if(target==GL_TEXTURE_2D &&
		(name == GL_TEXTURE_MIN_FILTER || name == GL_TEXTURE_MAG_FILTER))
	{
		return ((void* (__stdcall*)(GLenum, GLenum, GLint))oglTexParametri)
			(target, name, GL_NEAREST);
	}
	else
	*/
	return static_cast<void* (__stdcall*)(GLenum, GLenum, GLint)>(ogl_tex_parametri)
		(target, name, param);
}

void* __stdcall HookGlTexImage2D(GLenum target,
	GLint level,
	GLint internalformat,
	GLsizei width,
	GLsizei height,
	GLint border,
	GLenum format,
	GLenum type,
	const void* data)
{
	int boundId;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundId);
	OutputDebug("glTexImage2D (%d): format: %d, %dx%d, void: %08x\n",
		boundId, internalformat, width, height, data);
	int lengthModifier = 0;
	if (internalformat == GL_RGBA || internalformat == GL_BGRA
		|| internalformat == GL_RGBA8)
		lengthModifier = 4;
	else if (internalformat == GL_RGB || internalformat == GL_BGR
		|| internalformat == GL_RGB8)
		lengthModifier = 3;
#if HASH_FEATURE
	if (data != nullptr && width != 0 && height != 0 && lengthModifier != 0)
	{
		const std::chrono::time_point<std::chrono::steady_clock> start =
			std::chrono::high_resolution_clock::now();
		//XXH32_hash_t hash = XXH32(data, width * height * lengthModifier, 0x85EBCA77U); //32ms
		const XXH128_hash_t hash = XXH128(data, width * height *
			lengthModifier, 0x85EBCA77U);
		if (!knownTextures.contains(hash.high64))
		{
			OutputDebug("New hash for textureId: %u. Hash: %016llX%016llX ",
				boundId, hash.high64, hash.low64);
			knownTextures.insert(std::pair(hash.high64, texImageInformation{
							hash.low64,(GLuint)boundId, internalformat, width, height }));
			std::filesystem::path destinationPath = std::filesystem::current_path();
			destinationPath.append(std::string(DIRECT_IO_EXPORT_DIR)
				+ "hashOutput");
			if (!std::filesystem::exists(destinationPath))
				std::filesystem::create_directory(destinationPath);
			std::stringstream ss;
			ss << std::setw(16) << std::setfill('0') << std::hex
				<< std::uppercase << hash.high64 << std::dec << std::nouppercase;
			destinationPath.append(ss.str() + ".png");
			if (!std::filesystem::exists(destinationPath))
			{
				bx::FileWriter writer;
				bimg::TextureFormat::Enum fmt = bimg::TextureFormat::RGBA8;
				switch (internalformat)
				{
				case GL_RGB8: case GL_RGB: case GL_BGR: fmt = bimg::TextureFormat::RGB8; break;
				case GL_BGRA: fmt = bimg::TextureFormat::BGRA8; break;
				case GL_RGBA: default: fmt = bimg::TextureFormat::RGBA8; break;
				}
				if (bx::open(&writer, bx::FilePath(destinationPath.string().c_str())))
					bimg::imageWritePng(&writer, width, height, width * lengthModifier
						, data, fmt, false);

				//bimg::imageWritePng()

			}
		}
		const std::chrono::time_point<std::chrono::steady_clock> stop = std::chrono::high_resolution_clock::now();
		OutputDebug("Hashing of %dx%d*%d took %lfms\n", width, height,
			lengthModifier,
			std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() / 1e6);
	}
#endif
	return static_cast<void* (__stdcall*)(GLenum, GLint, GLint, GLsizei, GLsizei,
	                                      GLint, GLenum, GLenum, const void*)>(ogl_tex_image2d)
		(target, level, internalformat, width, height
			, border, format, type, data);
}


BYTE* cltBackAdd1;
BYTE* cltBackAdd2;
DWORD* _thisFF8 = 0; //__thiscall, so this is ECX



const char* GetTextureType(const int texType)
{
	static char textureType[4]{0};
	switch (texType)
	{
	case 11:
		return "BATTLE";
	case 18:
		return "WORLDMAP";
	case 25:
		return "FIELDBG";
	case 35:
		return "BATTLECHARACTER";
	case 57:
		return "FIELDENTITY";
	default:
		sprintf(textureType, "%02d", texType % 100);
		return textureType;
	}
}

__declspec(naked) void _cltObtainTexHeader()
{
	__asm
	{
		MOV tex_header, ESI
		MOV gl_textures, EDI //gl_textures is actually tex_struct

		//ORIGINAL CODE
		PUSH ECX
		PUSH EDX //this one gets cleared after 1020
		PUSH UPLOADVRAM
		CALL GetAddress

		MOV ECX, EAX
		MOV EAX, OFFSET IMAGE_BASE
		MOV EAX, [EAX]
		ADD EAX, ECX //upload_texture_vram
		POP EDX
		POP ECX
		CALL EAX
			
		JMP cltBackAdd2
	}
}

void _cltVoid()
{
	int textureType = tex_struct[48];
	TEX_TYPE = textureType;
	int palette = tex_header[52];
	UINT unknownDword = tex_struct[65];
	BOOL bHaveHD = tex_struct[49];
	DWORD unknownCheckHD = tex_struct[47];
	DWORD tPage = tex_struct[50];



	char n[255];
	n[0] = '\0';
	if (textureType == 0)
		return;
	if (textureType == 18 && (tPage <= 15 || tPage>=28))
		return;
	if (textureType == 57) //field
		return;
	OutputDebug("\ncommon_load_texture: tex_type: %s, pal: %d, unk: %08x, bHaveHD: %s, Tpage: %d\n", GetTextureType(textureType), palette, unknownDword, bHaveHD > 0 ? "TRUE" : "FALSE", tPage);

	return;
}

__declspec(naked) void _cltObtainTexStructDebug()
{
	__asm
	{
		PUSH EAX
		PUSH EBX
		PUSH ECX
		PUSH EDX
		MOV tex_struct, EDX
		CALL _cltVoid

		POP EDX
		POP ECX
		POP EBX
		POP EAX

		//ORIGINAL CODE

		CMP dword ptr[edx + 0x0BC], 0
		JMP cltBackAdd1
	}
}

void ReplaceTextureFunction()
{
	OutputDebug("Applying texture patches...\n");
	if (BATTLE_CHARA)
	{
		OutputDebug("Applying BATTLE_CHARA patch\n");
		ApplyBattleCharacterPatch();
	}
	if (FIELD_ENTITY)
	{
		OutputDebug("Applying FIELD_ENTITY patch\n");
		ApplyFieldEntityPatch();
	}
	if (BATTLE_HOOK)
	{
		OutputDebug("BATTLE_HOOK- STARTING PATCHING\n");
		ApplyBattleHookPatch();
		ApplyBattleMonsterPatch();
		ApplyBattleFieldPatch();
	}
	if (FIELD_BACKGROUND)
	{
		OutputDebug("Applying FIELD_BACKGROUND PATCH\n");
		ApplyFieldBackgroundPatch();
	}
	if (WORLD_TEXTURES)
	{
		OutputDebug("Applying WORLD_TEXTURES PATCH\n");
		ApplyWorldPatch();
	}

	cltBackAdd2 = InjectJMP(IMAGE_BASE + GetAddress(CLTBACKADD1), (DWORD)_cltObtainTexHeader, 5);
	cltBackAdd1 = InjectJMP(IMAGE_BASE + GetAddress(CLTBACKADD2), (DWORD)_cltObtainTexStructDebug, 7);
}
