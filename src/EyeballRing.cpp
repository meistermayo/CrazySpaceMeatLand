#include "EyeballRing.h"
#include "ShaderColorLightTexture.h"
#include "src/Graphics/CrazySpaceMeatLand/src/GraphicObject_TextureLight.h"
#include "Texture.h"
#include "Math/Vect.h"
#include "Math/Matrix.h"
#include "Model.h"
#include "Camera.h"
#include "Math/Constants.h"
#include <d3d11.h>

EyeballRing::EyeballRing(ShaderColorLightTexture* pShaderTexLight)
{
	int texNum = 7;
	ppTex_EyeballBoi = new Texture*[texNum];
	ppTex_EyeballBoi[0] = new Texture("../Assets/Textures/Tex_Eyeball_Stalk.tga");
	ppTex_EyeballBoi[1] = new Texture("../Assets/Textures/Tex_Eyeball_Stalk.tga");
	ppTex_EyeballBoi[2] = new Texture("../Assets/Textures/Tex_Eyeball_Stalk.tga");
	ppTex_EyeballBoi[3] = new Texture("../Assets/Textures/Tex_Eyeball_Stalk.tga");
	ppTex_EyeballBoi[4] = new Texture("../Assets/Textures/Tex_Eyeball_Stalk.tga");
	ppTex_EyeballBoi[5] = new Texture("../Assets/Textures/Tex_Eyeball_Stalk.tga");
	ppTex_EyeballBoi[6] = new Texture("../Assets/Textures/Tex_Eyeball_Ball.tga");

	pModel_EyeballBoi = new Model("../Assets/Models/Eyeball_Ascii.azul", false, true, 0.25f);

	EyeballBois = new GraphicObject_TextureLight*[eyeballBoiCount];

	mWorld_EyeballBois = new Matrix*[eyeballBoiCount];
	for (int i = 0; i < eyeballBoiCount; i++)
	{
		EyeballBois[i] = new GraphicObject_TextureLight(pShaderTexLight, pModel_EyeballBoi);
		for (int j = 0; j < texNum; j++)
		{
			EyeballBois[i]->SetTexture(ppTex_EyeballBoi[j], j);
		}
		// Initialize the world matrix
		float fCos = cosf(i * 36 * MATH_PI / 180.0f);
		float fSin = sinf(i * 36 * MATH_PI / 180.0f);
		float fSin_y = sinf(((i * 36) + 90) * MATH_PI / 180.0f);
		Matrix tempMatrix = Matrix::Trans(Vect((fSin * 2.0f), (fSin_y * 1.0f), (fCos*2.0f)));
		mWorld_EyeballBois[i] = new Matrix(tempMatrix);

		EyeballBois[i]->SetWorld(*mWorld_EyeballBois[i]);
	}
	this->pShaderTexLight = pShaderTexLight;
}

void EyeballRing::Render(ID3D11DeviceContext* md3dImmediateContext, Camera* pCam, Vect eyepos, float fogStart, float fogRange, Vect fogCol)
{
	pShaderTexLight->SetToContext(md3dImmediateContext);
	pShaderTexLight->SendFogData(fogStart, fogRange, fogCol);
	pShaderTexLight->SendCamMatrices(pCam->getViewMatrix(), pCam->getProjMatrix());
	pShaderTexLight->SendLightParameters(eyepos);
	for (int i = 0; i<eyeballBoiCount; i++)
		EyeballBois[i]->Render();
}
