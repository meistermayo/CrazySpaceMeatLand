#pragma once

#include "Align16.h"

#include <xmmintrin.h>
#include <smmintrin.h>  

class Matrix;

class Vect : public Align16
{
	friend struct vM;
	friend struct vM2;
	friend struct vM3;
	friend struct vM4;

	friend inline vM operator * (const Vect& _v1, const Matrix& _m1);
	friend inline vM2 operator * (const vM& _vM, const Matrix& _m1);
	friend inline vM3 operator * (const vM2& _vM, const Matrix& _m1);
	friend inline vM4 operator * (const vM3& _vM, const Matrix& _m1);

public:
	static const Vect Right;
	static const Vect Up;
	static const Vect Forward;
	static const Vect Zero;
	static const Vect One;

	Vect()
	{
	};

	Vect(const Vect& tmp)
	{
		this->_m = _mm_set_ps(tmp.w, tmp.z, tmp.y, tmp.x);
	}

	Vect(__m128 _m)
	{
		this->_m = _m;
	}

	Vect& operator = (const Vect& tmp)
	{
		this->_m = _mm_set_ps(tmp.w, tmp.z, tmp.y, tmp.x);
		return *this;
	}

	~Vect()
	{
	};


	Vect(const float tx, const float ty, const float tz, const float tw = 1.0f)
	{
		this->_m = _mm_set_ps(tw, tz, ty, tx);
	}

	void set(const float xVal, const float yVal, const float zVal, const float wVal)
	{
		this->_m = _mm_set_ps(wVal, zVal, yVal, xVal);
	}

	Vect operator + (const Vect& tmp) const
	{
		//return Vect(tmp.x + x, tmp.y + y, tmp.z + z, tmp.w + w); // ?
		__m128 teMp = _mm_add_ps(this->_m, tmp._m);
		return Vect(teMp);
	}

	Vect& operator += (const Vect& tmp)
	{
		this->_m = _mm_add_ps(this->_m, tmp._m);
		return *this;
	}

	Vect operator - (const Vect& tmp) const
	{
		/*return Vect(
			x - tmp.x,
			y - tmp.y,
			z - tmp.z,
			w - tmp.w); */
		__m128 teMp = _mm_sub_ps(this->_m, tmp._m);
		return Vect(teMp);
	}

	Vect& operator -= (const Vect& tmp)
	{
		this->_m = _mm_sub_ps(this->_m, tmp._m);
		return *this;
	}

	Vect operator * (const Vect& tmp) const
	{
		//__m128 teMp = _mm_mul_ps(this->_m, tmp._m);
		//return Vect(teMp);

		Vect _tmp = Vect();
		_tmp.x = (y * tmp.z) - (z * tmp.y);
		_tmp.y = (z * tmp.x) - (x * tmp.z);
		_tmp.z = (x * tmp.y) - (y * tmp.x);
		_tmp.w = 1;
		return _tmp;
	}

	Vect& operator *= (const Vect& tmp)
	{
		//	this->_m = _mm_mul_ps(this->_m, tmp._m);
		Vect t = Vect(x, y, z, w);
		x = (t.y * tmp.z) - (t.z * tmp.y);
		y = (t.z * tmp.x) - (t.x * tmp.z);
		z = (t.x * tmp.y) - (t.y * tmp.x);
		w = 1;
		return *this;
	}

	Vect operator / (const Vect& tmp) const
	{
		__m128 teMp = _mm_div_ps(this->_m, tmp._m);
		return Vect(teMp);

		//return Vect(x / tmp.x, y / tmp.y, z / tmp.z, w / tmp.w);
	}

	Vect& operator /= (const Vect& tmp)
	{
		this->_m = _mm_div_ps(this->_m, tmp._m);
		return *this;
		/*/
		this->x /= tmp.x;
		this->y /= tmp.y;
		this->z /= tmp.z;
		this->w /= tmp.w;
		*/
		//return *this;
	}

	float dot(const Vect& t) const
	{
		// Only do the inner product for {x,y,z}
		//_mm_dp_ps(this->_m, t._m, 0xE);
		return (x * t.x) + (y * t.y) + (z * t.z) + (w * t.w);
	}

	Vect operator * (const float s) const
	{
		return Vect(x * s, y * s, z * s, w * s);
	}

	//Vect operator * (const Matrix &m) const;

	static void VectLerp(const Vect& a, const Vect& b, const float t, Vect& outVect)
	{
		outVect = a + ((b - a) * t);
	}
public:

	union
	{
		__m128	_m;

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};





