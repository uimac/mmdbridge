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

#include <string>

namespace umbase
{

/**
 * Restrict copy constructor
 */
#ifndef DISALLOW_COPY_AND_ASSIGN
	#ifdef WITH_EMSCRIPTEN
		#define DISALLOW_COPY_AND_ASSIGN(TypeName) 
	#else
		#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
			private: TypeName(const TypeName&); \
			void operator=(const TypeName&)
	#endif // WITH_EMSCRIPTEN
#endif

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


/**
 * string
 */
#ifdef WITH_EMSCRIPTEN
	typedef std::string umstring;
	typedef std::wstring umtextstring;
#else
	typedef std::u16string umstring;
	typedef std::wstring umtextstring;
#endif