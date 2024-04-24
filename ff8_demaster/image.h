#pragma once
#include <memory>
#include <bimg/bimg.h>
#include "renderer.h"

typedef std::unique_ptr<bimg::ImageContainer, decltype(&bimg::imageFree)> SafeBimg;
SafeBimg SafeBimgInit(bimg::ImageContainer* img = nullptr);
SafeBimg LoadImageFromFile(const char* const filename);

bool DDSorPNG(char* buffer, size_t size, const char* fmt, ...);

void RenderTexture(const bimg::ImageContainer* img);
void RenderUncompressedTexture(const bimg::ImageContainer* img, const TextureFormatInfo& texInfo);
void RenderCompressedTexture(const bimg::ImageContainer* img, const TextureFormatInfo& texInfo);
void LoadAndRenderTexture(const char* const filename);