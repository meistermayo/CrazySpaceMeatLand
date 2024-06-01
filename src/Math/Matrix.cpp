//----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------

#include "Vect.h"
#include "Matrix.h"

Vect Matrix::operator * (const Vect& v)
{
	return Vect(v.dot(v0), v.dot(v1), v.dot(v2), v.dot(v3));
}

//*/
Matrix Matrix::operator * (const Matrix& t)
{
	return Matrix(
		Vect(
			this->v0.dot(Vect(t.m0, t.m4, t.m8, t.m12)),
			this->v0.dot(Vect(t.m1, t.m5, t.m9, t.m13)),
			this->v0.dot(Vect(t.m2, t.m6, t.m10, t.m14)),
			this->v0.dot(Vect(t.m3, t.m7, t.m11, t.m15))),
		Vect(
			this->v1.dot(Vect(t.m0, t.m4, t.m8, t.m12)),
			this->v1.dot(Vect(t.m1, t.m5, t.m9, t.m13)),
			this->v1.dot(Vect(t.m2, t.m6, t.m10, t.m14)),
			this->v1.dot(Vect(t.m3, t.m7, t.m11, t.m15))),
		Vect(
			this->v2.dot(Vect(t.m0, t.m4, t.m8, t.m12)),
			this->v2.dot(Vect(t.m1, t.m5, t.m9, t.m13)),
			this->v2.dot(Vect(t.m2, t.m6, t.m10, t.m14)),
			this->v2.dot(Vect(t.m3, t.m7, t.m11, t.m15))),
		Vect(
			this->v3.dot(Vect(t.m0, t.m4, t.m8, t.m12)),
			this->v3.dot(Vect(t.m1, t.m5, t.m9, t.m13)),
			this->v3.dot(Vect(t.m2, t.m6, t.m10, t.m14)),
			this->v3.dot(Vect(t.m3, t.m7, t.m11, t.m15)))
	);
}
//*/


// ---  End of File ---------------
