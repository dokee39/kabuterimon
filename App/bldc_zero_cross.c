#include "bldc_zero_cross.h"

static const uint16_t OPA_SELECT[4] = { OPA_SELECT_U, OPA_SELECT_V, OPA_SELECT_W, OPA_SELECT_NONE };

void bldc_zero_cross_switch(bldc_ctrl_t *ctrl)
{
    OPA_CHANNEL = OPA_SELECT[six_step[ctrl->step].float_phase];
    EXTI->RTENR = ctrl->periph.GPIO_Pin_EXTI;
    EXTI->FTENR = ctrl->periph.GPIO_Pin_EXTI;
    EXTI->INTENR |= ctrl->periph.GPIO_Pin_EXTI;
}

void bldc_zero_cross_disable(bldc_ctrl_t *ctrl)
{
    EXTI->INTENR &= ~ctrl->periph.GPIO_Pin_EXTI;
    EXTI->INTFR = ctrl->periph.GPIO_Pin_EXTI;
    EXTI->RTENR = 0;
    EXTI->FTENR = 0;
    OPA_CHANNEL = OPA_SELECT[PHASE_NONE];
}

