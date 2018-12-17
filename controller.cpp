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
#include <algorithm>
#include "physical.hpp"
#include "config.h"
#include <systemd/sd-event.h>


int main(int argc, char** argv)
{

    auto name = "inspur_identify";

    // Unique bus name representing a single LED.
    auto busName =  std::string(BUSNAME) + '.' + name;
    auto objPath =  std::string(OBJPATH) + '/' + name;

    // Get a handle to system dbus.
    auto bus = std::move(sdbusplus::bus::new_default());

    // Add systemd object manager.
    sdbusplus::server::manager::manager(bus, objPath.c_str());

    sd_event *event = nullptr;

    auto rc = sd_event_default(&event);
    if(rc < 0){
	    std::cout << "sd_event_default rc=" << rc << std::endl;
	    return rc;
    }

    inspur::identify::EventPtr eventP{event};
    event = nullptr;


    
    // Create the Physical LED objects for directing actions.
    // Need to save this else sdbusplus destructor will wipe this off.
    phosphor::led::Physical led(bus, objPath,eventP);
    std::cout << "objPath:"<<objPath << std::endl;
    /** @brief Claim the bus */
    bus.request_name(busName.c_str());

    /** @brief Wait for client requests */
    while(true)
    {
        // Handle dbus message / signals discarding unhandled
        bus.process_discard();
        bus.wait();
    }
    return 0;
}
