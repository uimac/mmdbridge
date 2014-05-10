/**
 * @file UMVector.h
 * vector
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <cmath>
#include <cfloat>
#include <limits>

namespace umbase
{

template <class T> class UMVector2;
template <class T> class UMVector3;
template <class T> class UMVector4;

typedef UMVector2<int>    UMVec2i;
typedef UMVector2<unsigned int>    UMVec2ui;
typedef UMVector2<float>  UMVec2f;
typedef UMVector2<double> UMVec2d;
typedef UMVector3<int>    UMVec3i;
typedef UMVector3<unsigned int>    UMVec3ui;
typedef UMVector3<float>  UMVec3f;
typedef UMVector3<double> UMVec3d;
typedef UMVector4<int>    UMVec4i;
typedef UMVector4<unsigned int>    UMVec4ui;
typedef UMVector4<float>  UMVec4f;
typedef UMVector4<double> UMVec4d;

/**
 * vector 2
 */
template <class T>
class UMVector2
{
public:
	T x;
	T y;
	
	/**
	 * constructor
	 */
	UMVector2() : x(0), y(0) {}

	/**
	 * constructor
	 */
	UMVector2(T a) : x(a), y(a) {}

	/**
	 * constructor
	 */
	UMVector2(T a, T b) : x(a), y(b) {}
	
	/**
	 * copy constructor
	 */
	UMVector2 (const UMVector2 &v) : x(v.x), y(v.y) {}
	
	/**
	 * assign
	 */
	const UMVector2& operator = (const UMVector2 &v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	
	/**
	 * get
	 */
	T& operator [] (int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& operator [] (int i) const { return (&x)[i]; }

	/**
	 * compare equal
	 */
	bool operator == (const UMVector2 &v) const {
		return (x == v.x && y == v.y);
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMVector2 &v) const {
		return !(*this == v);
	}
	
	/**
	 * plus
	 */
	UMVector2 operator + (const UMVector2 &v) const {
		return UMVector2(x + v.x, y + v.y);
	}
	
	/**
	 * plus equal
	 */
	UMVector2 operator += (const UMVector2 &v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	/**
	 * minus
	 */
	UMVector2 operator - () const {
		return UMVector2(-x, -y);
	}

	/**
	 * minus
	 */
	UMVector2 operator - (const UMVector2 &v) const {
		return UMVector2(x - v.x, y - v.y);
	}
	
	/**
	 * minus equal
	 */
	UMVector2 operator -= (const UMVector2 &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	/**
	 * multiply scolor
	 */
	template <class U>
	UMVector2 operator * (const U &s) const {
		return UMVector2(x * s, y * s);
	}
	
	/**
	 * multiply equal
	 */
	template <class U>
	UMVector2 operator *= (const U &s) {
		x *= s;
		y *= s;
		return *this;
	}

	/**
	 * divide by scolor
	 */
	template <class U>
	UMVector2 operator / (const U &s) const {
		return (*this) * static_cast<U>(1 / s);
	}

	/**
	 * divide equal
	 */
	template <class U>
	UMVector2 operator /= (const U &s) {
		(*this) *= static_cast<U>(1 / s);
		return *this;
	}
	
	/**
	 * dot
	 */
	T dot(const UMVector2 &v) const {
		return (x*v.x + y*v.y);
	}

	/**
	 * multiply
	 */
	UMVector2 multiply(const UMVector2 &v) const {
		return UMVector2(x * v.x, y * v.y);
	}

	/**
	 * get normalized
	 */
	UMVector2 normalized() const {
		UMVector2 dst(*this);
		T a = x * x + y * y;
		if (a > std::numeric_limits<T>::epsilon()) {
			T b = static_cast<T>(1.0 / sqrt(a));
			dst.x = x * b;
			dst.y = y * b;
		} else {
			dst.x = dst.y = static_cast<T>(0);
		}
		return dst;
	}
	
	/**
	 * square of length
	 */
	T length_sq() const {
		return x * x + y * y;
	}

	/**
	 * length
	 */
	T length() const {
		return sqrt(length_sq());
	}
};

/**
 * vector 3
 */
template <class T>
class UMVector3
{
public:
	T x;
	T y;
	T z;
	
	/**
	 * constructor
	 */
	UMVector3() : x(0), y(0), z(0) {}

	/**
	 * constructor
	 */
	UMVector3(T a) : x(a), y(a), z(a) {}

	/**
	 * constructor
	 */
	UMVector3(T a, T b, T c) : x(a), y(b), z(c) {}
	
	/**
	 * constructor
	 */
	UMVector3(const UMVector2<T>& v, T c) : x(v.x), y(v.y), z(c) {}

	/**
	 * copy constructor
	 */
	UMVector3 (const UMVector3 &v) : x(v.x), y(v.y), z(v.z) {}

	/**
	 * assign
	 */
	const UMVector3& operator = (const UMVector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	/**
	 * get
	 */
	T& operator [] (int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& operator [] (int i) const { return (&x)[i]; }

	/**
	 * compare equal
	 */
	bool operator == (const UMVector3 &v) const {
		return (x == v.x && y == v.y && z== v.z);
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMVector3 &v) const {
		return !(*this == v);
	}
	
	/**
	 * plus
	 */
	UMVector3 operator + (const UMVector3 &v) const {
		return UMVector3(x + v.x, y + v.y, z + v.z);
	}
	
	/**
	 * plus equal
	 */
	UMVector3 operator += (const UMVector3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	/**
	 * minus
	 */
	UMVector3 operator - () const {
		return UMVector3(-x, -y, -z);
	}
	
	/**
	 * minus
	 */
	UMVector3 operator - (const UMVector3 &v) const {
		return UMVector3(x - v.x, y - v.y, z - v.z);
	}
	
	/**
	 * minus equal
	 */
	UMVector3 operator -= (const UMVector3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	/**
	 * multiply scolor
	 */
	template <class U>
	UMVector3 operator * (const U &s) const {
		return UMVector3(x * (T)s, y * (T)s, z * (T)s);
	}
	
	/**
	 * multiply equal
	 */
	template <class U>
	UMVector3 operator *= (const U &s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	/**
	 * divide by scolor
	 */
	template <class U>
	UMVector3 operator / (const U &s) const {
		return (*this) * static_cast<U>(1 / s);
	}
	
	/**
	 * divide equal
	 */
	template <class U>
	UMVector3 operator /= (const U &s) {
		(*this) *= static_cast<U>(1 / s);
		return *this;
	}
	
	/**
	 * dot
	 */
	T dot(const UMVector3 &v) const {
		return (x*v.x + y*v.y + z*v.z);
	}

	/**
	 * cross
	 */
	UMVector3 cross(const UMVector3 &v) const {
		return UMVector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	/**
	 * multiply
	 */
	UMVector3 multiply(const UMVector3 &v) const {
		return UMVector3(x * v.x, y * v.y, z * v.z);
	}

	/**
	 * get normalized
	 */
	UMVector3 normalized() const {
		UMVector3 dst(*this);
		T a = x * x + y * y + z * z;
		if (a > std::numeric_limits<T>::epsilon()) {
			T b = static_cast<T>(1.0 / sqrt(a));
			dst.x = x * b;
			dst.y = y * b;
			dst.z = z * b;
		} else {
			dst.x = dst.y = dst.z = static_cast<T>(0);
		}
		return dst;
	}
	
	/**
	 * square of length
	 */
	T length_sq() const {
		return x * x + y * y + z * z;
	}

	/**
	 * length
	 */
	T length() const {
		return sqrt(length_sq());
	}
};

/**
 * vector 4
 */
template <class T>
class UMVector4
{
public:
	T x;
	T y;
	T z;
	T w;
	
	/**
	 * constructor
	 */
	UMVector4() : x(0), y(0), z(0), w(0) {}

	/**
	 * constructor
	 */
	UMVector4(T a) : x(a), y(a), z(a), w(a) {}

	/**
	 * constructor
	 */
	UMVector4(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {}
	
	/**
	 * constructor
	 */
	UMVector4(const UMVector3<T>& v, T d) : x(v.x), y(v.y), z(v.z), w(d) {}
	
	/**
	 * copy constructor
	 */
	UMVector4 (const UMVector4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	
	/**
	 * assign
	 */
	const UMVector4& operator = (const UMVector4 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	
	/**
	 * get
	 */
	T& operator [] (int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& operator [] (int i) const { return (&x)[i]; }

	/** 
	 * get vector3
	 */
	UMVector3<T> xyz() const { return UMVector3<T>(x, y, z); }

	/**
	 * compare equal
	 */
	bool operator == (const UMVector4 &v) const {
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMVector4 &v) const {
		return !(*this == v);
	}
	
	/**
	 * plus
	 */
	UMVector4 operator + (const UMVector4 &v) const {
		return UMVector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	
	/**
	 * plus equal
	 */
	UMVector4 operator += (const UMVector4 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	/**
	 * minus
	 */
	UMVector4 operator - () const {
		return UMVector4(-x, -y, -z, -w);
	}

	/**
	 * minus
	 */
	UMVector4 operator - (const UMVector4 &v) const {
		return UMVector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	
	/**
	 * minus equal
	 */
	UMVector4 operator -= (const UMVector4 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	/**
	 * multiply scolor
	 */
	template <class U>
	UMVector4 operator * (const U &s) const {
		return UMVector4(x * s, y * s, z * s, w * s);
	}
	
	/**
	 * multiply equal
	 */
	template <class U>
	UMVector4 operator *= (const U &s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	/**
	 * divide by scolor
	 */
	template <class U>
	UMVector4 operator / (const U &s) const {
		return (*this) * static_cast<U>(1 / s);
	}

	/**
	 * divide equal
	 */
	template <class U>
	UMVector4 operator /= (const U &s) {
		(*this) *= static_cast<U>(1 / s);
		return *this;
	}
	
	/**
	 * dot
	 */
	T dot(const UMVector4 &v) const {
		return (x*v.x + y*v.y + z*v.z + w*v.w);
	}
	
	/**
	 * cross
	 */
	UMVector4 cross(const UMVector4 &v1, const UMVector4 &v2) const {
		return UMVector4(
			  y * (v1.z * v2.w - v2.z * v1.w) - z * (v1.y * v2.w - v2.y * v1.w) + w * (v1.y * v2.z - v1.z * v2.y),
			-(x * (v1.z * v2.w - v2.z * v1.w) - z * (v1.x * v2.w - v2.x * v1.w) + w * (v1.x * v2.z - v1.z * v2.x)),
			  x * (v1.y * v2.w - v2.y * v1.w) - y * (v1.x * v2.w - v2.x * v1.w) + w * (v1.x * v2.y - v2.x * v1.y),
			-(x * (v1.y * v2.z - v2.y * v1.z) - y * (v1.x * v2.z - v2.x * v1.z) + z * (v1.x * v2.y - v2.x * v1.y))
			);
	}

	/**
	 * multiply
	 */
	UMVector4 multiply(const UMVector4 &v) const {
		return UMVector4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	/**
	 * get normalized
	 */
	UMVector4 normalized() const {
		UMVector4 dst(*this);
		T a = x * x + y * y + z * z + w * w;
		if (a > std::numeric_limits<T>::epsilon()) {
			T b = static_cast<T>(1.0 / sqrt(a));
			dst.x = x * b;
			dst.y = y * b;
			dst.z = z * b;
			dst.w = w * b;
		} else {
			dst.x = dst.y = dst.z = dst.w = static_cast<T>(0);
		}
		return dst;
	}

	/**
	 * square of length
	 */
	T length_sq() const {
		return x * x + y * y + z * z + w * w;
	}

	/**
	 * length
	 */
	T length() const {
		return sqrt(length_sq());
	}
};

} // umbase
