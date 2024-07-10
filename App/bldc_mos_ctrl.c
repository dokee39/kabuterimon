#include "bldc_mos_ctrl.h"

#define offset_of(type, member) (unsigned int) & ((type *)0)->member

#define CHxCVR(hTIM, PHASE) (*(uint16_t *)((char *)(hTIM) + CHxCVR_OFFSET[(PHASE)]))

static const unsigned int CHxCVR_OFFSET[3] = {
    offset_of(TIM_TypeDef, CH1CVR),
    offset_of(TIM_TypeDef, CH2CVR),
    offset_of(TIM_TypeDef, CH3CVR),
};

static const uint16_t CCER_SET[4] = { 0x055D, 0x05D5, 0x0D55, 0x0555 };

const bldc_six_step_t six_step[6] = {
    { PHASE_U, PHASE_W, PHASE_V },
    { PHASE_U, PHASE_V, PHASE_W },
    { PHASE_W, PHASE_V, PHASE_U },
    { PHASE_W, PHASE_U, PHASE_V },
    { PHASE_V, PHASE_U, PHASE_W },
    { PHASE_V, PHASE_W, PHASE_U },
};

void bldc_mos_change_phase(bldc_ctrl_t *ctrl)
{
    ctrl->step += ctrl->dir == CW ? 1 : -1;
    if (ctrl->step > 5) {
        ctrl->step = 0;
    } else if (ctrl->step < 0) {
        ctrl->step = 5;
    }

    ctrl->hTIM_pwm->CH1CVR = 0;
    ctrl->hTIM_pwm->CH2CVR = 0;
    ctrl->hTIM_pwm->CH3CVR = 0;
    ctrl->hTIM_pwm->CCER = CCER_SET[six_step[ctrl->step].float_phase];
    CHxCVR(ctrl->hTIM_pwm, six_step[ctrl->step].upper_on_phase) = ctrl->duty.value;
}

void bldc_mos_update_duty(bldc_ctrl_t * ctrl)
{
    CHxCVR(ctrl->hTIM_pwm, six_step[ctrl->step].upper_on_phase) = ctrl->duty.value;
    CHxCVR(ctrl->hTIM_pwm, six_step[ctrl->step].lower_on_phase) = 0;
    CHxCVR(ctrl->hTIM_pwm, six_step[ctrl->step].float_phase) = 0;
}

void bldc_mos_stop(bldc_ctrl_t *ctrl) 
{
    ctrl->hTIM_pwm->CH1CVR = 0;
    ctrl->hTIM_pwm->CH2CVR = 0;
    ctrl->hTIM_pwm->CH3CVR = 0;
    ctrl->hTIM_pwm->CCER = CCER_SET[PHASE_NONE];
}

