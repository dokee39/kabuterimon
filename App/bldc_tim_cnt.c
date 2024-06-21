#include "bldc_tim_cnt.h"

void bldc_tim_cnt_disable(bldc_ctrl_t *ctrl)
{
    ctrl->periph.hTIM_cnt->DMAINTENR &= ~TIM_IT_Update;
    ctrl->periph.hTIM_cnt->INTFR = 0;
    ctrl->periph.hTIM_cnt->CNT = 0;
}

void bldc_tim_cnt_enable(bldc_ctrl_t *ctrl, uint16_t cnt)
{
    ctrl->periph.hTIM_cnt->CNT = 0;
    ctrl->periph.hTIM_cnt->ATRLR = cnt;
    ctrl->periph.hTIM_cnt->INTFR = 0;;
    ctrl->periph.hTIM_cnt->DMAINTENR |= TIM_IT_Update;
}

