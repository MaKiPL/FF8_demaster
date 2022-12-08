#pragma once

#include <map>

#define DEBUGOUT TRUE
#define HASH_FEATURE TRUE
#define HASH_FEATURE_SAVE TRUE

#define HASH_EXTENSION ".png"
#define HASH_HD_NAME "_HD" HASH_EXTENSION

enum HashExt
{
	png,
	dds
};


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