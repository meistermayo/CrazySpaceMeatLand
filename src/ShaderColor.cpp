#include "ShaderColor.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>


ShaderColor::ShaderColor(ID3D11Device* device)
	: ShaderBase(device, L"../Assets/Shaders/D3D/ColorSelected3D.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	bd.ByteWidth = sizeof(Data_WorldColor);
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
}

ShaderColor::~ShaderColor()
{
	ReleaseAndDeleteCOMobject(mpBuffWordColor);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
	ReleaseAndDeleteCOMobject(mpFog);
}

void ShaderColor::SendCamMatrices( const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;  
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderColor::SendWorldColor(const Matrix& world, const Vect& col)
{
	Data_WorldColor wc;
	wc.World = world;
	wc.WorldInv = world.GetWorldInverse();
	wc.Color = col;

	this->GetContext()->UpdateSubresource(mpBuffWordColor, 0, nullptr, &wc, 0, 0);
}

void ShaderColor::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->VSSetConstantBuffers(1, 1, &mpBuffWordColor);
	devcon->VSSetConstantBuffers(2, 1, &mpFog);

	devcon->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->PSSetConstantBuffers(1, 1, &mpBuffWordColor);
	devcon->PSSetConstantBuffers(2, 1, &mpFog);
}


void ShaderColor::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;
	fogData.eyePos = eyepos;
	this->GetContext()->UpdateSubresource(mpFog, 0, nullptr, &fogData, 0, 0);
}
void ShaderColor::GetEyePos(const Vect& _eyepos)
{
	this->eyepos = _eyepos;
}


