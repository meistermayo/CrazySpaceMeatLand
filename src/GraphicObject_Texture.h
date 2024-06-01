#pragma once

#ifndef _GraphicObject_Texture
#define _GraphicObject_Texture

#include "GraphicObject_Base.h"
#include "Math/Vect.h"
#include "Texture.h"
#include "ShaderTexture.h"

class GraphicObject_Texture : public GraphicObject_Base
{
	friend class Texture;
	//friend class ShaderTexture;

public:
	GraphicObject_Texture(const GraphicObject_Texture&) = delete;
	GraphicObject_Texture(GraphicObject_Texture&&) = default;
	GraphicObject_Texture& operator=(const GraphicObject_Texture&)& = default;
	GraphicObject_Texture& operator = (GraphicObject_Texture&&) & = default;
	~GraphicObject_Texture();

	GraphicObject_Texture() = delete;

	void SetTexture(Texture* _tex, int i);
	void SetWorld(const Matrix& m);
	virtual void Render() override;

	GraphicObject_Texture(ShaderTexture* shader, Model* mod);

private:
	Texture ** tex;
	Matrix world;
	ShaderTexture* pShader;
};

#endif _GraphicObject_Texture