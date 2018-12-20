#include "utility.hpp"

#include <cstdlib>
#include <iostream>

namespace utility
{

std::string getEnv(const char* key)
{
    auto value = std::getenv(key);
    return value ? std::string(value) : std::string();
}

uint32_t getGpioOffset(const char* key)
{
    auto gpio = getEnv(key);

    std::cout << std::string(key) << ":" << gpio << std::endl;
    return gpioplus::utility::aspeed::nameToOffset(gpio);
}

uint32_t getHandleGpioOffset()
{
    return getGpioOffset("control");
}

uint32_t getEventGpioOffset()
{
    return getGpioOffset("read");
}

} // namespace utility
