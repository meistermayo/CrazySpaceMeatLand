#include "Worm.h"
#include "ShaderColorLightTexture.h"
#include "GraphicObject_TextureL.h"
#include "Texture.h"
#include "Math/Vect.h"
#include "Math/Matrix.h"
#include "Model.h"
#include "Camera.h"
#include <d3d11.h>
#include "FbxModelLoader.h"


Worm::Worm(ID3D11Device * md3dDevice, ShaderColorLightTexture * pShaderTexLight)
{
	FbxModelLoader fbxModelLoader = FbxModelLoader();
	FbxModelInfo fbxModelInfo = fbxModelLoader.GetModel("../Assets/Models/Cube_Tri.fbx");

	Matrix tempMatrix = Matrix(RotType::ROT_Y, 180.5) * Matrix(SCALE, 1, 1, 1) * Matrix(TRANS, 0, 0, -20);
	mWorld_WormyBoi = new Matrix(tempMatrix);
	pModel_WormyBoi = new Model(md3dDevice, "../Assets/Models/WormyBoi_Full.azul", false, true, 2);
	WormyBoi = new GraphicObject_TextureLight(pShaderTexLight, pModel_WormyBoi);
	WormyBoi->SetWorld(*mWorld_WormyBoi);
	ppTex_WormyBoi = new Texture*[4];
	ppTex_WormyBoi[0] = new Texture(md3dDevice, L"../Assets/Textures/WormyBoi_Neck.tga");
	ppTex_WormyBoi[1] = new Texture(md3dDevice, L"../Assets/Textures/WormyBoi_Eyes.tga");
	ppTex_WormyBoi[2] = new Texture(md3dDevice, L"../Assets/Textures/WormyBoi_Teeth.tga");
	ppTex_WormyBoi[3] = new Texture(md3dDevice, L"../Assets/Textures/WormyBoi_Face.tga");
	for (int i = 0; i < 4; i++)
	{
		WormyBoi->SetTexture(ppTex_WormyBoi[i], i);
	}
	pModel_UnitSphere = new Model(md3dDevice, Model::PreMadedeModels::UnitSphere, 12);
	//pModel_UnitSphere = new Model(md3dDevice, Model::PreMadedeModels::UnitBoxRepeatedTexture,12.0f);
	//pModel_UnitSphere = new Model(md3dDevice, fbxModelInfo);


	GroundSpheres = new GraphicObject_TextureLight*[7];

	pTerrain_Texture = new Texture(md3dDevice, L"../Assets/Textures/brownsand.tga");

	for (int i = 0; i < 7; i++)
	{
		GroundSpheres[i] = new GraphicObject_TextureLight(pShaderTexLight, pModel_UnitSphere);
		GroundSpheres[i]->SetTexture(pTerrain_Texture, 0);
	}

	GroundSpheres[0]->SetWorld(Matrix(TRANS, 0, 0, -50));
	GroundSpheres[1]->SetWorld(Matrix(TRANS, 20, -2, -55));
	GroundSpheres[2]->SetWorld(Matrix(TRANS, 17, -2, -75));
	GroundSpheres[3]->SetWorld(Matrix(TRANS, 5, 1, -85));
	GroundSpheres[4]->SetWorld(Matrix(TRANS, -5, -4, -85));
	GroundSpheres[5]->SetWorld(Matrix(TRANS, -15, 1, -65));
	GroundSpheres[6]->SetWorld(Matrix(TRANS, -10, 3, -55));

	this->pShaderTexLight = pShaderTexLight;
}

void Worm::Render(ID3D11DeviceContext * md3dImmediateContext, Camera * pCam, Vect eyepos, float fogStart, float fogRange, Vect fogCol)
{
	pShaderTexLight->SetToContext(md3dImmediateContext);
	pShaderTexLight->SendFogData(fogStart, fogRange, fogCol);
	pShaderTexLight->SendCamMatrices(pCam->getViewMatrix(), pCam->getProjMatrix());
	pShaderTexLight->SendLightParameters(eyepos);
	WormyBoi->Render();
	for (int i = 0; i < 7; i++)
		GroundSpheres[i]->Render();
}
