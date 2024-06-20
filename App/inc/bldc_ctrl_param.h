#ifndef _BLDC_CTRL_PARAM_H_
#define _BLDC_CTRL_PARAM_H_

#include <stdint.h>
#include "main.h"

#define REG_ON BSHR
#define REG_OFF BCR

#define BLDC_DIR CW
#define BLDC_DUTY_MAX (10000)

#define ALIGNMENT_DUTY_RATIO (0.01f)
#define ALIGNMENT_TIME       (1000000)

typedef enum {
    CW = 0,
    CCW,
} bldc_dir_e;

typedef enum {
    IDLE = 0,      /*!< Motor idle */
    ALIGNMENT,     /*!< Alignment phase for fixing the motor to the initial phase */
    DRAG,          /*!< Strong drag gives the motor some initial velocity */
    CLOSED_LOOP,   /*!< Closed-loop sensorless ctrl */
    BLOCKED,       /*!< Motor blocked */
    STOP,          /*!< Motor stalls */
    FAULT,         /*!< Motor failure */
} bldc_ctrl_status_e;

typedef enum {
    PHASE_U = 0,
    PHASE_V,
    PHASE_W,
    PHASE_NONE,
} bldc_phase_e;

typedef struct {

} bldc_periph_t;

typedef struct bldc_ctrl_param_ {
    bldc_dir_e dir;
    bldc_ctrl_status_e status;

    TIM_TypeDef *hTIM;

    GPIO_TypeDef *hGPIO_Port_UL;
    uint16_t GPIO_Pin_UL;
    GPIO_TypeDef *hGPIO_Port_VL;
    uint16_t GPIO_Pin_VL;
    GPIO_TypeDef *hGPIO_Port_WL;
    uint16_t GPIO_Pin_WL;

    int8_t step;

    uint16_t duty;
    uint16_t duty_max;
    uint16_t alignment_duty;

    uint32_t step_time[6];
    uint32_t step_time_sum;
    uint32_t alignment_time;

    float speed;
    float speed_set;

    void (*clear)(struct bldc_ctrl_param_ *);
} bldc_ctrl_param_t;

#endif

