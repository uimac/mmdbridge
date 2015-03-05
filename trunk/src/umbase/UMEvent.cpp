/**
 * @file UMEvent.cpp
 * a event
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMEvent.h"
#include "UMListener.h"

namespace umbase
{

/**
 * notify to all listeners
 */
void UMEvent::notify()
{
	UMListenerList::iterator it = listeners_.begin();
	for (; it != listeners_.end(); ++it)
	{
		if (UMListenerPtr listener = (*it).lock())
		{
			listener->update(key_, parameter_);
		}
	}
}

/**
 * add listener (attach)
 */
void UMEvent::add_listener(UMListenerWeakPtr listener)
{
	listeners_.push_back(listener);
}

/**
 * delete objserver (detach)
 */
void UMEvent::delete_listener(UMListenerWeakPtr listener)
{
	UMListenerList::iterator it = listeners_.begin();
	for (int i = static_cast<int>(listeners_.size() - 1); i >= 0; --i)
	{
		if (listeners_.at(i).lock() == listener.lock())
		{
			listeners_.erase(listeners_.begin() + i);
		}
	}
}

/**
 * clear all listeners
 */
void UMEvent::clear_listeners()
{
	listeners_.clear();
}

} // umbase
