# inspur-uuid

Adapter for OpenBmc  [phosphor-led-sysfs](https://github.com/openbmc/phosphor-led-sysfs)

 The phsophor-led-sysfs needs only one pin
    to control the led(linux led subsystem).
    But inspur's motherbaord needs 2 pins
    to manager the identify-led(only identify-led)
    
    gpio_control 1-0-1 keep 0>50ms toggle the led
    gpio_read    get the led status(0 means on 1 means off)

