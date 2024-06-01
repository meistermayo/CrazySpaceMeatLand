#ifndef _EyeballRing
#define _EyeballRing

class Vect;
class Model;
class GraphicObject_TextureLight;
class ShaderColorLightTexture;
class Matrix;
class Texture;
class Camera;
struct ID3D11Device;
struct ID3D11DeviceContext;

class EyeballRing
{
public:
	EyeballRing(ID3D11Device* md3dDevice, ShaderColorLightTexture* pShaderTexLight);
	void Render(ID3D11DeviceContext* immediateContext, Camera* camera, Vect eyepos, float fogStart, float fogRange, Vect fogColor);
private:
	// EYEBALL BOI 0.0
	Model * pModel_EyeballBoi;
	int eyeballBoiCount = 10;
	Matrix** mWorld_EyeballBois;
	Texture** 	ppTex_EyeballBoi;
	GraphicObject_TextureLight**	EyeballBois;
	ShaderColorLightTexture* pShaderTexLight;
};

#endif