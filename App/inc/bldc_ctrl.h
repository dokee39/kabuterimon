#ifndef _BLDC_CTRL_H_
#define _BLDC_CTRL_H_

#include <stdint.h>
#include "main.h"
#include "user_lib.h"

#define BLDC_DIR                   CW
#define BLDC_SPEED_MODE            SPEED_OPEN_LOOP

#warning "DEBUD"
#define BLDC_DUTY_MIN              (100)
#define BLDC_DUTY_MAX              (2000)
#define BLDC_DUTY_INC_MIN          (-10)
#define BLDC_DUTY_INC_MAX          (10)

#define BLDC_CHANGE_PHASE_CNT_FAIL (50)
#define BLDC_CHANGE_PHASE_CNT_DRAG (20)

#define BLDC_DRAG_CNT_SUM_START    US_TO_CNT(100000) // note that (BLDC_DRAG_CNT_SUM_START / BLDC_CNT_MAX_ADV) should not bigger than 65535 
#define BLDC_DRAG_CNT_SUM_END      US_TO_CNT(8000)
#define BLDC_DRAG_CNT_DEC          (((BLDC_DRAG_CNT_SUM_START - BLDC_DRAG_CNT_SUM_END) / (BLDC_CHANGE_PHASE_CNT_DRAG)) / 6)

#define BLDC_CNT_MIN_ADV           32
#define BLDC_CNT_MAX_ADV           8
#define BLDC_CNT_ZERO_CROSS_ADV    12


typedef enum {
    CW = 0,
    CCW,
} bldc_dir_e;

typedef enum {
    IDLE = 0,
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

    uint16_t GPIO_Pin_EXTI;

    bldc_dir_e dir;
    bldc_ctrl_status_e status;
    bldc_ctrl_status_e status_prev;

    ramp_int32_t duty;

    int8_t step;
    uint32_t change_phase_ok_cnt;
    uint32_t change_phase_fail_cnt;

    uint32_t step_cnt[6];
    uint32_t step_cnt_sum;
    uint32_t step_cnt_sum_filtered;

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

