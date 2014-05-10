/**
 * @file UMMacro.h
 * global macros
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

namespace umbase
{

/**
 * Restrict copy constructor
 */
#ifndef DISALLOW_COPY_AND_ASSIGN
	#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
		private: TypeName(const TypeName&); \
		void operator=(const TypeName&)
#endif

/**
 * for directx com release
 */
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

/**
 * PI
 */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
	
#ifndef M_PI_INV
#define M_PI_INV 0.31830988618379069
#endif

} // umbase
