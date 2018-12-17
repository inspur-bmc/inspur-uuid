#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/object.hpp>
#include <xyz/openbmc_project/Led/Physical/server.hpp>

#include "gpio.hpp"


namespace phosphor
{
namespace led
{

using namespace inspur::identify;

class Physical : public sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Led::server::Physical>
{
    public:
        Physical() = delete;
        ~Physical() = default;
        Physical(const Physical&) = delete;
        Physical& operator=(const Physical&) = delete;
        Physical(Physical&&) = delete;
        Physical& operator=(Physical&&) = delete;

        /** @brief Constructs LED object. Argument 'true' says that we hold off
         *   from sending the signals since we need to do some house keeping and
         *   only when we finish that, we are considered active and can then
         *   broadcast the signal.
         *
         * @param[in] bus       - system dbus handler
         * @param[in] objPath   - The Dbus path that hosts physical LED
         */
        Physical(sdbusplus::bus::bus& bus,
                const std::string& objPath,EventPtr &event) :
            sdbusplus::server::object::object<
                sdbusplus::xyz::openbmc_project::Led::server::Physical>(
                        bus, objPath.c_str(), true),gpioIdentify(event)
        {
            // Suppose this is getting launched as part of BMC reboot, then we
            // need to save what the micro-controller currently has.
	    //
            setInitialState();
	
            // We are now ready.
            emit_object_added();
        }

        /** @brief Overloaded State Property Setter function
         *
         *  @param[in] value   -  One of OFF / ON / BLINK
         *  @return            -  Success or exception thrown
         */
        Action state(Action value) override;

    private:

	inspur::identify::GpioIdentify gpioIdentify;

        void setInitialState();

        /** @brief Applies the user triggered action on the LED
         *   by writing to sysfs
         *
         *  @param [in] current - Current state of LED
         *  @param [in] request - Requested state
         *
         *  @return None
         */
        void driveLED(Action current, Action request);

        /** @brief Sets the LED to either ON or OFF state
         *
         *  @param [in] action - Requested action. Could be OFF or ON
         *  @return None
         */
        void stableStateOperation(Action action);

        /** @brief Sets the LED to BLINKING
         *
         *  @return None
         */
        void blinkOperation();

};

} // namespace led
}
