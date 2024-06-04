//----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------

#include "Vect.h"
#include "Matrix.h"

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
//*/


// ---  End of File ---------------
