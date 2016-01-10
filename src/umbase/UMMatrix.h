/**
 * @file UMMatrix.h
 * Matrix
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMVector.h"

namespace umbase
{
	
template <class T> class UMMatrix44;

typedef UMMatrix44<float>  UMMat44f;
typedef UMMatrix44<double> UMMat44d;

/**
 * matrix 4x4
 */
template <class T>
class UMMatrix44
{
public:
	/// column-major order matrix
	T m[4][4];
	
	/**
	 * constructor
	 */
	UMMatrix44() {
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k)
			{
				m[i][k] = (i == k) ? (T)1 : (T)0;
			}
		}
	}

	/**
	 * constructor
	 */
	UMMatrix44(const T *mat)
	{
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] = mat[i*4 + k];
			}
		}
	}

	/**
	 * constructor
	 */
	UMMatrix44(
		T _m11, T _m12, T _m13, T _m14,
		T _m21, T _m22, T _m23, T _m24,
		T _m31, T _m32, T _m33, T _m34,
		T _m41, T _m42, T _m43, T _m44)
	{
		m[0][0] = _m11; m[0][1] = _m12; m[0][2] = _m13; m[0][3] = _m14;
		m[1][0] = _m21; m[1][1] = _m22; m[1][2] = _m23; m[1][3] = _m24;
		m[2][0] = _m31; m[2][1] = _m32; m[2][2] = _m33; m[2][3] = _m34;
		m[3][0] = _m41; m[3][1] = _m42; m[3][2] = _m43; m[3][3] = _m44;
	}
	
	/**
	 * copy constructor
	 */
	UMMatrix44(const UMMatrix44 &mat)
	{
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] = mat.m[i][k];
			}
		}
	}
	
	/**
	 * assign
	 */
	const UMMatrix44& operator = (const UMMatrix44 &mat)
	{
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] = mat.m[i][k];
			}
		}
		return *this;
	}

	/**
	* assign
	*/
	void set(int row, int column, T value)
	{
		m[row][column] = value;
	}

	/**
	* get
	*/
	T get(int row, int column)
	{
		return m[row][column];
	}

	T* operator [] (int i)
	{
		return m[i];
	}

	const T* operator [] (int i) const
	{
		return m[i];
	}

	/**
	 * compare equal
	 */
	bool operator == (const UMMatrix44 &v) const 
	{
		bool equal = true;
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				if (m[i][k] != v.m[i][k]) {
					equal = false; break;
				}
			}
		}
		return equal;
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMMatrix44 &mat) const 
	{
		return !(*this == mat);
	}
	
	/**
	 * multiply
	 */
	UMMatrix44 operator * (const UMMatrix44 &mat) const 
	{
		UMMatrix44<T> dst;
		for(int i = 0; i < 4; ++i) {
			for(int j = 0; j < 4; ++j) {
				dst.m[i][j] = 0.0; 
				for(int k = 0; k < 4 ; ++k) {
					dst.m[i][j] += m[i][k] * mat.m[k][j];
				}
			}
		}
		return dst;
	}
	
	/**
	 * multiply (transformed vector)
	 */
	UMVector3<T> operator * (const UMVector3<T>& v) const
	{
		const T tmp[] = {
			v[0]*m[0][0] + v[1]*m[1][0] + v[2]*m[2][0] + 1.0f *m[3][0],
			v[0]*m[0][1] + v[1]*m[1][1] + v[2]*m[2][1] + 1.0f *m[3][1],
			v[0]*m[0][2] + v[1]*m[1][2] + v[2]*m[2][2] + 1.0f *m[3][2]
		};
		return UMVector3<T>(tmp[0], tmp[1], tmp[2]);
	}

	/**
	 * multiply (transformed vector)
	 */
	UMVector4<T> operator * (const UMVector4<T>& v) const
	{
		const T tmp[] = {
			v[0]*m[0][0] + v[1]*m[1][0] + v[2]*m[2][0] + v[3]*m[3][0],
			v[0]*m[0][1] + v[1]*m[1][1] + v[2]*m[2][1] + v[3]*m[3][1],
			v[0]*m[0][2] + v[1]*m[1][2] + v[2]*m[2][2] + v[3]*m[3][2],
			v[0]*m[0][3] + v[1]*m[1][3] + v[2]*m[2][3] + v[3]*m[3][3]
		};
		return UMVector4<T>(tmp[0], tmp[1], tmp[2], tmp[3]);
	}
	
	/**
	 * identity
	 */
	void identity()
	{
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] = (i == k) ? (T)1 : (T)0;
			}
		}
	}
	
	/**
	 * get transposed matrix
	 */
	UMMatrix44 transposed() const
	{
		UMMatrix44 dst;
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				dst.m[k][i] = m[i][k];
			}
		}
		return dst;
	}

	/**
	 * get determinant
	 */
	T determinant() const
	{
		T temp_a = (m[2][2] * m[3][3]) - (m[2][3] * m[3][2]);
		T temp_b = (m[2][1] * m[3][3]) - (m[2][3] * m[3][1]);
		T temp_c = (m[2][1] * m[3][2]) - (m[2][2] * m[3][1]);
		T temp_d = (m[2][0] * m[3][3]) - (m[2][3] * m[3][0]);
		T temp_e = (m[2][0] * m[3][2]) - (m[2][2] * m[3][0]);
		T temp_f = (m[2][0] * m[3][1]) - (m[2][1] * m[3][0]);
		return (m[0][0] * m[1][1]) * (temp_a)
			- (m[0][0] * m[1][2]) * (temp_b)
			+ (m[0][0] * m[1][3]) * (temp_c)
			- (m[0][1] * m[1][0]) * (temp_a)
			+ (m[0][1] * m[1][2]) * (temp_d)
			- (m[0][1] * m[1][3]) * (temp_e)
			+ (m[0][2] * m[1][0]) * (temp_b)
			- (m[0][2] * m[1][1]) * (temp_d)
			+ (m[0][2] * m[1][3]) * (temp_f)
			- (m[0][3] * m[1][0]) * (temp_c)
			+ (m[0][3] * m[1][1]) * (temp_e)
			- (m[0][3] * m[1][2]) * (temp_f);
	}

	/**
	 * get inverted
	 */
	UMMatrix44 inverted() const
	{
		UMMatrix44 dst;
		UMVector4<T> vec[3];

		T det = determinant();
		if ( !det ) { return UMMatrix44(); }

		for (int i = 0; i < 4; ++i) 
		{
			for (int j = 0; j<4; ++j) 
			{
				if (j != i) 
				{
					int a = j;
					if ( j > i ) { a = a-1; }
					vec[a].x = m[j][0];
					vec[a].y = m[j][1];
					vec[a].z = m[j][2];
					vec[a].w = m[j][3];
				}
			}

			UMVector4<T> v(vec[0]);
			v = v.cross(vec[1], vec[2]);
			T rev_det = static_cast<T>(1.0 / det);
			dst.m[0][i] = pow(-1.0f, i) * v.x * rev_det;
			dst.m[1][i] = pow(-1.0f, i) * v.y * rev_det;
			dst.m[2][i] = pow(-1.0f, i) * v.z * rev_det;
			dst.m[3][i] = pow(-1.0f, i) * v.w * rev_det;
		}
		return dst;
	}

	UMVector3<T> translate() const 
	{
		return UMVector3<T>(m[3][0], m[3][1], m[3][2]);
	}
};


/**
* convert matrix to euler xyz
*/
template<typename T>
UMVector3<T> um_matrix_to_euler_xyz(const UMMatrix44<T> &src)
{
	UMVector3<T> euler;

	const T &r00 = src[0][0];
	const T &r01 = src[0][1];
	const T &r02 = src[0][2];

	const T &r10 = src[1][0];
	const T &r11 = src[1][1];
	const T &r12 = src[1][2];

	const T &r20 = src[2][0];
	const T &r21 = src[2][1];
	const T &r22 = src[2][2];

	if (r02 < +1)
	{
		if (r02 > -1)
		{
			euler.y = asin(r02);
			euler.x = atan2(-r12, r22);
			euler.z = atan2(-r01, r00);
		}
		else     // r02 = -1
		{
			euler.y = -M_PI / 2.0;
			euler.x = -atan2(r10, r11);
			euler.z = 0;
		}
	}
	else // r02 = +1
	{
		euler.y = +M_PI / 2.0;
		euler.x = atan2(r10, r11);
		euler.z = 0;
	}
	return euler;
}

/**
* convert euler  xyz to matrix
*/
template<typename T>
UMMatrix44<T> um_euler_to_matrix_xyz(const UMVector3<T> &src)
{
	UMMatrix44<T> dst;

	const T cx = cos(src.x);
	const T cy = cos(src.y);
	const T cz = cos(src.z);
	const T sx = sin(src.x);
	const T sy = sin(src.y);
	const T sz = sin(src.z);

	dst[0][0] = cy * cz;
	dst[0][1] = -cy * sz;
	dst[0][2] = sy;

	dst[1][0] = cz * sx * sy + cx * sz;
	dst[1][1] = cx * cz - sx * sy * sz;
	dst[1][2] = -cy * sx;

	dst[2][0] = -cx * cz * sy + sx * sz;
	dst[2][1] = cz * sx + cx * sy * sz;
	dst[2][2] = cx * cy;

	return dst;
}

/**
 * D3DXMatrixOrthoLH
 */
template<typename T>
void um_matrix_ortho_lh(UMMatrix44<T>& dst, T w, T h, T zn, T zf)
{
	dst.m[0][0] = 2/w;
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2/h;
	dst.m[2][1] = 0.0;
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = 1/(zf-zn);
	dst.m[3][2] = zn/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 0.0;
	dst.m[3][3] = 1.0;
}

/**
 * D3DXMatrixOrthoRH
 */
template<typename T>
void um_matrix_ortho_rh(UMMatrix44<T>& dst, T w, T h, T zn, T zf)
{
	dst.m[0][0] = 2/w;
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2/h;
	dst.m[2][1] = 0.0;
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = 1/(zn-zf);
	dst.m[3][2] = zn/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 0.0;
	dst.m[3][3] = 1.0;
}

/**
 * D3DXMatrixPerspectiveLH
 */
template<typename T>
void um_matrix_perspective_lh(UMMatrix44<T>& dst, T w, T h, T zn, T zf)
{
	dst.m[0][0] = 2*zn/w;
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2*zn/h;
	dst.m[2][1] = 0.0;
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = zf/(zf-zn);
	dst.m[3][2] = zn*zf/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 1.0;
	dst.m[3][3] = 0.0;
}

/**
 * D3DXMatrixPerspectiveRH
 */
template<typename T>
void um_matrix_perspective_rh(UMMatrix44<T>& dst, T w, T h, T zn, T zf)
{
	dst.m[0][0] = 2*zn/w;
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2*zn/h;
	dst.m[2][1] = 0.0;
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = zf/(zn-zf);
	dst.m[3][2] = zn*zf/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = -1.0;
	dst.m[3][3] = 0.0;
}

/**
 * D3DXMatrixPerspectiveFovLH
 */
template<typename T>
void um_matrix_perspective_fov_lh(UMMatrix44<T>& dst, T fov_y, T aspect, T zn, T zf)
{
	T h = (T)1.0 / tan(fov_y/(T)2.0);
	T w;
	if (FLT_EPSILON < fabs(aspect)) {
		w = h / aspect;
	} else {
		w = 0;
	}

	T zf_zn = zf-zn;

	dst.m[0][0] = w;
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = h;
	dst.m[2][1] = 0.0;
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	if ( (zf_zn < -FLT_EPSILON) || (FLT_EPSILON < zf_zn)) {
		dst.m[2][2] = zf/zf_zn;
		dst.m[3][2] = zn*zf/(zn-zf);
	} else {
		dst.m[2][2] = 0.0;
		dst.m[3][2] = 0.0;
	}

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 1.0;
	dst.m[3][3] = 0.0;
}

/**
 * D3DXMatrixPerspectiveFovRH
 */
template<typename T>
void um_matrix_perspective_fov_rh(UMMatrix44<T>& dst, T fov_y, T aspect, T zn, T zf)
{
	T h = (T)1.0 / tan(fov_y/(T)2.0);
	T w;
	if (FLT_EPSILON < fabs(aspect)) {
		w = h / aspect;
	} else {
		w = 0;
	}

	T zn_zf = zn-zf;

	dst.m[0][0] = w;
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = h;
	dst.m[2][1] = 0.0;
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	if ( (zn_zf < -FLT_EPSILON) || (FLT_EPSILON < zn_zf)) {
		dst.m[2][2] = zf/zn_zf;
		dst.m[3][2] = zn*zf/zn_zf;
	} else {
		dst.m[2][2] = 0.0;
		dst.m[3][2] = 0.0;
	}

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = -1.0;
	dst.m[3][3] = 0.0;	
}

/**
 * D3DXMatrixOrthoOffCenterLH
 */
template<typename T>
void um_matrix_ortho_off_center_lh(
	UMMatrix44<T>& dst, T min_x, T max_x, T min_y, T max_y, T zn, T zf)
{
	dst.m[0][0] = 2/(max_x-min_x);
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = (min_x+max_x)/(min_x-max_x);

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2/(max_y-min_y);
	dst.m[2][1] = 0.0;
	dst.m[3][1] = (min_y+max_y)/(min_y-max_y);

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = 1/(zf-zn);
	dst.m[3][2] = zn/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 0.0;
	dst.m[3][3] = 1.0;
}

/**
 * D3DXMatrixOrthoOffCenterRH
 */
template<typename T>
void um_matrix_ortho_off_center_rh(
		UMMatrix44<T>& dst, T min_x, T max_x, T min_y, T max_y, T zn, T zf)
{
	dst.m[0][0] = 2/(max_x-min_x);
	dst.m[1][0] = 0.0;
	dst.m[2][0] = 0.0;
	dst.m[3][0] = (min_x+max_x)/(min_x-max_x);

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2/(max_y-min_y);
	dst.m[2][1] = 0.0;
	dst.m[3][1] = (min_y+max_y)/(min_y-max_y);

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = 1/(zn-zf);
	dst.m[3][2] = zn/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 0.0;
	dst.m[3][3] = 1.0;
}
/**
 * D3DXMatrixPerspectiveOffCenterLH
 */
template<typename T>
void um_matrix_perspective_off_center_lh(
		UMMatrix44<T>& dst, T min_x, T max_x, T min_y, T max_y, T zn, T zf)
{
	dst.m[0][0] = 2*zn/(max_x-min_x);
	dst.m[1][0] = 0.0;
	dst.m[2][0] = (min_x+max_x)/(min_x-max_x);
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2*zn/(max_y-min_y);
	dst.m[2][1] = (min_y+max_y)/(min_y-max_y);
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = zf/(zf-zn);
	dst.m[3][2] = zn*zf/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 1.0;
	dst.m[3][3] = 0.0;
}

/**
 * D3DXMatrixPerspectiveOffCenterRH
 */
template<typename T>
void um_matrix_perspective_off_center_rh(
		UMMatrix44<T>& dst, T min_x, T max_x, T min_y, T max_y, T zn, T zf)
{
	dst.m[0][0] = 2*zn/(max_x-min_x);
	dst.m[1][0] = 0.0;
	dst.m[2][0] = (min_x+max_x)/(max_x-min_x);
	dst.m[3][0] = 0.0;

	dst.m[0][1] = 0.0;
	dst.m[1][1] = 2*zn/(max_y-min_y);
	dst.m[2][1] = (min_y+max_y)/(max_y-min_y);
	dst.m[3][1] = 0.0;

	dst.m[0][2] = 0.0;
	dst.m[1][2] = 0.0;
	dst.m[2][2] = zf/(zn-zf);
	dst.m[3][2] = zn*zf/(zn-zf);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = -1.0;
	dst.m[3][3] = 0.0;
}

/**
 * D3DXMatrixLookAtLH
 */
template<typename T>
void um_matrix_look_at_lh(
		UMMatrix44<T>& dst, 
		const UMVector3<T>& eye, 
		const UMVector3<T>& at,
		const UMVector3<T>& up)
{
	UMVector3<T> zaxis( (at - eye).normalized() );

	UMVector3<T> xaxis( (up.cross(zaxis)).normalized() );

	UMVector3<T> yaxis( zaxis.cross(xaxis) );

	dst.m[0][0] = xaxis.x;
	dst.m[1][0] = xaxis.y;
	dst.m[2][0] = xaxis.z;
	dst.m[3][0] = - xaxis.dot(eye);

	dst.m[0][1] = yaxis.x;
	dst.m[1][1] = yaxis.y;
	dst.m[2][1] = yaxis.z;
	dst.m[3][1] = - yaxis.dot(eye);

	dst.m[0][2] = zaxis.x;
	dst.m[1][2] = zaxis.y;
	dst.m[2][2] = zaxis.z;
	dst.m[3][2] = - zaxis.dot(eye);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 0.0;
	dst.m[3][3] = 1.0;
}

/** 
 * D3DXMatrixLookAtRH
 */
template<typename T>
void um_matrix_look_at_rh(
		UMMatrix44<T>& dst,
		const UMVector3<T>& eye, 
		const UMVector3<T>& at,
		const UMVector3<T>& up)
{
	UMVector3<T> zaxis( (eye - at).normalized() );

	UMVector3<T> xaxis( (up.cross(zaxis)).normalized() );

	UMVector3<T> yaxis( zaxis.cross(xaxis) );

	dst.m[0][0] = xaxis.x;
	dst.m[1][0] = xaxis.y;
	dst.m[2][0] = xaxis.z;
	dst.m[3][0] = - xaxis.dot(eye);

	dst.m[0][1] = yaxis.x;
	dst.m[1][1] = yaxis.y;
	dst.m[2][1] = yaxis.z;
	dst.m[3][1] = - yaxis.dot(eye);

	dst.m[0][2] = zaxis.x;
	dst.m[1][2] = zaxis.y;
	dst.m[2][2] = zaxis.z;
	dst.m[3][2] = - zaxis.dot(eye);

	dst.m[0][3] = 0.0;
	dst.m[1][3] = 0.0;
	dst.m[2][3] = 0.0;
	dst.m[3][3] = 1.0;
}

/** 
 * remove scale from matrix
 */
template<typename T>
void um_matrix_remove_scale(
		UMMatrix44<T>& dst,
		const UMMatrix44<T>& src)
{
	const double len_x_inv = 1.0 / UMVec3d(src.m[0][0], src.m[0][1], src.m[0][2]).length();
	const double len_y_inv = 1.0 / UMVec3d(src.m[1][0], src.m[1][1], src.m[1][2]).length();
	const double len_z_inv = 1.0 / UMVec3d(src.m[2][0], src.m[2][1], src.m[2][2]).length();
	dst.m[0][0] *= len_x_inv; dst.m[0][1] *= len_x_inv; dst.m[0][2] *= len_x_inv;
	dst.m[1][0] *= len_y_inv; dst.m[1][1] *= len_y_inv; dst.m[1][2] *= len_y_inv;
	dst.m[2][0] *= len_z_inv; dst.m[2][1] *= len_z_inv; dst.m[2][2] *= len_z_inv;
}


} //umbase
