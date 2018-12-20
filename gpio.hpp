#pragma once
#include "event.hpp"

#include <systemd/sd-event.h>

#include <gpioplus/chip.hpp>
#include <gpioplus/event.hpp>
#include <gpioplus/handle.hpp>
#include <map>
#include <memory>
#include <sdbusplus/message.hpp>
#include <string>

namespace inspur
{
namespace identify
{

enum class IdentifyLedState
{
    On,
    Off,
};

class GpioIdentify
{

  public:
    explicit GpioIdentify(EventPtr& event);

    void blinkIdentifyLed();

    IdentifyLedState getIdentifyLedState();

    void setIdentifyLedState(IdentifyLedState state);

    int getEventFd();
    void cleanEventData();

  private:
    int gpioHandleCurrentValue;

    std::unique_ptr<gpioplus::Handle> gpioHandle;
    std::unique_ptr<gpioplus::Event> gpioEvent;

    std::unique_ptr<gpioplus::Handle> buildGpioHandle(uint32_t chip,
                                                      uint32_t offset);
    std::unique_ptr<gpioplus::Event> buildGpioEvent(uint32_t chip,
                                                    uint32_t offset);

    void toggleIdentifyLed();
    void initTimer();
    void startTimer();
    void setTimeout();
    static int timeoutHandler(sd_event_source* source, uint64_t usec,
                              void* userdata);
    void stopTimer();

    EventPtr& event;
    EventSourcePtr eventSource;
};

} // namespace identify
} // namespace inspur
