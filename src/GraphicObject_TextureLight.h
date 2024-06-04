#pragma once

#ifndef _GraphicObject_TextureL
#define _GraphicObject_TextureL

#include "GraphicObject_Base.h"
#include "Math/Vect.h"
#include "Texture.h"
#include "ShaderColorLightTexture.h"

class GraphicObject_TextureLight : public GraphicObject_Base
{
	friend class Texture;
	//friend class ShaderTexture;

public:
	GraphicObject_TextureLight(const GraphicObject_TextureLight&) = delete;
	GraphicObject_TextureLight(GraphicObject_TextureLight&&) = default;
	GraphicObject_TextureLight& operator=(const GraphicObject_TextureLight&)& = default;
	GraphicObject_TextureLight& operator = (GraphicObject_TextureLight&&) & = default;
	~GraphicObject_TextureLight();

	GraphicObject_TextureLight() = delete;

	void SetTexture(Texture* _tex, int i);
	void SetWorld(const Matrix& m);
	Matrix& GetWorld();
	virtual void Render() override;

	GraphicObject_TextureLight(ShaderColorLightTexture* shader, Model* mod);
	GraphicObject_TextureLight(Model* mod, ShaderBase* shader, Texture* inTexture, const Vect& col, const Vect& col2);

private:
	Texture ** tex;
	Matrix world;
	ShaderColorLightTexture* pShader;
};

#endif _GraphicObject_Texture