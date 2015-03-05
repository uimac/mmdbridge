/**
 * @file UMEvent.h
 * a event
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include <vector>
#include "UMMacro.h"
#include "UMListener.h"
#include "UMAny.h"
#include "UMEventType.h"

namespace umbase
{

class UMEvent;
typedef std::shared_ptr<UMEvent> UMEventPtr;

typedef std::vector<UMEventPtr> UMEventList;

class UMEvent 
{
	DISALLOW_COPY_AND_ASSIGN(UMEvent);

public:
	typedef umbase::UMAny Parameter;
	
	UMEvent(UMEventType event_type)
		: key_(event_type),
		parameter_(0) {}

	UMEvent(UMEventType event_type, Parameter& umany)
		: key_(event_type),
		parameter_(umany) {}

	virtual ~UMEvent() {}

	void add_listener(UMListenerWeakPtr listener);
	void delete_listener(UMListenerWeakPtr listener);
	int listener_count() const { return static_cast<int>(listeners_.size()); }
	void clear_listeners();
	void notify();

	void set_parameter(Parameter& parameter) {
		parameter_ = parameter;
	}

private:	
	const UMEventType key_;
	UMListenerList listeners_;
	Parameter parameter_;
};

} // umbase
