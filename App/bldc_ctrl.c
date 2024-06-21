#include "bldc_ctrl.h"
#include "bldc_mos_ctrl.h"
#include "bldc_zero_cross.h"
#include "bldc_tim_cnt.h"

bldc_ctrl_t ctrl;

static void bldc_ctrl_param_clear(bldc_ctrl_t *ctrl);

void bldc_ctrl_init(void)
{
    ctrl.periph.hTIM_pwm      = TIM1;
    ctrl.periph.hTIM_cnt      = TIM3;
    ctrl.periph.GPIO_Pin_UL   = PHASE_A_GPIO_LOW;
    ctrl.periph.GPIO_Pin_VL   = PHASE_B_GPIO_LOW;
    ctrl.periph.GPIO_Pin_WL   = PHASE_C_GPIO_LOW;
    ctrl.periph.hGPIO_Port_UL = PHASE_A_GPIO_PORT_LOW;
    ctrl.periph.hGPIO_Port_VL = PHASE_B_GPIO_PORT_LOW;
    ctrl.periph.hGPIO_Port_WL = PHASE_C_GPIO_PORT_LOW;
    ctrl.periph.GPIO_Pin_EXTI = EXTI_GPIO_PIN;

    ctrl.status = IDLE;
    ctrl.status_prev = IDLE;
    ctrl.mode   = BLDC_SPEED_MODE;
    ctrl.dir    = BLDC_DIR;
    ctrl.step   = 0;
    ramp_int32_init(&ctrl.duty, BLDC_DUTY_MIN, BLDC_DUTY_INC_MIN, BLDC_DUTY_INC_MAX, BLDC_DUTY_MIN, BLDC_DUTY_MAX);
    
    ctrl.param_clear=  bldc_ctrl_param_clear;
    
    ctrl.param_clear(&ctrl);
}

void bldc_ctrl_task(void)
{
    if (ctrl.duty.value < ctrl.duty.value_min) {
        ctrl.status = IDLE;
    } else if (ctrl.change_phase_fail_cnt > BLDC_CHANGE_PHASE_CNT_FAIL) {
        ctrl.status = STALL;
    } else if (ctrl.change_phase_ok_cnt < BLDC_CHANGE_PHASE_CNT_DRAG) {
        ctrl.status = DRAG;
        if (ctrl.status_prev == IDLE) {
            ctrl.step_cnt_sum_filtered = BLDC_DRAG_CNT_SUM_START;
            #warning "start tim & init cnt"
        }
    } else {
        ctrl.status = CLOSED_LOOP;
    }

    ctrl.status_prev = ctrl.status;
    
    switch (ctrl.status) {
        case IDLE:
            ctrl.param_clear(&ctrl);
            bldc_close_upper_lower(&ctrl);
            bldc_zero_cross_disable(&ctrl);
            break;
        case DRAG:
            ctrl.duty.value = ctrl.duty.value_min;
            break;
        case CLOSED_LOOP:
            break;
        case STALL:
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
    ctrl->step_cnt_sum          = 0;
    ctrl->speed                 = 0;

    for (int i = 0; i < 5; i++) {
        ctrl->step_cnt[i] = 0;
    }
}

void bldc_ctrl_zero_cross_callback(bldc_ctrl_t *ctrl)
{
    if (ctrl->status == DRAG || ctrl->status == CLOSED_LOOP) {
        uint32_t temp_cnt = ctrl->periph.hTIM_cnt->CNT;

        if (temp_cnt < ctrl->step_cnt_sum_filtered / BLDC_CNT_MIN_ADV) {
            return;
        }

        uint32_t cnt_delay = ctrl->step_cnt_sum_filtered / BLDC_CNT_ZERO_CROSS_ADV;

        ctrl->step_cnt_sum -= ctrl->step_cnt[ctrl->step];
        ctrl->step_cnt[ctrl->step] = temp_cnt + cnt_delay;
        ctrl->step_cnt_sum += ctrl->step_cnt[ctrl->step];
        ctrl->step_cnt_sum_filtered = (ctrl->step_cnt_sum_filtered * 7 + ctrl->step_cnt_sum) >> 3;

        bldc_zero_cross_disable(ctrl);
        bldc_tim_cnt_enable(ctrl, cnt_delay);
    }
}

void bldc_ctrl_change_phase_callback(bldc_ctrl_t *ctrl)
{
    if (ctrl->status == DRAG || ctrl->status == CLOSED_LOOP) {
        ctrl->change_phase_fail_cnt = 0;
        ctrl->change_phase_ok_cnt++;

        bldc_change_phase(ctrl);
        bldc_zero_cross_switch(ctrl);
        bldc_tim_cnt_enable(ctrl, ctrl->step_cnt_sum_filtered / BLDC_CNT_MAX_ADV);
    }
}

void bldc_ctrl_time_out_callback(bldc_ctrl_t *ctrl)
{
    switch (ctrl->status) {
        case DRAG:
            ctrl->change_phase_ok_cnt = 0;
            // no break
        case CLOSED_LOOP:
            ctrl->change_phase_fail_cnt++;
            bldc_change_phase(ctrl);
            bldc_zero_cross_switch(ctrl);
            bldc_tim_cnt_enable(ctrl, ctrl->step_cnt_sum_filtered / BLDC_CNT_ZERO_CROSS_ADV);
            break;
        default:
            break;
    }
}

