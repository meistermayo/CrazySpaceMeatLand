#include "Texture.h"
#include "d3dUtil.h"
#include "DirectXTex.h"
#include <assert.h>
#include "GraphicsBackend.h"

Texture::Texture(std::string filepath)
{
	mpTextureSampler = new TextureSampler();
	mpTextureSampler->LoadTexture(filepath);

	mpTextureSampler->CreateSampleState();
}

Texture::Texture(std::string filepath, uint32_t filter, uint32_t isotropic_level, bool ComputeMip, size_t miplevel, uint32_t filterflags)
{
	mpTextureSampler = new TextureSampler();
	mpTextureSampler->LoadTexture(filepath, ComputeMip, miplevel, filterflags);

	mpTextureSampler->CreateSampleState(filter, isotropic_level);
}

Texture::~Texture()
{
	delete mpTextureSampler;
}

Texture::Texture(float r, float g, float b, float a)
{
	r; g; b; a; 
	// create red image :) todo
}

void Texture::SetToContext(int texResSlot,  int sampSlot)
{
	mpTextureSampler->SetToContext(texResSlot, sampSlot);
}

