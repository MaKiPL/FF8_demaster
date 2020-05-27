#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>
#include <bx/math.h>
#include <bx/bx.h>
#include <bx/allocator.h>
#include <bx/file.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bimg/encode.h>

/*
* PARTS OF THIS CODE IS EXTRACTED FROM https://github.com/bkaradzic/bgfx/blob/master/src/renderer_gl.h
* AND https://github.com/bkaradzic/bgfx/blob/master/src/renderer_gl.cpp AS WE DON'T NEED THE ENTIRE
* BGFX LIBRARY WE USE ONLY THE BIT WE REQUIRE TO RENDER DIRECTLY IN OPENGL.
* BGFX IS LICENSED WITH A BSD 2-clause License ( Copyright 2010-2020 Branimir Karadzic ).
*/

#ifndef GL_ETC1_RGB8_OES
#	define GL_ETC1_RGB8_OES 0x8D64
#endif // GL_ETC1_RGB8_OES

#ifndef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
#	define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG 0x8C01
#endif // GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG

#ifndef GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
#	define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 0x8C03
#endif // GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG

#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#	define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#endif // GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG

#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#	define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif // GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG

#ifndef GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG
#	define GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG 0x9137
#endif // GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG

#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG
#	define GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG 0x9138
#endif // GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG

#ifndef GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT
#	define GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT 0x8A54
#endif // GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT

#ifndef GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT
#	define GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT 0x8A55
#endif // GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT

#ifndef GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT
#	define GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT 0x8A56
#endif // GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT

#ifndef GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT
#	define GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT 0x8A57
#endif // GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT

#ifndef GL_COMPRESSED_SRGB8_ASTC_4x4_KHR
#   define GL_COMPRESSED_SRGB8_ASTC_4x4_KHR 0x93D0
#endif

#ifndef GL_COMPRESSED_SRGB8_ASTC_5x5_KHR
#   define GL_COMPRESSED_SRGB8_ASTC_5x5_KHR 0x93D2
#endif

#ifndef GL_COMPRESSED_SRGB8_ASTC_6x6_KHR
#   define GL_COMPRESSED_SRGB8_ASTC_6x6_KHR 0x93D4
#endif

#ifndef GL_COMPRESSED_SRGB8_ASTC_8x5_KHR
#   define GL_COMPRESSED_SRGB8_ASTC_8x5_KHR 0x93D5
#endif

#ifndef GL_COMPRESSED_SRGB8_ASTC_8x6_KHR
#   define GL_COMPRESSED_SRGB8_ASTC_8x6_KHR 0x93D6
#endif

#ifndef GL_COMPRESSED_SRGB8_ASTC_10x5_KHR
#   define GL_COMPRESSED_SRGB8_ASTC_10x5_KHR 0x93D8
#endif

struct TextureFormatInfo
{
	GLenum m_internalFmt;
	GLenum m_internalFmtSrgb;
	GLenum m_fmt;
	GLenum m_fmtSrgb;
	GLenum m_type;
	bool m_supported;
};

static TextureFormatInfo s_textureFormat[] =
{
	{ GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,            GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,            GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,            GL_ZERO,                         false }, // BC1
	{ GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,            GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT,        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,            GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,            GL_ZERO,                         false }, // BC2
	{ GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,            GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,            GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,            GL_ZERO,                         false }, // BC3
	{ GL_COMPRESSED_LUMINANCE_LATC1_EXT,           GL_ZERO,                                       GL_COMPRESSED_LUMINANCE_LATC1_EXT,           GL_COMPRESSED_LUMINANCE_LATC1_EXT,           GL_ZERO,                         false }, // BC4
	{ GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,     GL_ZERO,                                       GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,     GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT,     GL_ZERO,                         false }, // BC5
	{ GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,     GL_ZERO,                                       GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,     GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,     GL_ZERO,                         false }, // BC6H
	{ GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,           GL_ZERO,                                       GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,           GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,           GL_ZERO,                         false }, // BC7
	{ GL_ETC1_RGB8_OES,                            GL_ZERO,                                       GL_ETC1_RGB8_OES,                            GL_ETC1_RGB8_OES,                            GL_ZERO,                         false }, // ETC1
	{ GL_COMPRESSED_RGB8_ETC2,                     GL_ZERO,                                       GL_COMPRESSED_RGB8_ETC2,                     GL_COMPRESSED_RGB8_ETC2,                     GL_ZERO,                         false }, // ETC2
	{ GL_COMPRESSED_RGBA8_ETC2_EAC,                GL_COMPRESSED_SRGB8_ETC2,                      GL_COMPRESSED_RGBA8_ETC2_EAC,                GL_COMPRESSED_RGBA8_ETC2_EAC,                GL_ZERO,                         false }, // ETC2A
	{ GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2, GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,  GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2, GL_ZERO,                         false }, // ETC2A1
	{ GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,          GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT,           GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,          GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,          GL_ZERO,                         false }, // PTC12
	{ GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,          GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT,           GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,          GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,          GL_ZERO,                         false }, // PTC14
	{ GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,         GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT,     GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,         GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,         GL_ZERO,                         false }, // PTC12A
	{ GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,         GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT,     GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,         GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,         GL_ZERO,                         false }, // PTC14A
	{ GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG,         GL_ZERO,                                       GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG,         GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG,         GL_ZERO,                         false }, // PTC22
	{ GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG,         GL_ZERO,                                       GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG,         GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG,         GL_ZERO,                         false }, // PTC24
	{ GL_ATC_RGB_AMD,                              GL_ZERO,                                       GL_ATC_RGB_AMD,                              GL_ATC_RGB_AMD,                              GL_ZERO,                         false }, // ATC
	{ GL_ATC_RGBA_EXPLICIT_ALPHA_AMD,              GL_ZERO,                                       GL_ATC_RGBA_EXPLICIT_ALPHA_AMD,              GL_ATC_RGBA_EXPLICIT_ALPHA_AMD,              GL_ZERO,                         false }, // ATCE
	{ GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,          GL_ZERO,                                       GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,          GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,          GL_ZERO,                         false }, // ATCI
	{ GL_COMPRESSED_RGBA_ASTC_4x4_KHR,             GL_COMPRESSED_SRGB8_ASTC_4x4_KHR,              GL_COMPRESSED_RGBA_ASTC_4x4_KHR,             GL_COMPRESSED_RGBA_ASTC_4x4_KHR,             GL_ZERO,                         false }, // ASTC4x4
	{ GL_COMPRESSED_RGBA_ASTC_5x5_KHR,             GL_COMPRESSED_SRGB8_ASTC_5x5_KHR,              GL_COMPRESSED_RGBA_ASTC_5x5_KHR,             GL_COMPRESSED_RGBA_ASTC_5x5_KHR,             GL_ZERO,                         false }, // ASTC5x5
	{ GL_COMPRESSED_RGBA_ASTC_6x6_KHR,             GL_COMPRESSED_SRGB8_ASTC_6x6_KHR,              GL_COMPRESSED_RGBA_ASTC_6x6_KHR,             GL_COMPRESSED_RGBA_ASTC_6x6_KHR,             GL_ZERO,                         false }, // ASTC6x6
	{ GL_COMPRESSED_RGBA_ASTC_8x5_KHR,             GL_COMPRESSED_SRGB8_ASTC_8x5_KHR,              GL_COMPRESSED_RGBA_ASTC_8x5_KHR,             GL_COMPRESSED_RGBA_ASTC_8x5_KHR,             GL_ZERO,                         false }, // ASTC8x5
	{ GL_COMPRESSED_RGBA_ASTC_8x6_KHR,             GL_COMPRESSED_SRGB8_ASTC_8x6_KHR,              GL_COMPRESSED_RGBA_ASTC_8x6_KHR,             GL_COMPRESSED_RGBA_ASTC_8x6_KHR,             GL_ZERO,                         false }, // ASTC8x6
	{ GL_COMPRESSED_RGBA_ASTC_10x5_KHR,            GL_COMPRESSED_SRGB8_ASTC_10x5_KHR,             GL_COMPRESSED_RGBA_ASTC_10x5_KHR,            GL_COMPRESSED_RGBA_ASTC_10x5_KHR,            GL_ZERO,                         false }, // ASTC10x5
	{ GL_ZERO,                                     GL_ZERO,                                       GL_ZERO,                                     GL_ZERO,                                     GL_ZERO,                         false }, // Unknown
	{ GL_ZERO,                                     GL_ZERO,                                       GL_ZERO,                                     GL_ZERO,                                     GL_ZERO,                         false }, // R1
	{ GL_ALPHA,                                    GL_ZERO,                                       GL_ALPHA,                                    GL_ALPHA,                                    GL_UNSIGNED_BYTE,                false }, // A8
	{ GL_R8,                                       GL_ZERO,                                       GL_RED,                                      GL_RED,                                      GL_UNSIGNED_BYTE,                false }, // R8
	{ GL_R8I,                                      GL_ZERO,                                       GL_RED_INTEGER,                              GL_RED_INTEGER,                              GL_BYTE,                         false }, // R8I
	{ GL_R8UI,                                     GL_ZERO,                                       GL_RED_INTEGER,                              GL_RED_INTEGER,                              GL_UNSIGNED_BYTE,                false }, // R8U
	{ GL_R8_SNORM,                                 GL_ZERO,                                       GL_RED,                                      GL_RED,                                      GL_BYTE,                         false }, // R8S
	{ GL_R16,                                      GL_ZERO,                                       GL_RED,                                      GL_RED,                                      GL_UNSIGNED_SHORT,               false }, // R16
	{ GL_R16I,                                     GL_ZERO,                                       GL_RED_INTEGER,                              GL_RED_INTEGER,                              GL_SHORT,                        false }, // R16I
	{ GL_R16UI,                                    GL_ZERO,                                       GL_RED_INTEGER,                              GL_RED_INTEGER,                              GL_UNSIGNED_SHORT,               false }, // R16U
	{ GL_R16F,                                     GL_ZERO,                                       GL_RED,                                      GL_RED,                                      GL_HALF_FLOAT,                   false }, // R16F
	{ GL_R16_SNORM,                                GL_ZERO,                                       GL_RED,                                      GL_RED,                                      GL_SHORT,                        false }, // R16S
	{ GL_R32I,                                     GL_ZERO,                                       GL_RED_INTEGER,                              GL_RED_INTEGER,                              GL_INT,                          false }, // R32I
	{ GL_R32UI,                                    GL_ZERO,                                       GL_RED_INTEGER,                              GL_RED_INTEGER,                              GL_UNSIGNED_INT,                 false }, // R32U
	{ GL_R32F,                                     GL_ZERO,                                       GL_RED,                                      GL_RED,                                      GL_FLOAT,                        false }, // R32F
	{ GL_RG8,                                      GL_ZERO,                                       GL_RG,                                       GL_RG,                                       GL_UNSIGNED_BYTE,                false }, // RG8
	{ GL_RG8I,                                     GL_ZERO,                                       GL_RG_INTEGER,                               GL_RG_INTEGER,                               GL_BYTE,                         false }, // RG8I
	{ GL_RG8UI,                                    GL_ZERO,                                       GL_RG_INTEGER,                               GL_RG_INTEGER,                               GL_UNSIGNED_BYTE,                false }, // RG8U
	{ GL_RG8_SNORM,                                GL_ZERO,                                       GL_RG,                                       GL_RG,                                       GL_BYTE,                         false }, // RG8S
	{ GL_RG16,                                     GL_ZERO,                                       GL_RG,                                       GL_RG,                                       GL_UNSIGNED_SHORT,               false }, // RG16
	{ GL_RG16I,                                    GL_ZERO,                                       GL_RG_INTEGER,                               GL_RG_INTEGER,                               GL_SHORT,                        false }, // RG16I
	{ GL_RG16UI,                                   GL_ZERO,                                       GL_RG_INTEGER,                               GL_RG_INTEGER,                               GL_UNSIGNED_SHORT,               false }, // RG16U
	{ GL_RG16F,                                    GL_ZERO,                                       GL_RG,                                       GL_RG,                                       GL_FLOAT,                        false }, // RG16F
	{ GL_RG16_SNORM,                               GL_ZERO,                                       GL_RG,                                       GL_RG,                                       GL_SHORT,                        false }, // RG16S
	{ GL_RG32I,                                    GL_ZERO,                                       GL_RG_INTEGER,                               GL_RG_INTEGER,                               GL_INT,                          false }, // RG32I
	{ GL_RG32UI,                                   GL_ZERO,                                       GL_RG_INTEGER,                               GL_RG_INTEGER,                               GL_UNSIGNED_INT,                 false }, // RG32U
	{ GL_RG32F,                                    GL_ZERO,                                       GL_RG,                                       GL_RG,                                       GL_FLOAT,                        false }, // RG32F
	{ GL_RGB8,                                     GL_SRGB8,                                      GL_RGB,                                      GL_RGB,                                      GL_UNSIGNED_BYTE,                false }, // RGB8
	{ GL_RGB8I,                                    GL_ZERO,                                       GL_RGB_INTEGER,                              GL_RGB_INTEGER,                              GL_BYTE,                         false }, // RGB8I
	{ GL_RGB8UI,                                   GL_ZERO,                                       GL_RGB_INTEGER,                              GL_RGB_INTEGER,                              GL_UNSIGNED_BYTE,                false }, // RGB8U
	{ GL_RGB8_SNORM,                               GL_ZERO,                                       GL_RGB,                                      GL_RGB,                                      GL_BYTE,                         false }, // RGB8S
	{ GL_RGB9_E5,                                  GL_ZERO,                                       GL_RGB,                                      GL_RGB,                                      GL_UNSIGNED_INT_5_9_9_9_REV,     false }, // RGB9E5F
	{ GL_RGBA8,                                    GL_SRGB8_ALPHA8,                               GL_BGRA,                                     GL_BGRA,                                     GL_UNSIGNED_BYTE,                false }, // BGRA8
	{ GL_RGBA8,                                    GL_SRGB8_ALPHA8,                               GL_RGBA,                                     GL_RGBA,                                     GL_UNSIGNED_BYTE,                false }, // RGBA8
	{ GL_RGBA8I,                                   GL_ZERO,                                       GL_RGBA_INTEGER,                             GL_RGBA_INTEGER,                             GL_BYTE,                         false }, // RGBA8I
	{ GL_RGBA8UI,                                  GL_ZERO,                                       GL_RGBA_INTEGER,                             GL_RGBA_INTEGER,                             GL_UNSIGNED_BYTE,                false }, // RGBA8U
	{ GL_RGBA8_SNORM,                              GL_ZERO,                                       GL_RGBA,                                     GL_RGBA,                                     GL_BYTE,                         false }, // RGBA8S
	{ GL_RGBA16,                                   GL_ZERO,                                       GL_RGBA,                                     GL_RGBA,                                     GL_UNSIGNED_SHORT,               false }, // RGBA16
	{ GL_RGBA16I,                                  GL_ZERO,                                       GL_RGBA_INTEGER,                             GL_RGBA_INTEGER,                             GL_SHORT,                        false }, // RGBA16I
	{ GL_RGBA16UI,                                 GL_ZERO,                                       GL_RGBA_INTEGER,                             GL_RGBA_INTEGER,                             GL_UNSIGNED_SHORT,               false }, // RGBA16U
	{ GL_RGBA16F,                                  GL_ZERO,                                       GL_RGBA,                                     GL_RGBA,                                     GL_HALF_FLOAT,                   false }, // RGBA16F
	{ GL_RGBA16_SNORM,                             GL_ZERO,                                       GL_RGBA,                                     GL_RGBA,                                     GL_SHORT,                        false }, // RGBA16S
	{ GL_RGBA32I,                                  GL_ZERO,                                       GL_RGBA_INTEGER,                             GL_RGBA_INTEGER,                             GL_INT,                          false }, // RGBA32I
	{ GL_RGBA32UI,                                 GL_ZERO,                                       GL_RGBA_INTEGER,                             GL_RGBA_INTEGER,                             GL_UNSIGNED_INT,                 false }, // RGBA32U
	{ GL_RGBA32F,                                  GL_ZERO,                                       GL_RGBA,                                     GL_RGBA,                                     GL_FLOAT,                        false }, // RGBA32F
	{ GL_RGB565,                                   GL_ZERO,                                       GL_RGB,                                      GL_RGB,                                      GL_UNSIGNED_SHORT_5_6_5,         false }, // R5G6B5
	{ GL_RGBA4,                                    GL_ZERO,                                       GL_BGRA,                                     GL_BGRA,                                     GL_UNSIGNED_SHORT_4_4_4_4_REV,   false }, // RGBA4
	{ GL_RGB5_A1,                                  GL_ZERO,                                       GL_BGRA,                                     GL_BGRA,                                     GL_UNSIGNED_SHORT_1_5_5_5_REV,   false }, // RGB5A1
	{ GL_RGB10_A2,                                 GL_ZERO,                                       GL_RGBA,                                     GL_RGBA,                                     GL_UNSIGNED_INT_2_10_10_10_REV,  false }, // RGB10A2
	{ GL_R11F_G11F_B10F,                           GL_ZERO,                                       GL_RGB,                                      GL_RGB,                                      GL_UNSIGNED_INT_10F_11F_11F_REV, false }, // RG11B10F
	{ GL_ZERO,                                     GL_ZERO,                                       GL_ZERO,                                     GL_ZERO,                                     GL_ZERO,                         false }, // UnknownDepth
	{ GL_DEPTH_COMPONENT16,                        GL_ZERO,                                       GL_DEPTH_COMPONENT,                          GL_DEPTH_COMPONENT,                          GL_UNSIGNED_SHORT,               false }, // D16
	{ GL_DEPTH_COMPONENT24,                        GL_ZERO,                                       GL_DEPTH_COMPONENT,                          GL_DEPTH_COMPONENT,                          GL_UNSIGNED_INT,                 false }, // D24
	{ GL_DEPTH24_STENCIL8,                         GL_ZERO,                                       GL_DEPTH_STENCIL,                            GL_DEPTH_STENCIL,                            GL_UNSIGNED_INT_24_8,            false }, // D24S8
	{ GL_DEPTH_COMPONENT32,                        GL_ZERO,                                       GL_DEPTH_COMPONENT,                          GL_DEPTH_COMPONENT,                          GL_UNSIGNED_INT,                 false }, // D32
	{ GL_DEPTH_COMPONENT32F,                       GL_ZERO,                                       GL_DEPTH_COMPONENT,                          GL_DEPTH_COMPONENT,                          GL_FLOAT,                        false }, // D16F
	{ GL_DEPTH_COMPONENT32F,                       GL_ZERO,                                       GL_DEPTH_COMPONENT,                          GL_DEPTH_COMPONENT,                          GL_FLOAT,                        false }, // D24F
	{ GL_DEPTH_COMPONENT32F,                       GL_ZERO,                                       GL_DEPTH_COMPONENT,                          GL_DEPTH_COMPONENT,                          GL_FLOAT,                        false }, // D32F
	{ GL_STENCIL_INDEX8,                           GL_ZERO,                                       GL_STENCIL_INDEX,                            GL_STENCIL_INDEX,                            GL_UNSIGNED_BYTE,                false }, // D0S8
};
