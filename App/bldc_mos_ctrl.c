#include <stdlib.h>
#include "bldc_mos_ctrl.h"

const bldc_six_step_t six_step[6] = {
    { {0xFF, 0x00, 0x00}, {0x00, 0x00, 0xFF}, PHASE_V },
    { {0xFF, 0x00, 0x00}, {0x00, 0xFF, 0x00}, PHASE_W },
    { {0x00, 0x00, 0xFF}, {0x00, 0xFF, 0x00}, PHASE_U },
    { {0x00, 0x00, 0xFF}, {0xFF, 0x00, 0x00}, PHASE_V },
    { {0x00, 0xFF, 0x00}, {0xFF, 0x00, 0x00}, PHASE_W },
    { {0x00, 0xFF, 0x00}, {0x00, 0x00, 0xFF}, PHASE_U },
};

static void bldc_update_lower(bldc_ctrl_t *ctrl);

void bldc_change_phase(bldc_ctrl_t *ctrl)
{
    ctrl->step += ctrl->dir == CW ? 1 : -1;
    if (ctrl->step > 5) {
        ctrl->step = 0;
    } else if (ctrl->step < 0) {
        ctrl->step = 5;
    }

    bldc_close_upper_lower(ctrl);
    bldc_update_upper_duty(ctrl);
    bldc_update_lower(ctrl);
}

void bldc_close_upper_lower(bldc_ctrl_t *ctrl)
{
    ctrl->periph.hTIM_pwm->CH1CVR = 0;
    ctrl->periph.hTIM_pwm->CH2CVR = 0;
    ctrl->periph.hTIM_pwm->CH3CVR = 0;

    ctrl->periph.hGPIO_Port_UL->REG_OFF = ctrl->periph.GPIO_Pin_UL;
    ctrl->periph.hGPIO_Port_VL->REG_OFF = ctrl->periph.GPIO_Pin_VL;
    ctrl->periph.hGPIO_Port_WL->REG_OFF = ctrl->periph.GPIO_Pin_WL;
}

void bldc_update_upper_duty(bldc_ctrl_t *ctrl)
{
#warning "brake"
    ctrl->periph.hTIM_pwm->CH1CVR = (uint16_t)(ctrl->duty.value) & six_step[ctrl->step].upper_phase_duty_mask[PHASE_U];
    ctrl->periph.hTIM_pwm->CH2CVR = (uint16_t)(ctrl->duty.value) & six_step[ctrl->step].upper_phase_duty_mask[PHASE_V];
    ctrl->periph.hTIM_pwm->CH3CVR = (uint16_t)(ctrl->duty.value) & six_step[ctrl->step].upper_phase_duty_mask[PHASE_W];
}

void bldc_update_lower(bldc_ctrl_t *ctrl)
{
    ctrl->periph.hGPIO_Port_UL->REG_OFF = ctrl->periph.GPIO_Pin_UL & (~six_step[ctrl->step].lower_phase_output_mask[PHASE_U]);
    ctrl->periph.hGPIO_Port_VL->REG_OFF = ctrl->periph.GPIO_Pin_VL & (~six_step[ctrl->step].lower_phase_output_mask[PHASE_V]);
    ctrl->periph.hGPIO_Port_WL->REG_OFF = ctrl->periph.GPIO_Pin_WL & (~six_step[ctrl->step].lower_phase_output_mask[PHASE_W]);
    ctrl->periph.hGPIO_Port_UL->REG_ON = ctrl->periph.GPIO_Pin_UL & six_step[ctrl->step].lower_phase_output_mask[PHASE_U];
    ctrl->periph.hGPIO_Port_VL->REG_ON = ctrl->periph.GPIO_Pin_VL & six_step[ctrl->step].lower_phase_output_mask[PHASE_V];
    ctrl->periph.hGPIO_Port_WL->REG_ON = ctrl->periph.GPIO_Pin_WL & six_step[ctrl->step].lower_phase_output_mask[PHASE_W];
}

