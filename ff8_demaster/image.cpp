#include "image.h"

#include <filesystem>
#include <fstream>
#include "debug.h"
#include <bimg/bimg.h>

#include "coreHeader.h"
#include "opengl.h"

SafeBimg SafeBimgInit(bimg::ImageContainer* img) { return {img, bimg::imageFree}; }

SafeBimg LoadImageFromFile(const char* const filename)
{
	CheckGlew();
	char msg[1024]{0};
	OutputDebug("%s - Opening File: %s\n", __func__, filename);
	
	std::fstream fp = std::fstream(filename, std::ios::in | std::ios::binary);
	if (!fp.is_open())
		return SafeBimgInit();

	fp.seekg(0, std::ios::end);
	const std::streampos filesize = fp.tellg();
	const std::unique_ptr<char[]> buffer = std::make_unique<char[]>(filesize);

	if (!buffer)
		return SafeBimgInit();

	fp.seekg(0, std::ios::beg);
	fp.read(buffer.get(), filesize);

	// REQUIRED BY BIMG FILE DECODING
	static bx::DefaultAllocator texAllocator{};
	bx::Error error{};
	constexpr static bimg::TextureFormat::Enum format{bimg::TextureFormat::Enum::Count};
	bimg::ImageContainer* img = bimg::imageParse(&texAllocator, buffer.get(), static_cast<uint32_t>(filesize)
		,format, &error);
	
	if (error.isOk())
		return SafeBimgInit(img);
	
	OutputDebug("%s::%d::bimg error: %s ", __func__, __LINE__, error.getMessage().getPtr());
	return SafeBimgInit();
}

//appends DDS checks if file exists and then checks for PNG. returns false if atleast one exists. true on failure.
bool DDSorPNG(char* buffer, const size_t inSize, const char* fmt, ...)
{
	const size_t size = inSize - 4U; //for extension
	va_list args;
	va_start(args, fmt);
	int argLength = vsnprintf(buffer, size, fmt, args);
	strcat(buffer, ".dds");
	va_end(args);
	if (!std::filesystem::exists(buffer))
	{
		va_start(args, fmt);
		vsnprintf(buffer, size, fmt, args);
		strcat(buffer, ".png");
		va_end(args);
		return !std::filesystem::exists(buffer); //if it exist, then it should return false on success, right?
	}
	return false;
}
void RenderTexture(const bimg::ImageContainer* img)
{
	const TextureFormatInfo& texInfo = s_textureFormat[img->m_format];
	if (isCompressed(img->m_format))
		RenderCompressedTexture(img, texInfo);
	else
		RenderUncompressedTexture(img, texInfo);
}

void RenderUncompressedTexture(const bimg::ImageContainer* img, const TextureFormatInfo& texInfo)
{
	CheckGlew();
	uint32_t width = img->m_width;
	uint32_t height = img->m_height;
	uint32_t depth = img->m_depth;
	const uint8_t startLod = bx::min<uint8_t>(0, img->m_numMips - 1);

	for (uint8_t lod = 0, num = img->m_numMips; lod < num; ++lod)
	{
		width = bx::uint32_max(1, width);
		height = bx::uint32_max(1, height);

		if (bimg::ImageMip mip{}; imageGetRawData(*img, 0, lod + startLod, img->m_data, img->m_size, mip))
			glTexImage2D(GL_TEXTURE_2D, lod, static_cast<GLint>(texInfo.m_internalFmt)
				, static_cast<GLsizei>(img->m_width), static_cast<GLsizei>(img->m_height), 0
				, texInfo.m_fmt, texInfo.m_type, mip.m_data);
	}
}

void RenderCompressedTexture(const bimg::ImageContainer* img, const TextureFormatInfo& texInfo)
{
	if (!GLEW_ARB_texture_compression)
	{
		OutputDebug("Texture is compressed, but compression is not supported on your GPU. Skipping draw.");
		return;
	}
	uint32_t width = img->m_width;
	uint32_t height = img->m_height;
	const uint8_t startLod = bx::min<uint8_t>(0, img->m_numMips - 1);

	for (uint8_t lod = 0, num = img->m_numMips; lod < num; ++lod)
	{
		width = bx::uint32_max(1, width);
		height = bx::uint32_max(1, height);

		if (bimg::ImageMip mip{}; bimg::imageGetRawData(*img, 0, lod + startLod, img->m_data, img->m_size, mip))
			glCompressedTexImage2D(GL_TEXTURE_2D, lod, texInfo.m_internalFmt, static_cast<GLsizei>(img->m_width)
			                       , static_cast<GLsizei>(img->m_height), 0, static_cast<GLsizei>(mip.m_size),
			                       mip.m_data);
	}
}

void LoadAndRenderTexture(const char* const filename, const bool bAppendPath)
{
	std::string localPath;
	if (bAppendPath)
	{
		localPath = DIRECT_IO_EXPORT_DIR;
		localPath.append(filename);
	}
	else
		localPath = filename;

	const SafeBimg img = LoadImageFromFile(localPath.c_str());
	RenderTexture(img.get());
}

