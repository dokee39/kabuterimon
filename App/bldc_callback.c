#include "bldc_callback.h"
#include "bldc_mos_ctrl.h"
#include "bldc_zero_cross.h"
#include "bldc_tim_cnt.h"
#warning DEBUG
#include "led_task.h"

static void bldc_cnt_calc_and_delay(bldc_ctrl_t *ctrl);

void bldc_change_phase_callback(bldc_ctrl_t *ctrl)
{
    switch (ctrl->status) {
        case DRAG:
        case CLOSED_LOOP:
            bldc_mos_change_phase(ctrl);
            bldc_zero_cross_switch(ctrl);
            bldc_tim_cnt_enable(ctrl, ctrl->step_cnt_sum_filtered / BLDC_CNT_MAX_ADV);
            break;
        default:
            break;
    }
}

void bldc_zero_cross_callback(bldc_ctrl_t *ctrl)
{
    // if (ctrl->status != DRAG && ctrl->hTIM_cnt->CNT < ctrl->step_cnt_sum_filtered / BLDC_CNT_MIN_ADV) {
    // if (ctrl->hTIM_cnt->CNT < ctrl->step_cnt_sum_filtered / BLDC_CNT_MIN_ADV) {
    //     return;
    // }
    led_toggle();
    // switch (ctrl->status) {
    //     case DRAG:
    //         if (ctrl->step_cnt_sum > BLDC_DRAG_CNT_SUM_END) {
    //             uint32_t cnt_dec = ctrl->step_cnt_sum_filtered / 6 / BLDC_DRAG_CNT_DEC_ADV;
    //             for (int i = 0; i < 6; i++) {
    //                 ctrl->step_cnt[i] -= cnt_dec;
    //             }
    //             ctrl->step_cnt_sum -= cnt_dec * 6;
    //             ctrl->step_cnt_sum_filtered -= cnt_dec * 6;
    //         }
    //         // np break
    //     case CLOSED_LOOP:
    //         ctrl->change_phase_fail_cnt = 0;
    //         ctrl->change_phase_ok_cnt++;
    //         bldc_cnt_calc_and_delay(ctrl);
    //         break;
    //     default:
    //         break;
    // }
            bldc_mos_change_phase(ctrl);
            bldc_zero_cross_switch(ctrl);
}

void bldc_time_out_callback(bldc_ctrl_t *ctrl)
{
    switch (ctrl->status) {
        case DRAG:
            ctrl->change_phase_ok_cnt = 0;
            // no break
        case CLOSED_LOOP:
            ctrl->change_phase_fail_cnt++;
            bldc_cnt_calc_and_delay(ctrl);
            break;
        default:
            break;
    }
}

static void bldc_cnt_calc_and_delay(bldc_ctrl_t *ctrl)
{
    uint32_t cnt_delay;
    if (ctrl->change_phase_fail_cnt == 0) {
        cnt_delay = ctrl->step_cnt_sum_filtered / BLDC_CNT_ZERO_CROSS_ADV;
    } else {
        cnt_delay = ctrl->step_cnt_sum_filtered / BLDC_CNT_ZERO_CROSS_ADV / 2;
    }
    
    if (ctrl->step_cnt_sum_filtered < BLDC_DRAG_CNT_SUM_START) {
        ctrl->step_cnt_sum -= ctrl->step_cnt[ctrl->step];
        ctrl->step_cnt[ctrl->step] = ctrl->hTIM_cnt->CNT + cnt_delay;
        ctrl->step_cnt_sum += ctrl->step_cnt[ctrl->step];
        ctrl->step_cnt_sum_filtered = (ctrl->step_cnt_sum_filtered * 7 + ctrl->step_cnt_sum) / 8;
        // ctrl->step_cnt_sum_filtered = ctrl->step_cnt_sum;
    }
    
    bldc_zero_cross_disable(ctrl);
    bldc_tim_cnt_enable(ctrl, cnt_delay);
}

