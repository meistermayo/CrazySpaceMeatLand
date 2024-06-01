// ShaderColor
// Andre Berthiaume, july 2016

#ifndef _ShaderColor
#define _ShaderColor

#include "ShaderBase.h"
#include "Math/Matrix.h"

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderColor : public ShaderBase
{
public:
	ShaderColor(const ShaderColor&) = delete;				 // Copy constructor
	ShaderColor(ShaderColor&&) = default;                    // Move constructor
	ShaderColor& operator=(const ShaderColor&) & = default;  // Copy assignment operator
	ShaderColor& operator=(ShaderColor&&) & = default;       // Move assignment operator
	~ShaderColor();		  							         // Destructor

	ShaderColor(ID3D11Device* device);

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendWorldColor(const Matrix& world, const Vect& col);
	void SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol);
	void GetEyePos(const Vect& _eyepos);

private:
	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer*           mpBufferCamMatrices;

	struct Data_WorldColor
	{
		Matrix World;
		Matrix WorldInv;
		Vect Color;
	};

	ID3D11Buffer*           mpBuffWordColor;
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

#endif _ShaderColor
