#ifndef _SHADER_TEXTURE
#define _SHADER_TEXTURE

#include "ShaderBase.h"
#include "Texture.h"
#include "Math/Matrix.h"

template <typename T>
struct GenericBufferObject;

class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(std::string filename);
	ShaderTexture() = delete;
	~ShaderTexture();

	virtual void SetToContext() override;

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

	GenericBufferObject<CamMatrices>*           mpBufferCamMatrices;

	struct InstanceData
	{
		Matrix World;
		Matrix WorldInv;
	};

	GenericBufferObject<InstanceData>*           mpBuffWordColor;
	
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
	GenericBufferObject<FogData>* mpFog;
	Vect eyepos;
};
#endif