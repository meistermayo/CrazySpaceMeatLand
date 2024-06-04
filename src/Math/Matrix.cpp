//----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------

#include "Vect.h"
#include "Matrix.h"
#include "Quat.h"

const Matrix Matrix::Identity(Vect::Right_0, Vect::Up_0, Vect::Forward_0, Vect::Zero);

//*/
Matrix Matrix::operator * (const Matrix& t)
{
	return Matrix(
		Vect(
			this->v0.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v0.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v0.dot(Vect(t.m02, t.m12, t.m22, t.m32)),
			this->v0.dot(Vect(t.m03, t.m13, t.m23, t.m33))),
		Vect(
			this->v1.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v1.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v1.dot(Vect(t.m02, t.m12, t.m22, t.m32)),
			this->v1.dot(Vect(t.m03, t.m13, t.m23, t.m33))),
		Vect(
			this->v2.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v2.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v2.dot(Vect(t.m02, t.m12, t.m22, t.m32)), 
			this->v2.dot(Vect(t.m03, t.m13, t.m23, t.m33))),
		Vect(
			this->v3.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v3.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v3.dot(Vect(t.m02, t.m12, t.m22, t.m32)),
			this->v3.dot(Vect(t.m03, t.m13, t.m23, t.m33)))
	);
}

Matrix& Matrix::operator *= (const Matrix& t)
{
	*this = *this * t;
	return *this;
}

Matrix::Matrix(const Quat& tmp)
{
	// todo : optimize
	float x = tmp.x;
	float y = tmp.y;
	float z = tmp.z;
	float w = tmp.w;

	v0 = Vect(1.f - 2.f*y*y - 2.f*z*z,	2.f*x*y + 2.f*w*z,			2.f*x*z - 2.f*w*y,			0.f);
	v1 = Vect(2.f*x*y - 2.f*w*z,		1.f - 2.f*x*x - 2.f*z*z,	2.f*y*z + 2.f*w*x,			0.f);
	v2 = Vect(2.f*x*z - 2.f*w*y,		2.f*y*z - 2.f*w*x,			1.f -2.f*x*x - 2.f*y*y,		0.f);
	v3 = Vect::Zero;
}
//*/


// ---  End of File ---------------
