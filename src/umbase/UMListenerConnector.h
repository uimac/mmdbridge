/**
 * @file UMListenerConnector.h
 * a listener
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once
#include "UMMacro.h"
#include "UMListener.h"
#include "UMAny.h"
#include "UMEventType.h"
#include "UMEvent.h"
#include <vector>

namespace umbase
{

class UMListenerConnector;
typedef std::shared_ptr<UMListenerConnector> UMListenerConnectorPtr;

class UMListenerConnector
{
	DISALLOW_COPY_AND_ASSIGN(UMListenerConnector);

public:
	typedef umbase::UMAny Parameter;

	/**
	 * connect listener to events
	 */
	void connect(UMListenerPtr listener) { 
		if (!listener) return;
		UMEventList::iterator it = event_list_.begin();
		for (; it != event_list_.end(); ++it)
		{
			(*it)->add_listener(listener);
		}
		listeners_.push_back(listener);
	}

	UMListenerList& listener_list() { return listeners_; }

protected:
	UMListenerConnector() {}

	/** 
	 * get event list
	 */
	UMEventList& mutable_event_list() { return event_list_; }

private:
	UMEventList event_list_;
	UMListenerList listeners_;
};

} // umbase
