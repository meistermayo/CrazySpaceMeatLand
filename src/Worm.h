#ifndef _Worm
#define _Worm

class Vect;
class Model;
class GraphicsObject_TextureLight;
class ShaderColorLightTexture;
class Matrix;
class Texture;
class Camera;

class Worm
{
public:
	Worm(ShaderColorLightTexture* pShaderTexLight);
	void Render(Camera * pCam, Vect eyepos, float fogStart, float fogRange, Vect fogCol);

private:
	Model * pModel_WormyBoi;

	Texture* pTerrain_Texture;

	Matrix* mWorld_WormyBoi;
	Texture** ppTex_WormyBoi;
	GraphicsObject_TextureLight* WormyBoi;

	Model* pModel_UnitSphere;
	GraphicsObject_TextureLight** GroundSpheres;
	ShaderColorLightTexture* pShaderTexLight;

};

#endif