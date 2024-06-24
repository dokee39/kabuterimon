#include "bldc_zero_cross.h"

static const uint16_t OPA_SELECT[4] = { OPA_SELECT_U, OPA_SELECT_V, OPA_SELECT_W, OPA_SELECT_NONE };

void bldc_zero_cross_switch(bldc_ctrl_t *ctrl)
{
    #warning "edge delect"
    OPA_CHANNEL = OPA_SELECT[six_step[ctrl->step].float_phase];
    if (ctrl->step % 2) {
        EXTI->RTENR = 0;
        EXTI->FTENR = ctrl->GPIO_Pin_EXTI;
    } else {
        EXTI->FTENR = 0;
        EXTI->RTENR = ctrl->GPIO_Pin_EXTI;
    }
    EXTI->INTENR |= ctrl->GPIO_Pin_EXTI;
}

void bldc_zero_cross_disable(bldc_ctrl_t *ctrl)
{
    EXTI->INTENR &= ~ctrl->GPIO_Pin_EXTI;
    EXTI->INTFR = ctrl->GPIO_Pin_EXTI;
    EXTI->RTENR = 0;
    EXTI->FTENR = 0;
    OPA_CHANNEL = OPA_SELECT[PHASE_NONE];
}

