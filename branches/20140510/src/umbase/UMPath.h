/**
 * @file UMPath.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <string>
#include "UMMacro.h"

namespace umbase
{

/**
 * path utility
 */
class UMPath
{
	DISALLOW_COPY_AND_ASSIGN(UMPath);

public:
	static bool exists(const std::u16string& absolute_path);

	static std::u16string module_absolute_path();

	static std::u16string resource_absolute_path(const std::u16string& file_name);

	static std::u16string get_file_name(const std::u16string& file_name);
	
	static std::u16string get_absolute_path(const std::u16string& base_path, std::u16string& file_name);

	static std::u16string get_env(const std::u16string& env);
};

} // umbase
