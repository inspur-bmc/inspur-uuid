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
namespace phosphor
{
namespace led
{

/** @brief Populates key parameters */
void Physical::setInitialState()
{
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
        return blinkOperation();
    }

    // Transition TO Stable states.
    if(request == Action::On || request == Action::Off)
    {
        return stableStateOperation(request);
    }
    return;
}

/** @brief Either TurnON -or- TurnOFF */
void Physical::stableStateOperation(Action action)
{
    return;
}

/** @brief BLINK the LED */
void Physical::blinkOperation()
{
    return;
}

} // namespace led
} // namespace phosphor
