
#include <iostream>

#include "gpio.hpp"

namespace inspur
{
namespace identify
{

void GpioIdentify::setIdentifyLed(IdentifyLedState state)
{
	State = state;
	std::cout <<"setIdentifyLed" << std::endl;
}

void GpioIdentify::blinkIdentifyLed()
{
	std::cout << "blinkIdentifyLed" << std::endl;
}

IdentifyLedState GpioIdentify::getIdentifyLedState()
{
	std::cout << "getIdentifyLedState" << std::endl;
	
	return State;
}


} //namespace identify
} //namespace inspur
