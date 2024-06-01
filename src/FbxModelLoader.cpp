#include "FbxModelLoader.h"
#include "Model.h"
#include <vector>
#include <algorithm>
#include <assert.h>

FbxModelLoader::FbxModelLoader()
{

}

FbxModelInfo FbxModelLoader::GetModel(const char* fileName)
{
	FbxManager* pManager = FbxManager::Create();
	FbxModelInfo fbxModelInfo = FbxModelInfo();

	FbxIOSettings * ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");

	if (!pImporter->Initialize(fileName, -1, pManager->GetIOSettings()))
	{
		assert(false);
	}
	else
	{
		// ???
		FbxScene * pScene = FbxScene::Create(pManager, "uhhh");

		pImporter->Import(pScene);
		pImporter->Destroy();
			

		FbxNode* pRootNode = pScene->GetRootNode();
		if (pRootNode)
		{
			for (int i = 0; i < pRootNode->GetChildCount(); i++)
			{
				FbxMesh* mesh = pRootNode->GetChild(i)->GetMesh();
				if (mesh)
				{
					fbxModelInfo.nTris = mesh->GetPolygonCount();
					fbxModelInfo.nVerts = mesh->GetControlPointsCount();
					fbxModelInfo.pVertices = GetVertices(mesh);
					fbxModelInfo.pTris = GetTriangles(mesh);
				}
			}
		}
	}

	pManager->Destroy();


	return fbxModelInfo;
}

StandardVertex * FbxModelLoader::GetVertices(FbxMesh * pMesh)
{
	pMesh->GenerateNormals(true, true);

	int vertexCount = pMesh->GetControlPointsCount();
	StandardVertex* verts = new StandardVertex[vertexCount];

	FbxGeometryElementNormal* pNormalElement = pMesh->GetElementNormal();
	FbxVector4* vertexArray = pMesh->GetControlPoints();
	Vect* normals = new Vect[vertexCount];

	if (pNormalElement)
	{
		if (pNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int i = 0; i < vertexCount; i++)
			{
				FbxVector4 normal = pNormalElement->GetDirectArray().GetAt(i);
				normals[i] = Vect(
					static_cast<float>(normal[0]),
					static_cast<float>(normal[1]),
					static_cast<float>(normal[2]),
					static_cast<float>(normal[3])
				);// .norm();
			}
		}
		else if (pNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int triangleCount = pMesh->GetPolygonCount();
			for (int i = 0; i < triangleCount; i++)
			{
				int indexCount = pMesh->GetPolygonSize(i);
				for (int j = 0; j < indexCount; j++)
				{
					FbxVector4 normal = FbxVector4(1, 1, 1, 1);//pNormalElement->GetDirectArray().GetAt(j + (i*indexCount));
					normals[j + (i*indexCount)] = Vect(0,0,1,1);// normal[0], normal[1], normal[2], normal[3]).norm();
				}
			}
		}
	}
	FbxDouble scale = 1.0f;
	for (int i = 0; i < vertexCount; i++)
	{
		verts[i] = StandardVertex();
		verts[i].set(
			static_cast<float>(vertexArray[i].mData[0] * scale),
			static_cast<float>(vertexArray[i].mData[1] * scale),
			static_cast<float>(vertexArray[i].mData[2] * scale),
			static_cast<float>(i)/vertexCount,
			static_cast<float>(i) / vertexCount, normals[i],
			Vect(1,1,1),
			0
		);
	}

	return verts;
}

TriangleByIndex * FbxModelLoader::GetTriangles(FbxMesh * pMesh)
{
	int polygonCount = pMesh->GetPolygonCount();
	// int* triangles = pMesh->GetPolygonVertices();
	std::vector<TriangleByIndex*> triVector = std::vector<TriangleByIndex*>();


	for (int i = 0; i < polygonCount; i++)
	{
		int indexCount = pMesh->GetPolygonSize(i);
		assert(indexCount == 3);
			/*/
		{
			std::vector<int> indices = std::vector<int>();
			switch (indexCount)
			{
				 // :(
			case 4:
				indices.push_back(pMesh->GetPolygonVertex(i, 0));
				indices.push_back(pMesh->GetPolygonVertex(i, 1));
				indices.push_back(pMesh->GetPolygonVertex(i, 2));
				indices.push_back(pMesh->GetPolygonVertex(i, 3));
				
				std::sort(indices.begin(), indices.end());
				
				triVector.push_back(new TriangleByIndex());
				triVector[triVector.size()-1]->set(
					indices[0],
					indices[1],
					indices[2]
				);

				triVector.push_back(new TriangleByIndex());
				triVector[triVector.size() - 1]->set(
					indices[0],
					indices[2],
					indices[3]
				);
				indices.clear();
				break;


			case 5:
				triVector.push_back(new TriangleByIndex());
				triVector[triVector.size() - 1]->set(
					pMesh->GetPolygonVertex(i, 0),
					pMesh->GetPolygonVertex(i, 1),
					pMesh->GetPolygonVertex(i, 2)
				);
				triVector.push_back(new TriangleByIndex());
				triVector[triVector.size() - 1]->set(
					pMesh->GetPolygonVertex(i, 1),
					pMesh->GetPolygonVertex(i, 3),
					pMesh->GetPolygonVertex(i, 4)
				);
				triVector.push_back(new TriangleByIndex());
				triVector[triVector.size() - 1]->set(
					pMesh->GetPolygonVertex(i, 1),
					pMesh->GetPolygonVertex(i, 4),
					pMesh->GetPolygonVertex(i, 2)
				);
				break;
			}
		}
		else
		//*/
		{
			triVector.push_back(new TriangleByIndex());
			triVector[triVector.size() - 1]->set(
				pMesh->GetPolygonVertex(i, 0),
				pMesh->GetPolygonVertex(i, 1),
				pMesh->GetPolygonVertex(i, 2)
			);
		}
	}
	TriangleByIndex* tris = new TriangleByIndex[triVector.size()];

	for (size_t i = 0; i < triVector.size(); i++)
	{
		tris[i] = *triVector[i];
	}

	return tris;
}
