#include "Worm.h"
#include "ShaderColorLightTexture.h"
#include "GraphicObject_TextureLight.h"
#include "Texture.h"
#include "Math/Vect.h"
#include "Math/Matrix.h"
#include "Model.h"
#include "Camera.h"
#include <d3d11.h>
#include "FbxModelLoader.h"


Worm::Worm(ID3D11Device * md3dDevice, ShaderColorLightTexture * pShaderTexLight)
{
	Matrix tempMatrix = Matrix::RotY(180.5f) * Matrix::Scale(1.f) * Matrix::Trans(Vect(0.f, 0.f, -20.f));
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
	pModel_UnitSphere = new Model(md3dDevice, Model::PreMadeModels::UnitSphere, 12);
	//pModel_UnitSphere = new Model(md3dDevice, Model::PreMadeModels::UnitBoxRepeatedTexture,12.0f);
	//pModel_UnitSphere = new Model(md3dDevice, fbxModelInfo);


	GroundSpheres = new GraphicObject_TextureLight*[7];

	pTerrain_Texture = new Texture(md3dDevice, L"../Assets/Textures/brownsand.tga");

	for (int i = 0; i < 7; i++)
	{
		GroundSpheres[i] = new GraphicObject_TextureLight(pShaderTexLight, pModel_UnitSphere);
		GroundSpheres[i]->SetTexture(pTerrain_Texture, 0);
	}

	GroundSpheres[0]->SetWorld(Matrix::Trans(Vect(0, 0, -50)));
	GroundSpheres[1]->SetWorld(Matrix::Trans(Vect(20, -2, -55)));
	GroundSpheres[2]->SetWorld(Matrix::Trans(Vect(17, -2, -75)));
	GroundSpheres[3]->SetWorld(Matrix::Trans(Vect(5, 1, -85)));
	GroundSpheres[4]->SetWorld(Matrix::Trans(Vect(-5, -4, -85)));
	GroundSpheres[5]->SetWorld(Matrix::Trans(Vect(-15, 1, -65)));
	GroundSpheres[6]->SetWorld(Matrix::Trans(Vect(-10, 3, -55)));

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
