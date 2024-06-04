#include "GraphicObject_Wireframe.h"
#include "Model.h"
#include "ShaderWireframe.h"
#include <assert.h>

GraphicObject_Wireframe::GraphicObject_Wireframe(ShaderWireframe* shader, int meshCount, Model* mod, const Vect& color)
{
	meshCount; // unreferenced fopr now

	SetModel(mod);
	pShader = (ShaderWireframe*)shader;

	Color = color;
	World = Matrix::Identity;
}

GraphicObject_Wireframe::GraphicObject_Wireframe(Model* mod, ShaderBase* shader, Vect& color)
{
	SetModel(mod);
	pShader = (ShaderWireframe*)shader;

	Color = color;
	World = Matrix::Identity;
}

// NOTE can prob get rid of ptrs to lights...
GraphicObject_Wireframe::~GraphicObject_Wireframe()
{

}

void GraphicObject_Wireframe::SetWorld(const Matrix& m) 
{ 
	World = m;
}

void GraphicObject_Wireframe::Render()
{
	//pShader->SendWorldAndMaterial(World,material->Ambient,material->Diffuse,material->Specular); 
	pModel->BindVertexIndexBuffers(pShader->GetContext());
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldColor(World, Color);
		pModel->RenderMesh(pShader->GetContext(),i);
	}
}
