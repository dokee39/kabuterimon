#include "bldc_callback.h"
#include "bldc_mos_ctrl.h"
#include "bldc_zero_cross.h"
#include "bldc_tim_cnt.h"
#warning DEBUG
#include "led_task.h"

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
    if (ctrl->mode != DRAG && ctrl->hTIM_cnt->CNT < ctrl->step_cnt / BLDC_STEP_CNT_MIN_ADV) {
        return;
    }
    led_toggle();
    switch (ctrl->status) {
        case DRAG:
            // np break
        case CLOSED_LOOP:
            ctrl->change_phase_fail_num = 0;
            ctrl->change_phase_ok_num++;
            ctrl->step_cnt = (ctrl->step_cnt * 63 + ctrl->hTIM_cnt->CNT) / 64;
            bldc_zero_cross_disable(ctrl);
            if (ctrl->status == DRAG) {
                bldc_tim_cnt_enable(ctrl, BLDC_STEP_CNT_DRAG);
            } else {
                bldc_tim_cnt_enable(ctrl, ctrl->step_cnt);
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
            bldc_zero_cross_disable(ctrl);
            bldc_tim_cnt_enable(ctrl, BLDC_STEP_CNT_MAX);
            break;
        default:
            break;
    }
}

