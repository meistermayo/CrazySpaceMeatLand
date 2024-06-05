#include "ShaderTexture.h"
#include <assert.h>
#include "DirectXTex.h"
#include "d3dUtil.h"

ShaderTexture::ShaderTexture(ID3D11Device*device) :
	ShaderBase(device, L"../Assets/Shaders/D3D/Texture.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElements);

	HRESULT hr;

	// View Projection buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamMatrices);
	assert(SUCCEEDED(hr));

	// Color buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(InstanceData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWordColor);
	assert(SUCCEEDED(hr));

	// Foggoe buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FogData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpFog);
	assert(SUCCEEDED(hr));

	// mTex = new Texture(GetDevice(), L"../Assets/Textures/CrateFace.tga");
}

ShaderTexture::~ShaderTexture()
{
	delete mTex;
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
	ReleaseAndDeleteCOMobject(mpBuffWordColor);
	ReleaseAndDeleteCOMobject(mpTextureRV); // ???
	ReleaseAndDeleteCOMobject(mpSampler);
	ReleaseAndDeleteCOMobject(mpFog);
}

void ShaderTexture::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->VSSetConstantBuffers(1, 1, &mpBuffWordColor);
	devcon->VSSetConstantBuffers(2, 1, &mpFog);

	devcon->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->PSSetConstantBuffers(1, 1, &mpBuffWordColor);
	devcon->PSSetConstantBuffers(2, 1, &mpFog);

	mTex->SetToContext();
}

void ShaderTexture::SetTextureResourceAndSampler(Texture* _tex)
{
	_tex;
	DirectX::ScratchImage testTexture;
	HRESULT hr = LoadFromTGAFile(L"../Assets/Textures/CrateFace.tga", nullptr, testTexture);
	assert(SUCCEEDED(hr));
	ID3D11Device *d3dDev = GetDevice();

	CreateShaderResourceView(d3dDev, testTexture.GetImage(0, 0, 0), testTexture.GetImageCount(), testTexture.GetMetadata(), &mpTextureRV);

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = d3dDev->CreateSamplerState(&sampDesc, &mpSampler);
	assert(SUCCEEDED(hr));
}

void ShaderTexture::SendCamMatrices(const Matrix& view, const Matrix& proj)
{

	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderTexture::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;
	fogData.eyePos = eyepos;
	this->GetContext()->UpdateSubresource(mpFog, 0, nullptr, &fogData, 0, 0);
}
void ShaderTexture::GetEyePos(const Vect& _eyepos)
{
	this->eyepos = _eyepos;
}
void ShaderTexture::SendWorld(const Matrix& world)
{
	InstanceData wc;
	wc.World = world;
	wc.WorldInv = world.GetWorldInverse();
	this->GetContext()->UpdateSubresource(mpBuffWordColor, 0, nullptr, &wc, 0, 0);

}