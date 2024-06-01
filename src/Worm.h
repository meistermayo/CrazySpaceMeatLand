#ifndef _Worm
#define _Worm

class Vect;
class Model;
class GraphicObject_TextureLight;
class ShaderColorLightTexture;
class Matrix;
class Texture;
class Camera;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Worm
{
public:
	Worm(ID3D11Device* md3dDevice, ShaderColorLightTexture* pShaderTexLight);
	void Render(ID3D11DeviceContext * md3dImmediateContext, Camera * pCam, Vect eyepos, float fogStart, float fogRange, Vect fogCol);

private:
	Model * pModel_WormyBoi;

	Texture* pTerrain_Texture;

	Matrix* mWorld_WormyBoi;
	Texture** ppTex_WormyBoi;
	GraphicObject_TextureLight* WormyBoi;

	Model* pModel_UnitSphere;
	GraphicObject_TextureLight** GroundSpheres;
	ShaderColorLightTexture* pShaderTexLight;

};

#endif