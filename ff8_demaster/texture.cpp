// ReSharper disable CppUseStructuredBinding
#include "coreHeader.h"
#include "texture.h"
#include <chrono>
#include <filesystem>
#define XXH_INLINE_ALL
#include <fstream>
#include <xxhash.h>

#include "config.h"
#include "opengl.h"


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

struct loadedTextureInformation
{
	void* data;
	uint32_t length, width, height;
};

std::map<uint64_t, loadedTextureInformation> loadedTextures;


//below could use splitting into multiple functions
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
	//OutputDebug("glTexImage2D (%d): format: %d, %dx%d, void: %08x\n",boundId, internalformat, width, height, data);
	int lengthModifier = 0;
	if (internalformat == GL_RGBA || internalformat == GL_BGRA
		|| internalformat == GL_RGBA8)
		lengthModifier = 4;
	else if (internalformat == GL_RGB || internalformat == GL_BGR
		|| internalformat == GL_RGB8)
		lengthModifier = 3;
if(HASH_ENABLED) //=======HASHING====//
{
	if (data != nullptr && width != 0 && height != 0 && lengthModifier != 0
		&& width < 1024 && height < 1024)
	{
		uint64_t hashCopy = 0;
		const std::chrono::time_point<std::chrono::steady_clock> start =
			std::chrono::high_resolution_clock::now();
		//XXH32_hash_t hash = XXH32(data, width * height * lengthModifier, 0x85EBCA77U); //32ms
		int hashLength = width*height*lengthModifier;
		if(currentMode == Modes::MODE_WORLDMAP)
			if(width==256 && height == 256)
				hashLength = 256*192*lengthModifier;
		const auto [low64, high64] = XXH128(data, hashLength, 0x85EBCA77U);
		hashCopy = high64;
		std::filesystem::path destinationPath = std::filesystem::current_path();
		destinationPath.append(std::string(DIRECT_IO_EXPORT_DIR)
			+ "hashOutput");
		if (!std::filesystem::exists(destinationPath))
			std::filesystem::create_directory(destinationPath);
		std::stringstream ss;
		ss << std::setw(16) << std::setfill('0') << std::hex
			<< std::uppercase << high64 << std::dec << std::nouppercase;
		destinationPath.append(ss.str());
		if (!knownTextures.contains(high64))
		{
			OutputDebug("New hash for textureId: %u. Hash: %016llX%016llX ",
				boundId, high64, low64);
			knownTextures.insert(std::pair(high64, TexImageInformation{
							low64,static_cast<GLuint>(boundId), internalformat, width, height }));
if(HASH_OUTPUT) //======OUTPUT OF HASHED TEXTURES======//
{
	std::string exportPath = std::string(destinationPath.string());
	exportPath.append(GetHashExtension(true));
	if (!std::filesystem::exists(exportPath))
	{
		bx::FileWriter writer;
		bimg::TextureFormat::Enum fmt = bimg::TextureFormat::RGBA8;
		switch (internalformat)
		{
			case GL_RGB8: case GL_RGB: case GL_BGR: fmt = bimg::TextureFormat::BGRA8; break;
			case GL_BGRA: fmt = bimg::TextureFormat::BGRA8; break;
			case GL_RGBA: default: fmt = bimg::TextureFormat::BGRA8; break;
		}
		if (bx::open(&writer, bx::FilePath(exportPath.c_str())))
		{
			//refractor recommends switching to if/else for binary switch, however it's left here for future
			//if more formats are added
			/*switch(HASH_OUTPUT_EXT)
		   {
			   default:
				   OutputDebug("Only PNG is supported for now. Saving as PNG");
				   [[fallthrough]]
			   case 0:
				   bimg::imageWritePng(&writer, width, height, width * lengthModifier
			   , data, fmt, false);
			   break;
		   }*/
			bimg::imageWritePng(&writer, width, height, width * lengthModifier
			, data, fmt, false);
		}
	}
}
		}
		const std::chrono::time_point<std::chrono::steady_clock> stop = std::chrono::high_resolution_clock::now();
		// OutputDebug("Hashing of %dx%d*%d took %lfms\n", width, height,
		// 	lengthModifier,
		// 	static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count()) / 1e6);
		if(HASH_LOAD_HD) //=====HASH LOAD CODE=====//
		{
			std::string importPath = std::string(destinationPath.string());
			importPath.append(HASH_HD_SUFFIX);
			importPath.append(GetHashExtension(false));
			if (std::filesystem::exists(importPath))
			{
				if (loadedTextures.contains(hashCopy))
				{
					auto [dataLoaded, lengthLoaded, widthLoaded, heightLoaded]
						= loadedTextures[hashCopy];
					return static_cast<void* (__stdcall*)(GLenum, GLint, GLint, GLsizei, GLsizei,
														  GLint, GLenum, GLenum, const void*)>(ogl_tex_image2d)
						(target, level, internalformat, static_cast<GLsizei>(widthLoaded)
						 , static_cast<GLsizei>(heightLoaded), border, format, type, dataLoaded);
				}
				//Obtain file resolution
				if(Vector2Di resolution = GetImageResolutionFast(importPath.c_str());
					resolution.width != static_cast<uint32_t>(width)
					&& resolution.height != static_cast<uint32_t>(height)
					&& (resolution.width != 0 && resolution.height != 0))
				{
					SafeBimg img = LoadImageFromFile(importPath.c_str());
					if (const bimg::ImageContainer* imageContainer = img.get();
						imageContainer->m_data != nullptr)
					{
						OutputDebug("Loading custom HD texture: %s!", destinationPath.string().c_str());

						
						if(bimg::isCompressed(imageContainer->m_format))
						{
							RenderTexture(imageContainer); //redundant on checking two times for compression, but whatev
							//return cast to glTexImage2D but with error
							return static_cast<void * (__stdcall*) (GLenum, GLint, GLint, GLsizei, GLsizei,
								GLint, GLenum, GLenum, const void*)>(ogl_tex_image2d)
							(target, -1, internalformat, static_cast<GLsizei>(width)
								, static_cast<GLsizei>(height), border, format, type, data);
						}
						else
						{
							loadedTextureInformation lti{};
							lti.height = imageContainer->m_height;
							lti.width = imageContainer->m_width;
							lti.length = imageContainer->m_size;
							lti.data = malloc(lti.length);
							memcpy(lti.data, imageContainer->m_data, lti.length);
							//add lti to loadedTextures
							loadedTextures.emplace(hashCopy, lti);
							return static_cast<void* (__stdcall*)(GLenum, GLint, GLint, GLsizei, GLsizei,
																  GLint, GLenum, GLenum, const void*)>(ogl_tex_image2d)
								(target, level, internalformat, static_cast<int>(imageContainer->m_width)
								 , static_cast<int>(imageContainer->m_height), border, GL_RGBA/*format*/, type
								 , imageContainer->m_data);
						}
					}
				}
			}
		}
	}
}
	//no hashing
	serverInst.WriteLog(std::format("GLTexSubImage2D: ID: {} Width: {} Height: {}", GetCurrentBoundTextureID(), width, height));
	return static_cast<void* (__stdcall*)(GLenum, GLint, GLint, GLsizei, GLsizei,
	                                      GLint, GLenum, GLenum, const void*)>(ogl_tex_image2d)
		(target, level, internalformat, width, height
			, border, format, type, data);
}

void* __stdcall HookGlTexSubImage2D( 	GLenum target,
	  GLint level,
	  GLint xoffset,
	  GLint yoffset,
	  GLsizei width,
	  GLsizei height,
	  GLenum format,
	  GLenum type,
	  const void * pixels)
{
	OutputDebug("Hooked glTexSubImage2D width: %d height: %d\n", width, height);
	return static_cast<void* (__stdcall*)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei,
									  GLenum, GLenum, const void*)>(ogl_subTexImage2D)
	(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void* __stdcall HookGlTextureSubImage2D( 	GLuint texture,
	  GLint level,
	  GLint xoffset,
	  GLint yoffset,
	  GLsizei width,
	  GLsizei height,
	  GLenum format,
	  GLenum type,
	  const void *pixels)
{
	OutputDebug("Hooked glTextureSubImage2D width: %d height: %d\n", width, height);
	return static_cast<void* (__stdcall*)(GLuint, GLint, GLint, GLint, GLsizei, GLsizei,
								  GLenum, GLenum, const void*)>(ogl_subTexImage2D)
(texture, level, xoffset, yoffset, width, height, format, type, pixels);
}

//this is null sub- basically hooking api requires static cast to function pointer. However if there's need to
//actually call compressed texture function, this is the way to do it. I call the compressed texture method and
//then return the main texture2D method into this null function. The compressed texture and glTexImage2D are voids
//that don't return anything as they work under bindings, so compressed texture will utilize the bound texture
//while this null reroute will do nothing and not overwrite the texture
void NullHookGlTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
	GLenum format, GLenum type, const void* data)
{
}


BYTE* cltBackAdd1;
BYTE* cltBackAdd2;
DWORD* _thisFF8 = nullptr; //__thiscall, so this is ECX



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
	const int textureType = tex_struct[48];
	TEX_TYPE = textureType;
	const int palette = tex_header[52];
	const UINT unknownDword = tex_struct[65];
	const BOOL bHaveHD = tex_struct[49];
	const DWORD tPage = tex_struct[50];



	char n[255];
	n[0] = '\0';
	if (textureType == 0)
		return;
	if (textureType == 18 && (tPage <= 15 || tPage>=28))
		return;
	if (textureType == 57) //field
		return;
	//OutputDebug("\ncommon_load_texture: tex_type: %s, pal: %d, unk: %08x, bHaveHD: %s, Tpage: %d\n", GetTextureType(textureType), palette, unknownDword, bHaveHD > 0 ? "TRUE" : "FALSE", tPage);

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

void ReadPNGHeaderResolutionFast(std::istream& stream, Vector2Di &resolution)
{
	stream.seekg(8, std::ios::cur);
	uint32_t ihdrMagic = 0;
	stream.read(reinterpret_cast<char*>(&ihdrMagic), sizeof(uint32_t));
	if (ihdrMagic != 0x52444849)
	{
		OutputDebug("Invalid PNG file!");
		return;
	}
	stream.read(reinterpret_cast<char*>(&resolution.width), sizeof(uint32_t));
	stream.read(reinterpret_cast<char*>(&resolution.height), sizeof(uint32_t));
	resolution.height = _byteswap_ulong(resolution.height);
	resolution.width = _byteswap_ulong(resolution.width);
}

void ReadDDSHeaderResolutionFast(std::istream& stream, Vector2Di& resolution)
{
	uint32_t headerSize = 0;
	stream.read(reinterpret_cast<char*>(&headerSize), sizeof(uint32_t));
	if (headerSize != 124)
	{
		OutputDebug("Invalid DDS file!");
		return;
	}
	stream.seekg(4, std::ios::cur);
	stream.read(reinterpret_cast<char*>(&resolution.width), sizeof(uint32_t));
	stream.read(reinterpret_cast<char*>(&resolution.height), sizeof(uint32_t));
}



Vector2Di GetImageResolutionFast(const char* filePath)
{
	Vector2Di resolution{};
	
	std::ifstream inFile;
	inFile.open(filePath, std::ios::in | std::ios::binary);
	if(!inFile.is_open())
		{OutputDebug("%s error. File %s not opened\n", __func__, filePath); return resolution;}
	uint32_t magic = 0;
	inFile.read(reinterpret_cast<char*>(&magic), sizeof(uint32_t));
	switch(magic)
	{
		case MAGIC_PNG:
			ReadPNGHeaderResolutionFast(inFile, resolution);
			break;
		case MAGIC_DDS:
			ReadDDSHeaderResolutionFast(inFile,resolution);
			break;
		default:
			OutputDebug("%s error. Unknown file format- only DDS and PNG are supported. I'm too lazy\n", __func__);
	}
	inFile.close();
	OutputDebug("%s: %s resolution: %dx%d\n", __func__, filePath, resolution.width, resolution.height);
	return resolution;
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

	cltBackAdd2 = InjectJMP(IMAGE_BASE + GetAddress(CLTBACKADD1), reinterpret_cast<DWORD>(_cltObtainTexHeader), 5);
	cltBackAdd1 = InjectJMP(IMAGE_BASE + GetAddress(CLTBACKADD2), reinterpret_cast<DWORD>(_cltObtainTexStructDebug), 7);
}
