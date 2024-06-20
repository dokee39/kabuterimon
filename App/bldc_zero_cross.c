#include "bldc_zero_cross.h"

static const uint16_t OPA_SELECT[4] = { OPA_SELECT_U, OPA_SELECT_V, OPA_SELECT_W, OPA_SELECT_NONE };

void bldc_zero_cross(bldc_ctrl_param_t *param)
{
    
}

void bldc_zero_cross_enable(void)
{
    //
    EXTI->INTENR |= (1 << 2);
}

void bldc_zero_cross_disable(void)
{
    EXTI->INTENR &= ~(1 << 2);
    EXTI->INTFR = EXTI_Line2;
    OPA_CHANNEL = OPA_SELECT[PHASE_NONE];
}

