#include "TerrainModel.h"
#include "d3dUtil.h"
#include <assert.h>


TerrainModel::TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
{
	ytrans;
	DirectX::ScratchImage scrtTex;
	HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrtTex);
	assert(SUCCEEDED(hr));

	const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width );

	//size_t side = hgtmap->height;	// the image should be square

	mDevice = dev;

	WorldMat = Matrix(MatrixScaleType::SCALE, 0.25, 0.25, 0.25);

	// ** much work to add below **
	ConstructTerrain(hgtmap,maxheight,len,RepeatU,RepeatV); // how about two lines
	LoadData();
}

void TerrainModel::Render(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(StandardVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->DrawIndexed(ntris * 3, 0, 0);


}

void TerrainModel::ConstructTerrain(const DirectX::Image* hMap,float maxHeight,float len,int repeatU, int repeatV)
{
	// do you have to be able and willing to ruin your life to have a semblance of what morality is


	size_t height = hMap->height;
	size_t width =  hMap->width;

	ntris = (height-1)*(height-1) * 2;
	nverts = ntris * 3;

	pVerts = new StandardVertex[nverts];
	pTriList = new TriangleByIndex[ntris];

	for (size_t x = 0; x < width-1; x++)
	{
		for (size_t y = 0; y < height-1; y++)
		{
			size_t i = (y * 6) + (x * 6 * (height - 1));
			float scale = (maxHeight / 255.0f);
			float side = (float)width;

			float x0 = (float)(x);
			float x1 = (float)(x + 1);
			float y0 = (float)(y);
			float y1 = (float)(y + 1);


			float h_val_f0 = ((float)hMap->pixels[4 * ((x + 0) * hMap->height + y + 0)]) * scale;
			float h_val_f1 = ((float)hMap->pixels[4 * ((x + 1) * hMap->height + y + 1)]) * scale;
			float h_val_f2 = ((float)hMap->pixels[4 * ((x + 1) * hMap->height + y + 0)]) * scale;
			float h_val_f3 = ((float)hMap->pixels[4 * ((x + 0) * hMap->height + y + 0)]) * scale;
			float h_val_f4 = ((float)hMap->pixels[4 * ((x + 0) * hMap->height + y + 1)]) * scale;
			float h_val_f5 = ((float)hMap->pixels[4 * ((x + 1) * hMap->height + y + 1)]) * scale;
			
			// # P.xy store the position for which we want to calculate the normals

			

			pVerts[i + 0].set(x0*len, h_val_f0*len, y0*len, ((x0 / side)*repeatU), ((y0 / side)*repeatV), Vect(), Colors::White*h_val_f0);
			pVerts[i + 1].set(x1*len, h_val_f1*len, y1*len, ((x1 / side)*repeatU), ((y1 / side)*repeatV), Vect(), Colors::White*h_val_f1);
			pVerts[i + 2].set(x1*len, h_val_f2*len, y0*len, ((x1 / side)*repeatU), ((y0 / side)*repeatV), Vect(), Colors::White*h_val_f2);
			pVerts[i + 3].set(x0*len, h_val_f3*len, y0*len, ((x0 / side)*repeatU), ((y0 / side)*repeatV), Vect(), Colors::White*h_val_f3);
			pVerts[i + 4].set(x0*len, h_val_f4*len, y1*len, ((x0 / side)*repeatU), ((y1 / side)*repeatV), Vect(), Colors::White*h_val_f4);
			pVerts[i + 5].set(x1*len, h_val_f5*len, y1*len, ((x1 / side)*repeatU), ((y1 / side)*repeatV), Vect(), Colors::White*h_val_f5);


		}
	}

	for (size_t x = 1; x < width - 2; x++)
	{
		for (size_t y = 1; y < height - 2; y++)
		{
			size_t i = (y * 6) + (x * 6 * (height-1));
			if (x == 1 && y == 1) continue;
				// BL
				pVerts[i].normal =
					normalHelper(

						pVerts[i].Pos,										// p
						pVerts[i + 2].Pos,									// p6
						pVerts[i - 3].Pos,									// p2
						pVerts[i - (int)(6 * height) - 3].Pos,				// p1
						pVerts[i - (int)(6 * height) - 2].Pos,				// p4
						pVerts[i + 4].Pos,									// p8
						pVerts[i + 1].Pos									// p9

					);
				i++;

				//UR
				pVerts[i].normal =
					normalHelper(

						pVerts[i].Pos,										// p
						pVerts[i + (int)(6 * height)].Pos,									// p6
						pVerts[i + 1].Pos,								// p2
						pVerts[i + 2].Pos,				// p1
						pVerts[i + 3].Pos,				// p4
						pVerts[i + 6].Pos,									// p8
						pVerts[i + (int)(6 * height)+6].Pos									// p9

					);
				i++;

				//BR
				pVerts[i].normal =
					normalHelper(

						pVerts[i].Pos,										// p
						pVerts[i + (int)(6 * height)].Pos,									// p6
						pVerts[i - 6].Pos,									// p2
						pVerts[i - 8].Pos,				// p1
						pVerts[i + 1].Pos,				// p4
						pVerts[i - 1].Pos,									// p8
						pVerts[i + (int)(6 * height) - 1].Pos									// p9

					);
				i++;

				//BL2
				pVerts[i].normal =
					normalHelper(

						pVerts[i].Pos,										// p
						pVerts[i - 1].Pos,									// p6
						pVerts[i - 6].Pos,									// p2
						pVerts[i - (int)(6 * height)-6].Pos,				// p1
						pVerts[i - (int)(6 * height)].Pos,				// p4
						pVerts[i + 1].Pos,									// p8
						pVerts[i + 2].Pos									// p9

					);
				i++;

				//UL
				pVerts[i].normal =
					normalHelper(

						pVerts[i].Pos,										// p
						pVerts[i + 1].Pos,									// p6
						pVerts[i - 1].Pos,									// p2
						pVerts[i - (int)(6 * height) - 1].Pos,				// p1
						pVerts[i - (int)(6 * height)].Pos,				// p4
						pVerts[i + 6].Pos,									// p8
						pVerts[i + 7].Pos									// p9

					);
				i++;

				//UR2
				pVerts[i].normal =
					normalHelper(

						pVerts[i].Pos,										// p
						pVerts[i + (int)(6 * height)].Pos,									// p6
						pVerts[i - 3].Pos,									// p2
						pVerts[i - 2].Pos,				// p1
						pVerts[i - 1].Pos,				// p4
						pVerts[i + 2].Pos,									// p8
						pVerts[i + (int)(6 * height) + 2].Pos									// p9

					);
				i++;
		}
	}
	//assert(numLoops == nverts);
	int j = 0;
	for (int i = 0; i < (ntris); i+=2)
	{
		pTriList[i].set(j, 
						j + 1, 
						j + 2);
		pTriList[i+1].set(j+3, 
						j + 4, 
						j + 5);
		j += 6;
	}
	//assert(numLoops == ntris);
}
Vect TerrainModel::normalHelper(Vect p, Vect p6, Vect p2, Vect p1, Vect p4, Vect p8, Vect p9) 
{
	Vect v1 = p9 - p; // urr
	Vect v2 = p6 - p;
	Vect cross1 = v1.cross(v2);

	v1 = p8 - p; // uur
	v2 = p9 - p;
	Vect cross2 = v1.cross(v2);

	v1 = p4 - p; // ul
	v2 = p8 - p;
	Vect cross3 = v1.cross(v2);

	v1 = p1 - p; // bll
	v2 = p4 - p;
	Vect cross4 = v1.cross(v2);

	v1 = p2 - p; // bbl
	v2 = p1 - p;
	Vect cross5 = v1.cross(v2);

	v1 = p6 - p; // br
	v2 = p2 - p;
	Vect cross6 = v1.cross(v2);

	Vect norm = cross1 + cross2 + cross3 + cross4 + cross5 + cross6;
	float mult = 1.0f / 6.0f;
	norm = norm * mult;
	norm = norm.getNorm();
	if (norm.Y() < 0)
		norm.set(norm.X(), -norm.Y(), norm.Z());

	if (norm.Y() == 0)
		norm.set(norm.X(), 1, norm.Z());

	return norm;
}

void TerrainModel::LoadData()
{
	// Vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(StandardVertex) * nverts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = pVerts;
	HRESULT hr = mDevice->CreateBuffer(&bd, &InitData, &VertexBuffer); 
	assert(SUCCEEDED(hr));

	// Index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TriangleByIndex) * ntris;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = pTriList;
	hr = mDevice->CreateBuffer(&bd, &InitData, &IndexBuffer);
	assert(SUCCEEDED(hr));
}

TerrainModel::~TerrainModel()
{
	delete[] pVerts;
	delete[] pTriList;

	ReleaseAndDeleteCOMobject(VertexBuffer);
	ReleaseAndDeleteCOMobject(IndexBuffer);
}
