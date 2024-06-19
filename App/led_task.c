#include "led_task.h"

#include <stdint.h>
#include "ch32v20x.h"

#define LED_RUN_GROUP GPIOA
#define LED_RUN_PIN   GPIO_Pin_6

void led_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef        GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = LED_RUN_PIN;
    GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_RUN_GROUP, &GPIO_InitStruct);
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
        LED_RUN_GROUP->BCR = LED_RUN_PIN;
    }
    else
    {
        LED_RUN_GROUP->BSHR = LED_RUN_PIN;
    }
}

void led_toggle(void)
{
    ((LED_RUN_GROUP))->OUTDR ^= LED_RUN_PIN;
}
