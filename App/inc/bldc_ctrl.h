#ifndef _BLDC_CTRL_H_
#define _BLDC_CTRL_H_

#include <stdint.h>
#include "main.h"
#include "user_lib.h"

#define BLDC_DIR                   CW
#define BLDC_SPEED_MODE            SPEED_OPEN_LOOP

#warning "DEBUD"
#define BLDC_DUTY_MIN              (150)
#define BLDC_DUTY_MAX              (2000)
#define BLDC_DUTY_CHARGE           (10)
#define BLDC_DUTY_INC_MIN          (-10)
#define BLDC_DUTY_INC_MAX          (10)

#define BLDC_CHANGE_PHASE_NUM_FAIL (200)
#define BLDC_CHANGE_PHASE_NUM_DRAG (50)

#define BLDC_STEP_CNT_MIN_ADV      24

#define BLDC_TICK_ALIGNMEN         US_TO_TICK(50000)
#define BLDC_STEP_CNT_DRAG         US_TO_CNT(250)
#define BLDC_STEP_CNT_MAX          US_TO_CNT(2000)
#if BLDC_STEP_CNT_MAX > 65535
#error
#endif


typedef enum {
    CW = 0,
    CCW,
} bldc_dir_e;

typedef enum {
    IDLE = 0,
    ALIGNMENT,
    DRAG,
    CLOSED_LOOP,
    STALL,
} bldc_ctrl_status_e;

typedef enum {
    PHASE_U = 0,
    PHASE_V,
    PHASE_W,
    PHASE_NONE,
} bldc_phase_e;

typedef enum {
    SPEED_OPEN_LOOP = 0,
    SPEED_CLOSED_LOOP,
} bldc_speed_mode_t;

typedef struct bldc_ctrl_ {
    TIM_TypeDef *hTIM_pwm;
    TIM_TypeDef *hTIM_cnt;
    TIM_TypeDef *hTIM_input;

    uint16_t GPIO_Pin_EXTI;

    bldc_dir_e dir;
    bldc_ctrl_status_e status;
    bldc_ctrl_status_e status_prev;

    ramp_int32_t duty;

    int8_t step;
    uint32_t change_phase_ok_num;
    uint32_t change_phase_fail_num;

    uint32_t step_cnt;
    uint32_t alignment_cnt;

    bldc_speed_mode_t mode;
    float speed;
    float speed_set;
} bldc_ctrl_t;

typedef struct {
    bldc_phase_e upper_on_phase;
    bldc_phase_e lower_on_phase;
    bldc_phase_e float_phase;
} bldc_six_step_t;

extern const bldc_six_step_t six_step[6];

void bldc_ctrl_init(void);
void bldc_ctrl_task(void);

#endif

