# inspur-uuid

Adapter for OpenBmc  [phosphor-led-sysfs](https://github.com/openbmc/phosphor-led-sysfs)

we need 2 gpios to speak with inspur identify-led.

> gpio_control   1->0->1(pulse,keep 0 > 50ms) toggle the led
> gpio_read      read only, 1 means Off, 0 means On 

