#pragma once

#include <systemd/sd-event.h>
#include <sdbusplus/message.hpp>


namespace inspur
{
namespace identify
{

struct EventDeleter{
	void operator()(sd_event *event) const  
	{
		event = sd_event_unref(event);
	}
};

struct EventSourceDeleter{
	void operator()(sd_event_source *source) const
	{
		sd_event_source_unref(source);
	}
};

using EventPtr = std::unique_ptr<sd_event,EventDeleter>;
using EventSourcePtr = std::unique_ptr<sd_event_source ,EventSourceDeleter>;

}
}
