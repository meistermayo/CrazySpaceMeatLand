#include "ShaderTexture.h"
#include <assert.h>
#include "DirectXTex.h"
#include "d3dUtil.h"
#include "GraphicsBackend.h"

ShaderTexture::ShaderTexture(std::string filename) :
	ShaderBase(filename)
{
	mpShaderInterface->DefineInputLayoutTex();

	mpBufferCamMatrices = new GenericBufferObject<CamMatrices>();
	mpBufferCamMatrices->CreateBuffer(0);

	mpBuffWordColor = new GenericBufferObject<InstanceData>();
	mpBuffWordColor->CreateBuffer(1);

	mpFog = new GenericBufferObject<FogData>();
	mpFog->CreateBuffer(2);
	
	// mTex = new Texture(GetDevice(), L"../Assets/Textures/CrateFace.tga");
}

ShaderTexture::~ShaderTexture()
{
	delete mTex;
	delete mpBufferCamMatrices;
	delete mpBuffWordColor;
	// delete mpTextureRV; // todo ??? // C5205... v143 issue??
	// delete mpSampler;
	delete mpFog;
}

void ShaderTexture::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	mpBufferCamMatrices->Bind();
	mpBuffWordColor->Bind();
	mpFog->Bind();

	// mTex->SetToContext(); // todo
}

void ShaderTexture::SetTextureResourceAndSampler(Texture* _tex)
{
	// todo ????
	_tex;
	DirectX::ScratchImage testTexture;
	HRESULT hr = LoadFromTGAFile(L"../Assets/Textures/CrateFace.tga", nullptr, testTexture);
	assert(SUCCEEDED(hr));
	ID3D11Device *d3dDev = GraphicsBackend::GetDevice().md3dDevice;

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

	mpBufferCamMatrices->UpdateBuffer(&mCamMatrices);
}

void ShaderTexture::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;
	fogData.eyePos = eyepos;
	mpFog->UpdateBuffer(&fogData);
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
	mpBuffWordColor->UpdateBuffer(&wc);
}