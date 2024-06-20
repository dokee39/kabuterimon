#ifndef _BLDC_CONTROL_PARAM_H_
#define _BLDC_CONTROL_PARAM_H_

#include <stdint.h>
#include "main.h"

#define REG_ON BSHR
#define REG_OFF BCR

#define DUTY_MAX

#define ALIGNMENT_DUTY (300)

typedef enum {
    CW = 0,
    CCW,
} bldc_dir_e;

typedef enum {
    ALIGNMENT = 0, /*!< Alignment phase for fixing the motor to the initial phase */
    DRAG,          /*!< Strong drag gives the motor some initial velocity */
    CLOSED_LOOP,   /*!< Closed-loop sensorless control */
    BLOCKED,       /*!< Motor blocked */
    STOP,          /*!< Motor stalls */
    FAULT,         /*!< Motor failure */
} bldc_control_status_e;

typedef enum {
    PHASE_U = 0,
    PHASE_V,
    PHASE_W,
} bldc_phase_e;

typedef struct {
    bldc_dir_e dir;
    bldc_control_status_e status;

    TIM_TypeDef *hTIM;

    GPIO_TypeDef *hGPIO_Port_UL;
    uint16_t GPIO_Pin_UL;
    GPIO_TypeDef *hGPIO_Port_VL;
    uint16_t GPIO_Pin_VL;
    GPIO_TypeDef *hGPIO_Port_WL;
    uint16_t GPIO_Pin_WL;

    uint16_t duty;
    uint16_t duty_max;

    int8_t step;
} bldc_control_param_t;

#endif

