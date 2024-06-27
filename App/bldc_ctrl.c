#include "bldc_ctrl.h"
#include "bldc_mos_ctrl.h"
#include "bldc_zero_cross.h"
#include "bldc_tim_cnt.h"

bldc_ctrl_t ctrl;

static void bldc_ctrl_param_clear(bldc_ctrl_t *ctrl);
static void bldc_ctrl_param_alignment(bldc_ctrl_t *ctrl);

void bldc_ctrl_init(void)
{
    ctrl.hTIM_pwm      = TIM1;
    ctrl.hTIM_cnt      = TIM3;
    ctrl.hTIM_input    = TIM2;
    ctrl.GPIO_Pin_EXTI = EXTI_GPIO_PIN;

    ctrl.status      = IDLE;
    ctrl.status_prev = IDLE;
    ctrl.mode        = BLDC_SPEED_MODE;
    ctrl.dir         = BLDC_DIR;
    ctrl.step        = 0;
    ramp_int32_init(&ctrl.duty, BLDC_DUTY_MIN, BLDC_DUTY_INC_MIN, BLDC_DUTY_INC_MAX, BLDC_DUTY_MIN, BLDC_DUTY_MAX);

    bldc_ctrl_param_clear(&ctrl);
    bldc_mos_stop(&ctrl);
    bldc_zero_cross_disable(&ctrl);
    bldc_tim_cnt_disable(&ctrl);
}

void bldc_ctrl_task(void)
{
    if (ctrl.duty.value < ctrl.duty.value_min) {
        ctrl.status = IDLE;
    } else if (ctrl.change_phase_fail_num > BLDC_CHANGE_PHASE_NUM_FAIL) {
        ctrl.status = STALL;
    } else if (ctrl.change_phase_ok_num < BLDC_CHANGE_PHASE_NUM_DRAG) {
        if (ctrl.status_prev == IDLE) {
            ctrl.status = ALIGNMENT;
            bldc_ctrl_param_alignment(&ctrl);
            bldc_zero_cross_disable(&ctrl);
            bldc_mos_change_phase(&ctrl);
            bldc_tim_cnt_disable(&ctrl);
        }
    } else {
        ctrl.status = CLOSED_LOOP;
        if (ctrl.status_prev == DRAG) {
            ctrl.duty.value = ctrl.duty.value_min;
            
            #warning "calc speed"
        }
    }

    ctrl.status_prev = ctrl.status;
    
    switch (ctrl.status) {
        case IDLE:
            bldc_ctrl_param_clear(&ctrl);
            bldc_mos_stop(&ctrl);
            bldc_zero_cross_disable(&ctrl);
            bldc_tim_cnt_disable(&ctrl);
            break;
        case ALIGNMENT:
            if (ctrl.alignment_cnt < BLDC_TICK_ALIGNMEN) {
                ctrl.alignment_cnt++;
            } else {
                ctrl.alignment_cnt = 0;
                ctrl.status = DRAG;
                bldc_mos_change_phase(&ctrl);
                bldc_zero_cross_switch(&ctrl);
                bldc_tim_cnt_enable(&ctrl, BLDC_STEP_CNT_MAX);
            }
            break;
        case DRAG:
#warning "DEBUD"
            ctrl.duty.value = ctrl.duty.value_min;
            bldc_mos_update_duty(&ctrl);
            break;
        case CLOSED_LOOP:
#warning "DEBUD"
            bldc_mos_update_duty(&ctrl);
            break;
        case STALL:
            bldc_ctrl_param_clear(&ctrl);
            bldc_mos_stop(&ctrl);
            bldc_zero_cross_disable(&ctrl);
            bldc_tim_cnt_disable(&ctrl);
            break;
        default:
            break;
    }
    
}

static void bldc_ctrl_param_clear(bldc_ctrl_t *ctrl)
{
    ctrl->duty.value            = 0;
    ctrl->change_phase_ok_num   = 0;
    ctrl->change_phase_fail_num = 0;
    ctrl->speed                 = 0;
    ctrl->step_cnt              = 0;
}

static void bldc_ctrl_param_alignment(bldc_ctrl_t *ctrl)
{
    ctrl->duty.value            = ctrl->duty.value_min;
    ctrl->change_phase_ok_num   = 0;
    ctrl->change_phase_fail_num = 0;
    ctrl->speed                 = 0;
    ctrl->step_cnt              = BLDC_STEP_CNT_DRAG;
}
