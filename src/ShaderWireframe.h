#pragma once

#include "ShaderBase.h"
#include "Math/Matrix.h"

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderWireframe : public ShaderBase
{
public:
	ShaderWireframe(const ShaderWireframe&) = delete;				 // Copy constructor
	ShaderWireframe(ShaderWireframe&&) = default;                    // Move constructor
	ShaderWireframe& operator=(const ShaderWireframe&) & = default;  // Copy assignment operator
	ShaderWireframe& operator=(ShaderWireframe&&) & = default;       // Move assignment operator
	~ShaderWireframe();		  							         // Destructor

	ShaderWireframe(ID3D11Device* device);

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
