#pragma once

#include <d3d9.h>
#include <string>

struct TextureParameter
{
	TextureParameter() :
		texture(NULL),
		hasTextureSampler0(FALSE),
		hasTextureSampler1(FALSE),
		hasTextureSampler2(FALSE),
		hasAlphaTexture(FALSE) {}

	IDirect3DTexture9* texture;
	bool hasTextureSampler0;
	bool hasTextureSampler1;
	bool hasTextureSampler2;
	bool hasAlphaTexture;
	std::string textureName;
	std::string textureMemoryName;
};