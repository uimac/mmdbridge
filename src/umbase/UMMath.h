/**
 * @file UMMath.h
 * math functions
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <cmath>

#include "UMMacro.h"
#include "UMMatrix.h"
#include "UMVector.h"

namespace umbase
{

/**
 * convert degree to radian
 */
template<typename T>
T um_to_radian(T degree) { return (T)((degree) * (M_PI / 180.0)); }

/**
 * convert radian to degree
 */
template<typename T>
T um_to_degree(T radian) { return (T)((radian) * (180.0 / M_PI)); }

/**
 * lerp
 */
template<typename T>
T um_lerp(T left, T right, T s)
{
	return left + s * (right - left);
}

/**
 * signum
 */
template <typename T>
int um_sign(T val) {
	return (T(0) < val) - (val < T(0));
}

/**
 * clip
 */
template <typename T>
T um_clip(T val) {
	if (val > (T)1.0) val = (T)1.0;
	if (val < (T)0.0) val = (T)0.0;
	return val;
}

/**
 * clamp
 */
template <class T>
T um_clamp(T a, T l, T h)
{
	return (a < l) ? l : ((a > h) ? h : a);
}

} // umbase
