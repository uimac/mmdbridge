/**
 * @file UMStringUtil.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <locale>
#include "UMMacro.h"

namespace umbase
{

/**
 * string utility
 */
class UMStringUtil
{
	DISALLOW_COPY_AND_ASSIGN(UMStringUtil);

public:
	
	template <typename T>
	static std::wstring number_to_wstring(T value)
	{
		std::wstringstream converter;
		std::wstring  wstr;
		converter << value;
		converter >> wstr;
		return wstr;
	}
	
	template <typename T>
	static std::string number_to_string(T value)
	{
		std::stringstream converter;
		std::string  str;
		converter << value;
		converter >> str;
		return str;
	}
	
	template <typename T>
	static std::string number_to_sequence_string(T value, int n)
	{
		std::stringstream converter;
		std::string  str;
		converter << std::setw(n) << std::setfill('0') << value;
		converter >> str;
		return str;
	}
	
	/**
	 * convert wstring to utf16
	 */
	static std::u16string wstring_to_utf16(const std::wstring& str)
	{
#ifdef _WIN32
		const char16_t* p = reinterpret_cast<const char16_t*>(str.c_str());
		std::u16string u16str(p);
#else
		// not implemented
		std::u16string u16str;
		assert(0);
#endif
		return u16str;
	}
	
	/**
	 * convert utf16 to wstring
	 */
	static std::wstring utf16_to_wstring(const std::u16string& utf16str)
	{
#ifdef _WIN32
		const wchar_t* p = reinterpret_cast<const wchar_t*>(utf16str.c_str());
		std::wstring wstr(p);
#else
		// not implemented
		std::wstring wstr;
		assert(0);
#endif
		return wstr;
	}
	
	/**
	 * convert wstring to utf8
	 */
	static std::string wstring_to_utf8(const std::wstring& str)
	{
#ifdef _WIN32
		const char16_t* p = reinterpret_cast<const char16_t*>(str.c_str());
		std::string utf8str = UMStringUtil::utf16_to_utf8(std::u16string(p));
#else
		// not implemented
		std::u16string u16str;
		assert(0);
#endif
		return utf8str;
	}

	/**
	 * convert utf8 string to utf16
	 */
	static std::u16string utf8_to_utf16(const std::string& utf8str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
		return convert.from_bytes(utf8str);
	}

	/**
	 * convert utf16 to utf8 string
	 */
	static std::string utf16_to_utf8(const std::u16string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
		return convert.to_bytes(str);
	}

	/**
	 * convert utf8 to utf32 string
	 */
	static std::u32string utf8_to_utf32(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> convert;
		return convert.from_bytes(str);
	}
};

} // umbase
