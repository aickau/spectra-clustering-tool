//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  Vecf.h
//! \brief vector classes

#ifndef _VECF_H
#define _VECF_H

#include <math.h>
#include <memory.h>

#pragma warning(disable : 4244)					// conversion double 2 float
#pragma warning(disable : 4305)					// truncation const double 2 float

#undef max
#undef min

class Vec2f
{
public:
	union {
		struct {float x,y;};
		float longvec[2];
	};	

	inline Vec2f() { }

	inline Vec2f(float _x, float _y) {
		this->x=_x; this->y=_y;
	}

	inline Vec2f(float *_a) {
		this->x=_a[0]; this->y=_a[1];
	}

	inline float& operator[](int i) {
		return longvec[i];
	}

	inline void set(float _x, float _y) {
		this->x=_x; this->y=_y; 
	}


	inline void set(float *_a) {
		memcpy(longvec,_a,2*sizeof(float));
	}

	inline void ceil() {
		x=ceilf(x); y=ceilf(y); 
	}

	inline void floor() {
		x=floorf(x); y=floorf(y);
	}

	inline void null() {
		x=y=0.f;
	}


	inline Vec2f max ( const Vec2f &_b ) {
		Vec2f ret;
		ret.x = _b.x>x?_b.x:x;
		ret.y = _b.y>y?_b.y:y;
		return ret;
	}

	inline Vec2f min ( const Vec2f &_b ) {
		Vec2f ret;
		ret.x = _b.x<x?_b.x:x;
		ret.y = _b.y<y?_b.y:y;
		return ret;
	}

	inline Vec2f abs () {
		Vec2f r;
		if (x<0) r.x = -x; else r.x = x;
		if (y<0) r.y = -y; else r.y = y;
		return r;
	}


	inline void negate() {
		x=-x; y=-y;
	}

	inline Vec2f negated() {
		return Vec2f(-x, -y);
	}

	inline void square() {
		x*=x; y*=y;
	}

	inline Vec2f squared() {
		return Vec2f(x*x, y*y);
	}

	inline void invert() {
		x=1.f/x; y=1.f/y;
	}

	inline Vec2f inverted() {
		return Vec2f(1.f/x, 1.f/y);
	}

	inline Vec2f invertedSafe() {
		return Vec2f((x != 0.0f) ? 1.f/x : 0.0f, (y != 0.0f) ? 1.f/y : 0.0f);
	}


	inline void scale(const Vec2f &s) {
		x*=s.x;
		y*=s.y;
	}

	inline Vec2f scaled(const Vec2f &s) {
		return Vec2f(x*s.x, y*s.y);
	}

	inline float magnitude() {
		return sqrtf(x*x+y*y);
	};

	inline float magnitudeSquare() {
		return x*x+y*y;
	};


	inline void normaliseSafe() {
		float l = magnitude();
		if (l==0) {
			x = 1.0f;
			y = 0.f;
		} 
		else 
		{
			x/=l;
			y/=l;
		}	
	}

	inline void normalise() 
	{				
		float l = magnitude();
		x/=l;
		y/=l;
	};

	inline void operator+=(const Vec2f &_a)
	{		
		x+=_a.x;
		y+=_a.y;
	}

	inline void operator-=(const Vec2f &_a){
		x-=_a.x;
		y-=_a.y;
	}

	inline void operator*=(float _a) {
		x*=_a;
		y*=_a;
	}
	inline void operator+=(float _a) {
		x+=_a;
		y+=_a;
	}
};


inline Vec2f operator-(const Vec2f &_a)
{
	return Vec2f( -_a.x, -_a.y);
}

inline Vec2f operator+(const Vec2f &_a, const Vec2f &_b)
{
	return Vec2f(_a.x+_b.x,_a.y+_b.y);
}

inline Vec2f operator-(const Vec2f &_a, const Vec2f &_b)
{
	return Vec2f(_a.x-_b.x,_a.y-_b.y);
}

inline Vec2f operator*(const Vec2f &_a, float _b)
{
	return Vec2f(_a.x*_b, _a.y*_b);
}

inline Vec2f operator*(float _a, const Vec2f &_b)
{
	return Vec2f(_a*_b.x, _a*_b.y);
}

inline Vec2f operator/(Vec2f &_a, float _b)
{
	float ib=1.f/_b;
	return Vec2f(_a.x*ib, _a.y*ib);
}

inline float operator*(const Vec2f &_a, const Vec2f &_b)
{
	return _a.x*_b.x+_a.y*_b.y;
}



// operations todo:
// min, max, lerp, scale, invert, grey, negate, square, magnitude, magnitude sqr, normalize, normalize safe, reflect
class Vec4f
{
public:	
	union {
		struct {float x,y,z,w;};
		struct {float r,g,b,a;};
		float longvec[4];
	};	

	inline Vec4f() { }
	
	inline Vec4f(float _x, float _y, float _z, float _w) {
		this->x=_x; this->y=_y; this->z=_z; this->w=_w;
	}

	inline Vec4f(float *_a) {
		this->x=_a[0]; this->y=_a[1]; this->z=_a[2]; this->w=_a[3];
	}

	inline float& operator[](int i) {
		return longvec[i];
	}
	
	inline void Set(float _x, float _y, float _z) {
		this->x=_x; this->y=_y; this->z=_z;
	}
	inline void Set(float _x, float _y, float _z, float _w) {
		this->x=_x; this->y=_y; this->z=_z; this->w=_w;
	}
	
	inline void Set(float *_a) {
		memcpy(longvec,_a,4*4);
	}

	inline void Ceil() {
		x=ceilf(x); y=ceilf(y); z=ceilf(z); w=ceilf(w);
	}

	inline void Floor() {
		x=floorf(x); y=floorf(y); z=floorf(z); w=floorf(w);
	}
	
	inline void Null() {
		x=y=z=w=0.f;
	}
};


#endif
