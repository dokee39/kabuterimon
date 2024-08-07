#include "bldc_callback.h"
#include "bldc_mos_ctrl.h"
#include "bldc_zero_cross.h"
#include "bldc_tim_cnt.h"
#include "user_lib.h"

void bldc_change_phase_callback(bldc_ctrl_t *ctrl)
{
    switch (ctrl->status) {
        case DRAG:
        case CLOSED_LOOP:
            bldc_mos_change_phase(ctrl);
            bldc_zero_cross_switch(ctrl);
            bldc_tim_cnt_enable(ctrl, BLDC_STEP_CNT_MAX);
            break;
        default:
            break;
    }
}

void bldc_zero_cross_callback(bldc_ctrl_t *ctrl)
{
    if (ctrl->hTIM_cnt->CNT < ctrl->step_cnt / BLDC_STEP_CNT_MIN_ADV) {
        return;
    }
    switch (ctrl->status) {
        case DRAG:
            // no break
        case CLOSED_LOOP:
            ctrl->change_phase_fail_num = 0;
            ctrl->change_phase_ok_num++;
            ctrl->step_cnt = (ctrl->step_cnt * 31 + ctrl->hTIM_cnt->CNT) / 32;
            bldc_zero_cross_disable(ctrl);
            if (ctrl->status == DRAG) {
                bldc_tim_cnt_enable(ctrl, BLDC_STEP_CNT_DRAG);
            } else {
                bldc_tim_cnt_enable(ctrl, ctrl->step_cnt / 2);
            }
            break;
        default:
            break;
    }
}

void bldc_time_out_callback(bldc_ctrl_t *ctrl)
{
    switch (ctrl->status) {
        case DRAG:
            ctrl->change_phase_ok_num = 0;
            // no break
        case CLOSED_LOOP:
            ctrl->change_phase_fail_num++;
            ctrl->step_cnt = (ctrl->step_cnt * 63 + ctrl->hTIM_cnt->CNT) / 64;
            bldc_change_phase_callback(ctrl);
            break;
        default:
            break;
    }
}

void bldc_pwm_input_callback(bldc_ctrl_t *ctrl)
{
    uint32_t period = TIM_GetCapture1(ctrl->hTIM_input) + 1;
    uint32_t high   = TIM_GetCapture2(ctrl->hTIM_input) + 1;
    uint32_t freq   = 1000000 / period;
    uint32_t duty   = 0;

    if( (freq > 20) && (freq < 400) && (high < 3000) && (high > 1000) ) {
        if(high >= 2000) {
            high = 1000;
        } else {
            high -= 1000;
        }
        duty = high * BLDC_DUTY_MAX / 1000;
        if (duty < ctrl->duty.value_min && ctrl->status == IDLE) {
            ctrl->duty.value = 0;
        } else if (ctrl->status == DRAG) {
            ctrl->duty.value = ctrl->duty.value_min;
        } else {
            ramp_int32_update(&ctrl->duty, duty);
        }
    } else {
        ctrl->status = IDLE;
        ctrl->status_prev = IDLE;
        ctrl->duty.value = 0;
    }

}

void bldc_pwm_input_timeout_callback(bldc_ctrl_t *ctrl)
{
    ctrl->status = IDLE;
    ctrl->status_prev = IDLE;
    ctrl->duty.value = 0;
}


