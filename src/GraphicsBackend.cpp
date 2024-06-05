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