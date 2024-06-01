#ifndef _TerrainModel
#define _TerrainModel

#include "Math/Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"
#include "Model.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModel : public Align16
{

private:
	Model* pModTerrain;
	Matrix WorldMat;


	void ConstructTerrain(const DirectX::Image* hMap, float maxHeight, float len,int repeatU, int repeatV);
	void LoadData();

	ID3D11Device *mDevice;

	StandardVertex *pVerts;
	TriangleByIndex* pTriList;
	int nverts;
	int ntris;
	ID3D11Buffer *VertexBuffer;
	ID3D11Buffer *IndexBuffer;
	Vect normalHelper(Vect p, Vect p6, Vect p2, Vect p1, Vect p4, Vect p8, Vect p9);
public:
	TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV);
	~TerrainModel();
	Matrix GetWorldMat() { return WorldMat; }

	void Render(ID3D11DeviceContext* context);

};



#endif _TerrainModel