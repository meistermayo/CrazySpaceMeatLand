#include "GraphicsBackend.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Model.h"

GraphicsBackend_Base* GraphicsBackend::pInst = nullptr;

void VertexBufferObject::LoadToGPU()
{
#ifdef BACKEND_D3D
		// Vertex buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(StandardVertex) * num;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pVerts;
		HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer);
		assert(SUCCEEDED(hr));
#endif
}

VertexBufferObject::~VertexBufferObject()
{
	ReleaseAndDeleteCOMobject(mpVertexBuffer);
}

void VertexBufferObject::Bind()
{
#ifdef BACKEND_D3D
	UINT stride = sizeof(StandardVertex);
	UINT offset = 0;
	GraphicsBackend::GetContext().md3dImmediateContext->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
#endif
}

void IndexBufferObject::LoadToGPU()
{
#ifdef BACKEND_D3D
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(TriangleByIndex) * num;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pTris;
		HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateBuffer(&bd, &InitData, &mpIndexBuffer);
		assert(SUCCEEDED(hr));
#endif
}

void IndexBufferObject::Bind()
{
#ifdef BACKEND_D3D
	GraphicsBackend::GetContext().md3dImmediateContext->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
#endif
}

IndexBufferObject::~IndexBufferObject()
{
	ReleaseAndDeleteCOMobject(mpIndexBuffer);
}

void TextureSampler::LoadTexture(std::string filepath, bool ComputeMip, size_t miplevel, uint32_t filterflags)
{
	LPCWSTR lpFilepath = L"";

	std::wstring fpath(lpFilepath);
	std::wstring ext = fpath.substr(fpath.find_last_of(L".") + 1);

	DirectX::ScratchImage scrtTex;
	HRESULT hr = S_OK;
	if (ext == L"tga" || ext == L"TGA")
	{
		hr = LoadFromTGAFile(lpFilepath, nullptr, scrtTex);
	}
	else if (ext == L"dds" || ext == L"DDS")
	{
		hr = LoadFromDDSFile(lpFilepath, DirectX::DDS_FLAGS_NONE, nullptr, scrtTex);
	}
	else
	{
		assert(false && "ERROR: Invalid file format");
	}
	assert(SUCCEEDED(hr));

	if (ComputeMip)
	{
		assert(scrtTex.GetImageCount() == 1 && "ERROR: File already contains MIP map.");
		DirectX::ScratchImage mipchain;
		hr = DirectX::GenerateMipMaps(*(scrtTex.GetImage(0, 0, 0)), filterflags, miplevel, mipchain);
		assert(SUCCEEDED(hr));

		CreateShaderResourceView(GraphicsBackend::GetDevice().md3dDevice, mipchain.GetImage(0, 0, 0), mipchain.GetImageCount(), mipchain.GetMetadata(), &mpTextureRV);
	}
	else
	{
		CreateShaderResourceView(GraphicsBackend::GetDevice().md3dDevice, scrtTex.GetImage(0, 0, 0), scrtTex.GetImageCount(), scrtTex.GetMetadata(), &mpTextureRV);
	}
}

void TextureSampler::CreateSampleState(uint32_t filter, uint32_t isotropic_level)
{
#ifdef BACKEND_D3D
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = (D3D11_FILTER)filter;
	sampDesc.MaxAnisotropy = isotropic_level;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = GraphicsBackend::GetDevice().md3dDevice->CreateSamplerState(&sampDesc, &mpSampler);
	assert(SUCCEEDED(hr));
#endif
}

void TextureSampler::SetToContext(int texResSlot, int sampSlot)
{
#ifdef BACKEND_D3D
	GraphicsBackend::GetContext().md3dImmediateContext->PSSetShaderResources(texResSlot, 1, &mpTextureRV);
	GraphicsBackend::GetContext().md3dImmediateContext->PSSetSamplers(sampSlot, 1, &mpSampler);
#endif
}

TextureSampler::~TextureSampler()
{
#ifdef BACKEND_D3D
	ReleaseAndDeleteCOMobject(mpTextureRV);
	ReleaseAndDeleteCOMobject(mpSampler);
#endif
}