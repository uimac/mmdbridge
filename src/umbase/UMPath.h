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
#include <vector>
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

	static bool exists(const umstring& absolute_path);
	
	static bool is_folder(const umstring& absolute_path);

	static bool get_child_path_list(
		std::vector<umstring>& dst_folder_list, 
		std::vector<umstring>& dst_file_list,
		const umstring& src_absolute_folder_path);

	static bool remove_file(const umstring& file_path);
	
	static umstring get_base_folder(const umstring& absolute_path);

	static umstring get_temp_absolute_path();

	static umstring module_absolute_path();

	static umstring resource_absolute_path(const umstring& file_name);

	static umstring get_file_name(const umstring& file_name);
	
	static umstring get_absolute_path(const umstring& base_path, umstring& file_name);

	static umstring get_env(const umstring& env);
};

} // umbase
