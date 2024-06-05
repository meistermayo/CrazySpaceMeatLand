#ifndef SKYBOX_H
#define SKYBOX_H

#include "Model.h"
#include "ShaderTexture.h"
#include "Texture.h"

class Skybox {

public:
	Skybox(ID3D11Device *device, ShaderTexture* pShader, Texture* pTexture);
	~Skybox();
	void CreateSkybox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&_pTriList, int& ntri);
	void Render(ID3D11DeviceContext *context);

private:
	//void LoadDataToGPU();

private :
	Model * pModel;
	ID3D11Device * mDevice;
	StandardVertex * pStdVerts;
	TriangleByIndex * pTriList;
	int numTri;
	int numVerts;

	ShaderTexture* mShader;
	Texture* mTexture;
};

#endif