/**
 * @file UMMathTypes.h
 * global typedefs
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

// caution! this header defines global typedefs.

#include <memory>

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

template <class T> class UMMatrix44;

typedef UMMatrix44<float>  UMMat44f;
typedef UMMatrix44<double> UMMat44d;

class UMBox;
typedef std::shared_ptr<UMBox> UMBoxPtr;
};

typedef umbase::UMVec2i UMVec2i;
typedef umbase::UMVec2ui UMVec2ui;
typedef umbase::UMVec2f UMVec2f;
typedef umbase::UMVec2d UMVec2d;
typedef umbase::UMVec3i UMVec3i;
typedef umbase::UMVec3ui UMVec3ui;
typedef umbase::UMVec3f UMVec3f;
typedef umbase::UMVec3d UMVec3d;
typedef umbase::UMVec4i UMVec4i;
typedef umbase::UMVec4ui UMVec4ui;
typedef umbase::UMVec4f UMVec4f;
typedef umbase::UMVec4d UMVec4d;

typedef umbase::UMMat44f UMMat44f;
typedef umbase::UMMat44d UMMat44d;

typedef umbase::UMBox UMBox;
typedef umbase::UMBoxPtr UMBoxPtr;

