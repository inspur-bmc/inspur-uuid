/**
 * Copyright © 2016 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <string>
#include "physical.hpp"
#include "gpio.hpp"

namespace phosphor
{
namespace led
{

using namespace inspur::identify;
/** @brief Populates key parameters */
void Physical::setInitialState()
{
	auto current = gpioIdentify.getIdentifyLedState();
	if(current == IdentifyLedState::On){
		sdbusplus::xyz::openbmc_project::Led::server
			::Physical::state(Action::On);
	}else{
		sdbusplus::xyz::openbmc_project::Led::server
			::Physical::state(Action::Off);
	}
}

/** @brief Overloaded State Property Setter function */
auto Physical::state(Action value) -> Action
{
    // Obtain current operation
    auto current = sdbusplus::xyz::openbmc_project::Led::server
                                   ::Physical::state();

    // Update requested operation into base class
    auto requested = sdbusplus::xyz::openbmc_project::Led::server
                                   ::Physical::state(value);

    // Apply the action.
    driveLED(current, requested);

    return value;
}

/** @brief apply action on the LED */
void Physical::driveLED(Action current, Action request)
{
    if (current == request)
    {
        // Best we can do here is ignore.
        return;
    }

    // Transition TO Blinking state
    if (request == Action::Blink)
    {
        return gpioIdentify.blinkIdentifyLed();
    }

    // Transition TO Stable states.
    if(request == Action::On)
    {
        return gpioIdentify.setIdentifyLed(IdentifyLedState::On);
    }
    return gpioIdentify.setIdentifyLed(IdentifyLedState::Off);
}

} // namespace led
} // namespace phosphor
