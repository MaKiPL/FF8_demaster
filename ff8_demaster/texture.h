#pragma once

#define DEBUGOUT TRUE
#define HASH_FEATURE FALSE

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
struct texImageInformation
{
	uint64_t lowAdditionalCheck;
	GLuint id;
	GLint internalformat;
	GLsizei width, height;
};
static  std::map<uint64_t, texImageInformation> knownTextures;
void* __stdcall HookGlTexImage2D(GLenum target,
	GLint level,
	GLint internalformat,
	GLsizei width,
	GLsizei height,
	GLint border,
	GLenum format,
	GLenum type,
	const void* data);