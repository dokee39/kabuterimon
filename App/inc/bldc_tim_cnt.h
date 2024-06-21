#ifndef _BLDC_TIM_CNT_H_
#define _BLDC_TIM_CNT_H_

#include "bldc_ctrl.h"

void bldc_tim_cnt_disable(bldc_ctrl_t *ctrl);
void bldc_tim_cnt_enable(bldc_ctrl_t *ctrl, uint16_t cnt);

#endif

