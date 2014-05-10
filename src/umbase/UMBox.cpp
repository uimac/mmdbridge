/**
 * @file UMBox.cpp
 * a box
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMBox.h"
#include "UMMath.h"

#include <cmath>
#include <cfloat>
#include <limits>
#include <assert.h>


namespace umbase
{
	
/**
 * constructor
 */
UMBox::UMBox()
	: min_( (std::numeric_limits<double>::infinity)() ),
	max_( -(std::numeric_limits<double>::infinity)() )
{}

/** 
 * initialize
 */
void UMBox::init()
{
	min_ = UMVec3d(  (std::numeric_limits<double>::infinity)() );
	max_ = UMVec3d( -(std::numeric_limits<double>::infinity)() );
}

/** 
 * extend box by point
 */
void UMBox::extend(const UMVec3d& p)
{
	set_minimum(UMVec3d(
		std::min(minimum().x, p.x),
		std::min(minimum().y, p.y),
		std::min(minimum().z, p.z)));
	set_maximum(UMVec3d(
		std::max(maximum().x, p.x),
		std::max(maximum().y, p.y),
		std::max(maximum().z, p.z)));
}

/** 
 * extend box by box
 */
void UMBox::extend(const UMBox& box)
{
	set_minimum(UMVec3d(
		std::min(minimum().x, box.minimum().x),
		std::min(minimum().y, box.minimum().y),
		std::min(minimum().z, box.minimum().z)));
	set_maximum(UMVec3d(
		std::max(maximum().x, box.maximum().x),
		std::max(maximum().y, box.maximum().y),
		std::max(maximum().z, box.maximum().z)));
}

/**
 * get normal at point
 */
UMVec3d UMBox::normal(const UMVec3d& point) const
{
	UMVec3d normal;
	UMVec3d center_to_point = point - center();
	const UMVec3d& size_ = size();
	double min = std::numeric_limits<double>::max();
	double distance = std::abs(size_.x - std::abs(center_to_point.x));
	if (distance < min) {
		min = distance;
		normal.x = 1;
		normal *= um_sign(center_to_point.x);
	}
	distance = std::abs(size_.y - std::abs(center_to_point.y));
	if (distance < min) {
		min = distance;
		normal.y = 1;
		normal *= um_sign(center_to_point.y);
	}
	distance = std::abs(size_.z - std::abs(center_to_point.z));
	if (distance < min) {
		min = distance;
		normal.z = 1;
		normal *= um_sign(center_to_point.z);
	}
	return normal;
}

/**
 * is overlap
 */
bool UMBox::is_overlap(const UMBox& box)
{
	if (box.minimum().length_sq() < max_.length_sq() &&
		box.maximum().length_sq() > min_.length_sq())
	{
		return true;
	}
	return false;
}

/**
 * get transformed box
 */
UMBox UMBox::transformed(const UMMat44d& mat) const
{
	if (is_empty()) return *this;
	
	UMBox box;
	if (mat.m[0][3] == 0 && mat.m[1][3] == 0 && mat.m[2][3] == 0 && mat.m[3][3] == 1)
	{
		for (int i = 0; i < 3; i++) 
		{
			box.min_[i] = box.max_[i] = mat.m[3][i];
			for (int k = 0; k < 3; ++k)
			{
				double a = mat.m[k][i] * box.minimum()[k];
				double b = mat.m[k][i] * box.maximum()[k];
				if (a < b)
				{
					box.min_[i] += a;
					box.max_[i] += b;
				}
				else
				{
					box.min_[i] += b;
					box.max_[i] += a;
				}
			}
		}
	}
	else
	{
		UMVec3d points[8] = 
		{
			UMVec3d(box.min_.x, box.min_.y, box.min_.z),
			UMVec3d(box.max_.x, box.min_.y, box.min_.z),
			UMVec3d(box.max_.x, box.max_.y, box.min_.z),
			UMVec3d(box.max_.x, box.min_.y, box.max_.z),
			UMVec3d(box.min_.x, box.max_.y, box.min_.z),
			UMVec3d(box.min_.x, box.min_.y, box.max_.z),
			UMVec3d(box.min_.x, box.max_.y, box.max_.z),
			UMVec3d(box.max_.x, box.max_.y, box.max_.z)
		};

		for (int i = 0; i < 8; ++i)
		{
			box.extend(mat * points[i]);
		}
	}
	return box;
}

} // umbase
