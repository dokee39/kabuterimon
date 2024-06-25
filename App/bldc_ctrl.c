#include "bldc_ctrl.h"
#include "bldc_mos_ctrl.h"
#include "bldc_zero_cross.h"
#include "bldc_tim_cnt.h"

bldc_ctrl_t ctrl;

static void bldc_ctrl_param_clear(bldc_ctrl_t *ctrl);
static void bldc_ctrl_param_drag(bldc_ctrl_t *ctrl);

void bldc_ctrl_init(void)
{
    ctrl.hTIM_pwm      = TIM1;
    ctrl.hTIM_cnt      = TIM3;
    ctrl.GPIO_Pin_EXTI = EXTI_GPIO_PIN;

    ctrl.status = IDLE;
    ctrl.status_prev = IDLE;
    ctrl.mode   = BLDC_SPEED_MODE;
    ctrl.dir    = BLDC_DIR;
    ctrl.step   = 0;
    ramp_int32_init(&ctrl.duty, BLDC_DUTY_MIN, BLDC_DUTY_INC_MIN, BLDC_DUTY_INC_MAX, BLDC_DUTY_MIN, BLDC_DUTY_MAX);

    bldc_ctrl_param_clear(&ctrl);
    bldc_mos_stop(&ctrl);
    bldc_zero_cross_disable(&ctrl);
    bldc_tim_cnt_disable(&ctrl);
}

#warning "DEBUD"
uint32_t duty = 300;

void bldc_ctrl_task(void)
{
    if (duty < ctrl.duty.value_min) {
        ctrl.status = IDLE;
    } else if (ctrl.change_phase_fail_cnt > BLDC_CHANGE_PHASE_CNT_FAIL) {
        // ctrl.status = STALL;
    } else if (ctrl.change_phase_ok_cnt < BLDC_CHANGE_PHASE_CNT_DRAG) {
        ctrl.status = DRAG;
        if (ctrl.status_prev == IDLE) {
            bldc_ctrl_param_drag(&ctrl);
            bldc_mos_change_phase(&ctrl);
            bldc_zero_cross_switch(&ctrl);
            bldc_tim_cnt_enable(&ctrl, ctrl.step_cnt_sum_filtered / BLDC_CNT_MAX_ADV);
        }
    } else {
        ctrl.status = CLOSED_LOOP;
        if (ctrl.status_prev == DRAG) {
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
        case DRAG:
#warning "DEBUD"
            ctrl.duty.value = ctrl.duty.value_min;
            bldc_mos_update_duty(&ctrl);
            uint32_t sum = 0;
        for (int i = 0; i < 6; i++) {
            sum += ctrl.step_cnt[i];
        }
        ctrl.step_cnt_sum_filtered = ctrl.step_cnt_sum = sum;
        
            break;
        case CLOSED_LOOP:
#warning "DEBUD"
            ctrl.duty.value = duty;
            bldc_mos_update_duty(&ctrl);
            break;
        case STALL:
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
    ctrl->change_phase_ok_cnt   = 0;
    ctrl->change_phase_fail_cnt = 0;
    ctrl->speed                 = 0;

    for (int i = 0; i < 5; i++) {
        ctrl->step_cnt[i] = 0;
    }
    ctrl->step_cnt_sum          = 0;
    ctrl->step_cnt_sum_filtered = 0;
}

static void bldc_ctrl_param_drag(bldc_ctrl_t *ctrl)
{
    ctrl->duty.value            = ctrl->duty.value_min;
    ctrl->change_phase_ok_cnt   = 0;
    ctrl->change_phase_fail_cnt = 0;
    ctrl->speed                 = 0;

    for (int i = 0; i < 5; i++) {
        ctrl->step_cnt[i] = BLDC_DRAG_CNT_SUM_START / 6;
    }
    ctrl->step_cnt_sum          = ctrl->step_cnt[0] * 6;
    ctrl->step_cnt_sum_filtered = ctrl->step_cnt_sum;
}

