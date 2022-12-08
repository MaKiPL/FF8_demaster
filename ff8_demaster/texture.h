#pragma once

#include <map>
#include <string>
#include <vector>
#include "config.h"


inline const char* hashExtensions[] = { ".png", ".dds" };
inline const char* GetHashExtension(const bool bIsSaving)
{
	if(HASH_OUTPUT_EXT!= 0 && bIsSaving)
		OutputDebug("\nOnly PNG is supported for output so far. Saving as PNG\n");
	return hashExtensions[bIsSaving ? /*HASH_OUTPUT_EXT*/ 0 : HASH_LOAD_HD_EXT];
}
#define HASH_HD_SUFFIX "_HD"

struct Vector2Di
{
	uint32_t width;
	uint32_t height;
};

Vector2Di GetImageResolutionFast(const char* filePath);
void ReadPNGHeaderResolutionFast(std::istream& stream, Vector2Di& resolution);
void ReadDDSHeaderResolutionFast(std::istream& stream, Vector2Di& resolution);

#define MAGIC_PNG 0x474E5089
#define MAGIC_DDS 0x20534444


/*
11 - BATTLE MONSTER + SCENERY
18 - WORLD MAP TEXTURE
25 - FIELD BACKGROUND [DONE]
35 - BATTLE CHARACTER PLAYER [DONE]
57 - FIELD ENTITY [DONE]
OTHER - SYSTEM, MENU, PROLOGUE etc. [AS-IS]
*/

/**
 * \brief Base function that implements hooks and hacks for texture replacement
 */
void ReplaceTextureFunction();


inline LPVOID ogl_bind_texture;
void* __stdcall HookGlBindTexture(GLenum target, GLuint texture);

inline LPVOID ogl_tex_parametri;
void* __stdcall HookGlTextParameteri(GLenum target, GLenum name, GLint param);

inline LPVOID ogl_tex_image2d;
struct TexImageInformation
{
	uint64_t lowAdditionalCheck;
	GLuint id;
	GLint internalformat;
	GLsizei width, height;
};

inline std::map<uint64_t, TexImageInformation> knownTextures;
void* __stdcall HookGlTexImage2D(GLenum target,
	GLint level,
	GLint internalformat,
	GLsizei width,
	GLsizei height,
	GLint border,
	GLenum format,
	GLenum type,
	const void* data);