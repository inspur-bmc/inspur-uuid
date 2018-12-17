#pragma once
#include <systemd/sd-event.h>
#include <sdbusplus/message.hpp>
#include <map>
#include <memory>
#include <string>

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
using EventPtr = std::unique_ptr<sd_event,EventDeleter>;

enum class IdentifyLedState{
	On,
	Off,
	Blink,
};

class GpioIdentify{

public:
	explicit GpioIdentify(EventPtr &event):event(event) {}

	void setIdentifyLed(IdentifyLedState state);

	IdentifyLedState getIdentifyLedState();

	void blinkIdentifyLed();
private:
	std::string pinControl;
	std::string pinRead;

	IdentifyLedState State;
	EventPtr &event;
};

}
}
