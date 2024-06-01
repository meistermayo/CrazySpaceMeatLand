#include "GraphicObject_TextureL.h"
#include "Model.h"
#include <assert.h>


GraphicObject_TextureLight::GraphicObject_TextureLight( ShaderColorLightTexture* shader,Model* mod)
{
	pShader = shader;
	SetModel(mod);
	tex = new Texture*[mod->GetMeshCount()];
	world = Matrix(IDENTITY);
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
		pShader->SendWorldAndMaterial(world, 1.0 * Vect(1, 1, 1, 1), 1.0 * Vect(1, 1, 1, 1), 1.0 * Vect(1, 1, 1, 1));
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}

Matrix& GraphicObject_TextureLight::GetWorld()
{
	return world;
}