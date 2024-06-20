#ifndef _MAIN_H_
#define _MAIN_H_

#include "ch32v20x.h"

#define SYSTICK_PERIOD_US 100
#define US_TO_TICK(t) ((t) / SYSTICK_PERIOD_US)

#define LED_PIN                 GPIO_Pin_6
#define LED_PIN_PORT            GPIOA

#define PHASE_A_GPIO_LOW        GPIO_Pin_1
#define PHASE_A_GPIO_PORT_LOW   GPIOB
#define PHASE_A_GPIO_HIGH       GPIO_Pin_10
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        GPIO_Pin_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       GPIO_Pin_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        GPIO_Pin_7
#define PHASE_C_GPIO_PORT_LOW   GPIOA
#define PHASE_C_GPIO_HIGH       GPIO_Pin_8
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define INPUT_GPIO_PIN          GPIO_Pin_0
#define INPUT_GPIO_PORT         GPIOA

#endif 

