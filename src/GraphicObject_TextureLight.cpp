#include "src/Graphics/CrazySpaceMeatLand/src/GraphicObject_TextureLight.h"
#include "Model.h"
#include <assert.h>


GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderColorLightTexture* shader, Model* mod)
{
	pShader = shader;
	SetModel(mod);
	tex = new Texture * [mod->GetMeshCount()];
	world = Matrix::Identity;
	pShader->SetTextureResourceAndSampler(NULL);
}

GraphicObject_TextureLight::GraphicObject_TextureLight(Model* mod, ShaderBase* shader, Texture* inTexture, const Vect& col1, const Vect& col2)
{
	// todo: cols

	pShader = (ShaderColorLightTexture*)shader;
	SetModel(mod);
	tex = new Texture * [mod->GetMeshCount()];
	world = Matrix::Identity;
	pShader->SetTextureResourceAndSampler(NULL);
}

GraphicObject_TextureLight::~GraphicObject_TextureLight() {
	delete[] tex;
}

void GraphicObject_TextureLight::SetWorld(const Matrix& m)
{
	world = m;
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
		pShader->SendWorldAndMaterial(world, Vect::One, Vect::One, Vect::One);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}

Matrix& GraphicObject_TextureLight::GetWorld()
{
	return world;
}