#include "ShaderColorLightTexture.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>

ShaderColorLightTexture::ShaderColorLightTexture(ID3D11Device* dev)
	: ShaderBase(dev, L"../Assets/Shaders/ColorLightTexture.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElements);

	HRESULT hr = S_OK;

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

	// light param
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_LightParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferLightParams);
	assert(SUCCEEDED(hr));

	// Color buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_WorldAndMaterial);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWordAndMaterial);
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

	// Zeroing the light data
	ZeroMemory(&DirLightData, sizeof(DirLightData));
	ZeroMemory(&PointLightData1, sizeof(PointLightData1));
	ZeroMemory(&PointLightData2, sizeof(PointLightData2));
	ZeroMemory(&PointLightData3, sizeof(PointLightData3));
	ZeroMemory(&SpotLightData, sizeof(SpotLightData));

	mTex = new Texture(GetDevice(), L"../Assets/Textures/CrateFace.tga");
}

ShaderColorLightTexture::~ShaderColorLightTexture()
{
	ReleaseAndDeleteCOMobject(mpBuffWordAndMaterial);
	ReleaseAndDeleteCOMobject(mpBufferLightParams);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
	ReleaseAndDeleteCOMobject(mpFog);
}

void ShaderColorLightTexture::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;
	this->GetContext()->UpdateSubresource(mpFog, 0, nullptr, &fogData, 0, 0);
}

void ShaderColorLightTexture::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderColorLightTexture::SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp)
{
	DirLightData.Light.Ambient = amb;
	DirLightData.Light.Diffuse = dif;
	DirLightData.Light.Specular = sp;
	DirLightData.Direction = dir;
}

void ShaderColorLightTexture::SetPointLightParameters1(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	PointLightData1.Light.Ambient = amb;
	PointLightData1.Light.Diffuse = dif;
	PointLightData1.Light.Specular = sp;
	PointLightData1.Position = pos;
	PointLightData1.Attenuation = att;
	PointLightData1.Range = r;
}

void ShaderColorLightTexture::SetPointLightParameters2(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	PointLightData2.Light.Ambient = amb;
	PointLightData2.Light.Diffuse = dif;
	PointLightData2.Light.Specular = sp;
	PointLightData2.Position = pos;
	PointLightData2.Attenuation = att;
	PointLightData2.Range = r;
}

void ShaderColorLightTexture::SetPointLightParameters3(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	PointLightData3.Light.Ambient = amb;
	PointLightData3.Light.Diffuse = dif;
	PointLightData3.Light.Specular = sp;
	PointLightData3.Position = pos;
	PointLightData3.Attenuation = att;
	PointLightData3.Range = r;
}

void ShaderColorLightTexture::SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp)
{
	SpotLightData.Light.Ambient = amb;
	SpotLightData.Light.Diffuse = dif;
	SpotLightData.Light.Specular = sp;
	SpotLightData.Position = pos;
	SpotLightData.Direction = dir;
	SpotLightData.Attenuation = att;
	SpotLightData.Range = r;
	SpotLightData.SpotExp = spotExp;
}

void ShaderColorLightTexture::SendLightParameters( const Vect& eyepos)
{
	Data_LightParams dl;
	dl.DirLight = DirLightData;
	dl.PntLight1 = PointLightData1;
	dl.PntLight2 = PointLightData2;
	dl.PntLight3 = PointLightData3;
	dl.SptLight = SpotLightData;
	dl.EyePosWorld = eyepos;

	this->GetContext()->UpdateSubresource(mpBufferLightParams, 0, nullptr, &dl, 0, 0);
}

void ShaderColorLightTexture::SendWorldAndMaterial(const Matrix& world, const Vect& amb, const Vect& dif, const Vect& sp)
{
	Data_WorldAndMaterial wm;
	wm.World = world;
	wm.WorlInv = world.getInv();
	wm.Mat.Ambient = amb;
	wm.Mat.Diffuse = dif;
	wm.Mat.Specular = sp;

	this->GetContext()->UpdateSubresource(mpBuffWordAndMaterial, 0, nullptr, &wm, 0, 0);
}
/*
void ShaderColorLightTexture::FuckYouMom()
{
	this->GetContext()->UpdateSubresource(urMumGay, 0, nullptr, &ym, 0, 0);
}
*/
void ShaderColorLightTexture::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();


	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->VSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->VSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);
	devcon->VSSetConstantBuffers(3, 1, &mpFog);

	devcon->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->PSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->PSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);
	devcon->PSSetConstantBuffers(3, 1, &mpFog);

	mTex->SetToContext(devcon);
}

void ShaderColorLightTexture::SetTextureResourceAndSampler(Texture* _tex)
{
	// mpSampler
	// mpTextureRV
	_tex;
	//tex->Texture::SetToContext(GetContext());
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