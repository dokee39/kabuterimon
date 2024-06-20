#include <stdint.h>
#include "led_task.h"
#include "main.h"

void led_init(void)
{
    led_control(LED_ON);
}

void led_task(void)
{
    led_toggle();
}

void led_control(LED_STATUS_enum state)
{
    if(state == LED_ON)
    {
        LED_PIN_PORT->BCR = LED_PIN;
    }
    else
    {
        LED_PIN_PORT->BSHR = LED_PIN;
    }
}

void led_toggle(void)
{
    ((LED_PIN_PORT))->OUTDR ^= LED_PIN;
}
