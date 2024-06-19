#ifndef _BLDC_CONTROL_PARAM_H_
#define _BLDC_CONTROL_PARAM_H_

#include <stdint.h>

#define DUTY_MAX

#define ALIGNMENT_DUTY (300)

typedef enum {
    ALIGNMENT = 0, /*!< Alignment phase for fixing the motor to the initial phase */
    DRAG,          /*!< Strong drag gives the motor some initial velocity */
    CLOSED_LOOP,   /*!< Closed-loop sensorless control */
    BLOCKED,       /*!< Motor blocked */
    STOP,          /*!< Motor stalls */
    FAULT,         /*!< Motor failure */
} bldc_control_status_e;

typedef struct {
    bldc_control_status_e status;
    uint16_t duty;
    uint16_t duty_max;
    uint8_t phase;
} bldc_control_param_t;

#endif

