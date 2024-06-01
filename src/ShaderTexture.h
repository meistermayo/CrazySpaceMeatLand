#ifndef _SHADER_TEXTURE
#define _SHADER_TEXTURE

#include <ShaderBase.h>
#include <Texture.h>
#include "Math/Matrix.h"

class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(ID3D11Device* device);
	~ShaderTexture();

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetTextureResourceAndSampler(Texture* _tex);
	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void ShaderTexture::SendWorld(const Matrix& world);
	void SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol);
	void GetEyePos(const Vect& _eyepos);

private:

	// Shader base stuff

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer*           mpBufferCamMatrices;

	struct InstanceData
	{
		Matrix World;
		Matrix WorldInv;
	};

	ID3D11Buffer*           mpBuffWordColor;
	
	// Texture stuff
	ID3D11ShaderResourceView* mpTextureRV;
	ID3D11SamplerState* mpSampler;
	Texture *mTex;
public:
	struct FogData {
		float fogMin;
		float fogMax;
		Vect fogCol;
		Vect eyePos;
	};
private:
	ID3D11Buffer * mpFog;
	Vect eyepos;
};
#endif