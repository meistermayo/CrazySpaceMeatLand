#include "src/Graphics/CrazySpaceMeatLand/src/GraphicObject_TextureLight.h"
#include "Model.h"
#include <assert.h>


GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderColorLightTexture* shader, Model* mod)
{
	ambColor = Vect::One;
	difColor = Vect::One;
	pShader = shader;
	SetModel(mod);
	tex = new Texture * [mod->GetMeshCount()];
	*pWorld = Matrix::Identity;
	pShader->SetTextureResourceAndSampler(NULL);
}

GraphicObject_TextureLight::GraphicObject_TextureLight(Model* mod, ShaderBase* shader, Texture* inTexture, const Vect& inAmb, const Vect& inDif)
{
	ambColor = inAmb;
	ambColor = inDif;

	pShader = (ShaderColorLightTexture*)shader;
	SetModel(mod);

	tex = new Texture * [mod->GetMeshCount()];
	tex[0] = inTexture;

	*pWorld = Matrix::Identity;
	pShader->SetTextureResourceAndSampler(NULL);
}

GraphicObject_TextureLight::~GraphicObject_TextureLight() {
	delete[] tex;
}

void GraphicObject_TextureLight::SetTexture(Texture* _tex, int i)
{
	assert(pModel->ValidMeshNum(i));
	this->tex[i] = _tex;
}

void GraphicObject_TextureLight::Render()
{
	pModel->BindVertexIndexBuffers(pShader->GetContext());
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		tex[i]->SetToContext(pShader->GetContext());
		pShader->SendWorldAndMaterial(*pWorld, ambColor, difColor, Vect::One);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}
