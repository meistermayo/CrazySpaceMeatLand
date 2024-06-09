#ifndef _EyeballRing
#define _EyeballRing

class Vect;
class Model;
class GraphicsObject_TextureLight;
class ShaderColorLightTexture;
class Matrix;
class Texture;
class Camera;

class EyeballRing
{
public:
	EyeballRing(ShaderColorLightTexture* pShaderTexLight);
	void Render(Camera* camera, Vect eyepos, float fogStart, float fogRange, Vect fogColor);
private:
	// EYEBALL BOI 0.0
	Model * pModel_EyeballBoi;
	int eyeballBoiCount = 10;
	Matrix** mWorld_EyeballBois;
	Texture** 	ppTex_EyeballBoi;
	GraphicsObject_TextureLight**	EyeballBois;
	ShaderColorLightTexture* pShaderTexLight;
};

#endif