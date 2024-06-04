#include "GraphicObject_Color.h"
#include "Model.h"
#include "ShaderColor.h"
#include <assert.h>

GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight* shader, int meshCount, Model* mod, const Vect &amb, const Vect &dif, const Vect& sp)
{
	meshCount;
	material = new ShaderColorLight::Material();
	this->material->Ambient = amb;
	this->material->Diffuse = dif;
	this->material->Specular = sp;
	SetModel(mod );
	pShader = shader;

	Color = new Vect[mod->GetMeshCount()];

	for (int i = 0; i < mod->GetMeshCount(); i++)
	{
		Color[i] = Vect(0,0,0, 1);
	}
	World = Matrix::Identity;
}
// NOTE can prob get rid of ptrs to lights...
GraphicObject_ColorLight::~GraphicObject_ColorLight()
{

}

void GraphicObject_ColorLight::SetWorld(const Matrix& m) 
{ 
	World = m;
}

void GraphicObject_ColorLight::Render()
{
	//pShader->SendWorldAndMaterial(World,material->Ambient,material->Diffuse,material->Specular); 
	pModel->BindVertexIndexBuffers(pShader->GetContext());
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World, material->Ambient, material->Diffuse, material->Specular);
		pModel->RenderMesh(pShader->GetContext(),i);
	}
}
