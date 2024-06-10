#include "Worm.h"
#include <d3d11.h>
#include "Graphics/Camera.h"
#include "Graphics/GraphicsObject/GraphicsObject_TextureLight.h"
#include "Graphics/Math/Vect.h"
#include "Graphics/Math/Matrix.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Model/FbxModelLoader.h"
#include "Graphics/Shader/ShaderColorLightTexture.h"
#include "Graphics/Texture/Texture.h"


Worm::Worm(ShaderColorLightTexture * pShaderTexLight)
{
	Matrix tempMatrix = Matrix::RotY(180.5f) * Matrix::Scale(1.f) * Matrix::Trans(Vect(0.f, 0.f, -20.f));
	mWorld_WormyBoi = new Matrix(tempMatrix);
	pModel_WormyBoi = new Model("../Assets/Models/WormyBoi_Full.azul", false, true, 2);
	WormyBoi = new GraphicsObject_TextureLight(pShaderTexLight, pModel_WormyBoi);
	WormyBoi->SetWorld(*mWorld_WormyBoi);
	ppTex_WormyBoi = new Texture*[4];
	ppTex_WormyBoi[0] = new Texture("../Assets/Textures/WormyBoi_Neck.tga");
	ppTex_WormyBoi[1] = new Texture("../Assets/Textures/WormyBoi_Eyes.tga");
	ppTex_WormyBoi[2] = new Texture("../Assets/Textures/WormyBoi_Teeth.tga");
	ppTex_WormyBoi[3] = new Texture("../Assets/Textures/WormyBoi_Face.tga");

	for (int i = 0; i < 4; i++)
	{
		WormyBoi->SetTexture(ppTex_WormyBoi[i], i);
	}

	pModel_UnitSphere = new Model(Model::PreMadeModels::UnitSphere, 12);

	GroundSpheres = new GraphicsObject_TextureLight*[7];

	pTerrain_Texture = new Texture("../Assets/Textures/brownsand.tga");

	for (int i = 0; i < 7; i++)
	{
		GroundSpheres[i] = new GraphicsObject_TextureLight(pShaderTexLight, pModel_UnitSphere);
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

void Worm::Render(Camera * pCam, Vect eyepos, float fogStart, float fogRange, Vect fogCol)
{
	pShaderTexLight->SetToContext();
	pShaderTexLight->SendFogData(fogStart, fogRange, fogCol);
	pShaderTexLight->SendCamMatrices(pCam->getViewMatrix(), pCam->getProjMatrix());
	pShaderTexLight->SendLightParameters(eyepos);
	WormyBoi->Render(pCam);
	for (int i = 0; i < 7; i++)
		GroundSpheres[i]->Render(pCam);
}
