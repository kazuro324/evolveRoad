#include "EventHandler.h"

EventHandlerNoArgs::EventHandlerNoArgs()
{
	eventList.clear();
}

EventHandlerNoArgs::~EventHandlerNoArgs()
{
	eventList.clear();
}

void EventHandlerNoArgs::addEvent(const FuncType& _event)
{
	if (!this)
	{
		return;
	}
	for (const auto& e : eventList)
	{
		if (e.target_type() == _event.target_type())
		{
			return;
		}
	}
	eventList.push_back(_event);
}

void EventHandlerNoArgs::removeEvent(const FuncType& _event)
{
	if (!this)
	{
		return;
	}
	eventList.erase(
		std::remove_if(eventList.begin(), eventList.end(),
			[&](const FuncType& e) { return e.target_type() == _event.target_type(); }),
		eventList.end()
	);
}

void EventHandlerNoArgs::clearEvents()
{
	eventList.clear();
}

void EventHandlerNoArgs::Fire()
{
	if (!this)
	{
		return;
	}
	for (const auto& e : eventList)
	{
		e();
	}
}
