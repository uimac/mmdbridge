/**
 * @file UMPath.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <windows.h>
#include <Mmsystem.h>

#include <string>
#include <tchar.h>
#include <shlwapi.h>

#include "UMPath.h"
#include "UMStringUtil.h"

namespace umbase
{
	
bool UMPath::exists(const std::u16string& absolute_path)
{
	std::wstring inpath = UMStringUtil::utf16_to_wstring(absolute_path);
	if (::PathFileExistsW(inpath.c_str()))
	{
		return true;
	}
	return false;
}

std::u16string UMPath::module_absolute_path()
{
	TCHAR path[1024];
	GetModuleFileName(NULL, path, sizeof(path) / sizeof(TCHAR));
	return UMStringUtil::wstring_to_utf16(path);
}

std::u16string UMPath::resource_absolute_path(const std::u16string& file_name)
{
	TCHAR path[1024];
	GetModuleFileName(NULL, path, sizeof(path) / sizeof(TCHAR));
	PathRemoveFileSpec(path);
	SetCurrentDirectory(path);
	SetCurrentDirectory(_T("../../../resource/"));
	GetCurrentDirectory(1024, path);
	std::wstring inpath = path + std::wstring(_T("\\")) + UMStringUtil::utf16_to_wstring(file_name);
	// honban you kozaiku
	if (! ::PathFileExistsW(inpath.c_str()))
	{
		SetCurrentDirectory(path);
		SetCurrentDirectory(_T("./resource/"));
		GetCurrentDirectory(1024, path);
		inpath = path + std::wstring(_T("\\")) + UMStringUtil::utf16_to_wstring(file_name);
	}
	return UMStringUtil::wstring_to_utf16(inpath);
}

std::u16string UMPath::get_file_name(const std::u16string& file_path)
{
	std::wstring path = UMStringUtil::utf16_to_wstring(file_path);
	std::wstring filename(::PathFindFileName(path.c_str()));
	return UMStringUtil::wstring_to_utf16(filename);
}

std::u16string UMPath::get_absolute_path(const std::u16string& base_path, std::u16string& file_name)
{
	std::u16string base_path_copy = base_path;
	std::u16string base_file_name = get_file_name(base_path_copy);
	if (!base_file_name.empty())
	{
		std::u16string::size_type pos = base_path_copy.find(base_file_name);
		if (pos != std::u16string::npos)
		{
			base_path_copy.erase(
				base_path_copy.begin() + pos ,
				base_path_copy.begin() + pos + base_file_name.length());
		}
	}
	return base_path_copy + file_name;
}

std::u16string UMPath::get_env(const std::u16string& env)
{
	if (wchar_t* val =  _wgetenv(UMStringUtil::utf16_to_wstring(env).c_str()))
	{
		return UMStringUtil::wstring_to_utf16(val);
	}
	std::u16string none;
	return none;
}

} // umbase
