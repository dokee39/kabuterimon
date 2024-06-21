#ifndef _BLDC_ZERO_CROSS_H_
#define _BLDC_ZERO_CROSS_H_

#include "main.h"
#include "bldc_ctrl.h"

void bldc_zero_cross_switch(bldc_ctrl_t *ctrl);
void bldc_zero_cross_disable(bldc_ctrl_t *ctrl);

#endif

