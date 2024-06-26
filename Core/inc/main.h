#ifndef _MAIN_H_
#define _MAIN_H_

#include "ch32v20x.h"

#define SYSTICK_PERIOD_US       (100)
#define US_TO_TICK(t)           ((t) / SYSTICK_PERIOD_US)
#define TIM_CNT_PERIOD_1_10_US  (1)
#define US_TO_CNT(t)            ((t) * 10 / TIM_CNT_PERIOD_1_10_US)

#define LED_TASK_INTERVAL       (250000)
#define BLDC_CTRL_TASK_INTERVAL (100)

#define LED_PIN                 GPIO_Pin_6
#define LED_PIN_PORT            GPIOA

#define PHASE_A_GPIO_LOW        GPIO_Pin_7
#define PHASE_A_GPIO_PORT_LOW   GPIOA
#define PHASE_A_GPIO_HIGH       GPIO_Pin_8
#define PHASE_A_GPIO_PORT_HIGH  GPIOA

#define PHASE_B_GPIO_LOW        GPIO_Pin_0
#define PHASE_B_GPIO_PORT_LOW   GPIOB
#define PHASE_B_GPIO_HIGH       GPIO_Pin_9
#define PHASE_B_GPIO_PORT_HIGH  GPIOA

#define PHASE_C_GPIO_LOW        GPIO_Pin_1
#define PHASE_C_GPIO_PORT_LOW   GPIOB
#define PHASE_C_GPIO_HIGH       GPIO_Pin_10
#define PHASE_C_GPIO_PORT_HIGH  GPIOA

#define INPUT_GPIO_PIN          GPIO_Pin_0
#define INPUT_GPIO_PORT         GPIOA

#define EXTI_GPIO_PIN           GPIO_Pin_2

#define REG_ON                  BSHR
#define REG_OFF                 BCR

#define OPA_CHANNEL             (OPA->CR)
#define OPA_SELECT_U            (0x0001)
#define OPA_SELECT_V            (0x0010)
#define OPA_SELECT_W            (0x0050)
#define OPA_SELECT_NONE         (0x0000)

#endif 

