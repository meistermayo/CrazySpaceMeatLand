// ShaderColorLight
// Andre Berthiaume, Feb 2017

#ifndef _ShaderColorLightTexture
#define _ShaderColorLightTexture

#include "ShaderBase.h"
#include "Math/Matrix.h"
#include"Texture.h"

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderColorLightTexture : public ShaderBase
{

public:
	ShaderColorLightTexture(const ShaderColorLightTexture&) = delete;				 // Copy constructor
	ShaderColorLightTexture(ShaderColorLightTexture&&) = default;                    // Move constructor
	ShaderColorLightTexture& operator=(const ShaderColorLightTexture&) & = default;  // Copy assignment operator
	ShaderColorLightTexture& operator=(ShaderColorLightTexture&&) & = default;       // Move assignment operator
	~ShaderColorLightTexture();		  				
	ShaderColorLightTexture(ID3D11Device* device);

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters1(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters2(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters3(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendLightParameters(const Vect& eyepos);
	void SendWorldAndMaterial(const Matrix& world, const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f));
	// Destructor
	void SetTextureResourceAndSampler(Texture* _tex);	
	void SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol);

private:
	struct Material
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};

	struct PhongADS
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};

	struct DirectionalLight
	{
		PhongADS Light;
		Vect Direction;
	};

	DirectionalLight DirLightData;

	struct PointLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		float Range;
	};

	PointLight PointLightData1;
	PointLight PointLightData2;
	PointLight PointLightData3;

	struct SpotLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		Vect Direction;
		float SpotExp;
		float Range;
	};

	SpotLight SpotLightData;

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer*  mpBufferCamMatrices;

	struct Data_WorldAndMaterial
	{
		Matrix World;
		Matrix WorlInv;
		Material Mat;
	};

	ID3D11Buffer*	mpBuffWordAndMaterial;

	struct Data_LightParams
	{
		DirectionalLight DirLight;
		PointLight PntLight1;
		PointLight PntLight2;
		PointLight PntLight3;
		SpotLight SptLight;
		Vect EyePosWorld;
	};

	ID3D11Buffer*  mpBufferLightParams;

	// Texture stuff
	ID3D11ShaderResourceView* mpTextureRV;
	ID3D11SamplerState* mpSampler;
	Texture *mTex;
	public:
		struct FogData {
			float fogMin;
			float fogMax;
			Vect fogCol;
		};

private:
	ID3D11Buffer * mpFog;
};

#endif _ShaderColorLight

