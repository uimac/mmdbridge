/**
 * @file UMTime.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"
#include <memory>
#include <string>

namespace umbase
{

class UMTime;
typedef std::shared_ptr<UMTime> UMTimePtr;

/**
 * print time on destructor
 */
class UMTime
{
	DISALLOW_COPY_AND_ASSIGN(UMTime);

public:

	/**
	 * constructor
	 * @param [in] print_time print time on destructor
	 */
	UMTime(const std::string& message);
	
	/**
	 * constructor
	 * @param [in] print_time print time on destructor
	 */
	UMTime(const std::string& message, bool show_message_box);

	/**
	 * get time
	 */
	static unsigned int current_time();

	~UMTime();

private:
	bool show_message_box_;
	std::string message_;
	unsigned long initial_time_;
};

} // umbase
