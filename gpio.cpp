#include "gpio.hpp"

#include "utility.hpp"

#include <iostream>
#include <thread>

namespace inspur
{
namespace identify
{

static constexpr auto consumer_label = "inspur-identify-led";
static constexpr auto PULS_TIME_MS = 100;
static constexpr auto BLINK_TIME_MS = 500;

GpioIdentify::GpioIdentify(EventPtr& event) : event(event)
{

    gpioHandle = buildGpioHandle(0, utility::getHandleGpioOffset());
    gpioEvent = buildGpioEvent(0, utility::getEventGpioOffset());
    initTimer();
}

void GpioIdentify::initTimer()
{
    sd_event_source* source = nullptr;
    auto rc = sd_event_add_time(event.get(), &source, CLOCK_MONOTONIC,
                                UINT64_MAX, 0, timeoutHandler, this);
    if (rc < 0)
    {
        std::cout << "sd_event_add_time error rc=" << rc << std::endl;
    }

    eventSource.reset(source);
}

std::unique_ptr<gpioplus::Handle> GpioIdentify::buildGpioHandle(uint32_t id,
                                                                uint32_t line)
{
    try
    {
        gpioplus::Chip chip(id);
        gpioplus::HandleFlags flags(chip.getLineInfo(line).flags);
        flags.output = true;
        std::vector<gpioplus::Handle::Line> lines = {{line, 0}};
        return std::make_unique<gpioplus::Handle>(chip, lines, flags,
                                                  consumer_label);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

std::unique_ptr<gpioplus::Event> GpioIdentify::buildGpioEvent(uint32_t id,
                                                              uint32_t line)
{
    try
    {
        gpioplus::Chip chip(id);
        gpioplus::HandleFlags handleflags(chip.getLineInfo(line).flags);
        handleflags.output = false;
        gpioplus::EventFlags eventflags;
        eventflags.falling_edge = true;
        eventflags.rising_edge = true;
        return std::make_unique<gpioplus::Event>(chip, line, handleflags,
                                                 eventflags, consumer_label);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return nullptr;
    }
}

int GpioIdentify::getEventFd()
{
    using namespace gpioplus::internal;
    return *gpioEvent->getFd();
}

void GpioIdentify::cleanEventData()
{
    gpioEvent->read();
}

int GpioIdentify::timeoutHandler(sd_event_source* source, uint64_t usec,
                                 void* userdata)
{
    auto gpioIdentify = static_cast<GpioIdentify*>(userdata);
    gpioIdentify->toggleIdentifyLed();
    gpioIdentify->setTimeout();

    return 0;
}

void GpioIdentify::stopTimer()
{
    auto rc = sd_event_source_set_enabled(eventSource.get(), SD_EVENT_OFF);
    if (rc < 0)
    {
        std::cout << "sd_event_source_set_enabled error rc=" << rc << std::endl;
    }
}

void GpioIdentify::startTimer()
{
    setTimeout();
    auto rc = sd_event_source_set_enabled(eventSource.get(), SD_EVENT_ON);
    if (rc < 0)
    {
        std::cout << "sd_event_source_set_enabled error rc=" << rc << std::endl;
    }
}

void GpioIdentify::setTimeout()
{
    using namespace std::chrono;

    auto now = steady_clock::now().time_since_epoch();
    auto expireTime =
        duration_cast<microseconds>(now) + milliseconds(BLINK_TIME_MS);

    auto rc = sd_event_source_set_time(eventSource.get(), expireTime.count());

    if (rc < 0)
    {
        std::cout << "sd_event_source_set_time error rc=" << rc << std::endl;
    }
}

void GpioIdentify::setIdentifyLedState(IdentifyLedState state)
{
    stopTimer();
    auto current = getIdentifyLedState();
    if (state == current)
        return;
    toggleIdentifyLed();
}

void GpioIdentify::toggleIdentifyLed()
{
    uint8_t v = 0;
    gpioHandle->setValues({v});
    std::this_thread::sleep_for(std::chrono::milliseconds(PULS_TIME_MS));
    v = 1;
    gpioHandle->setValues({v});
}

void GpioIdentify::blinkIdentifyLed()
{
    startTimer();
}

IdentifyLedState GpioIdentify::getIdentifyLedState()
{
    if (gpioEvent->getValue() == 0)
        return IdentifyLedState::Off;
    return IdentifyLedState::On;
}

} // namespace identify
} // namespace inspur
