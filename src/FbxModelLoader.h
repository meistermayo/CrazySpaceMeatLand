#ifndef _FbxModelLoader
#define _FbxModelLoader
#include <fbxsdk.h>
#include "Math/Vect.h"

struct StandardVertex;
struct TriangleByIndex;
struct FbxModelInfo
{
	int nVerts;
	StandardVertex * pVertices;

	int nTris;
	TriangleByIndex * pTris;

	Vect * pNormals;
	Vect * pUVs;
};

class FbxModelLoader
{
public:
	FbxModelLoader();

	FbxModelInfo GetModel(const char * fileName);
	StandardVertex * GetVertices(FbxMesh* pMesh);
	TriangleByIndex * GetTriangles(FbxMesh* pMesh);

	template <typename T>
	T GetLayerElement(FbxLayerElementTemplate<T>* pLayerElement, int index);

private:

};

#endif