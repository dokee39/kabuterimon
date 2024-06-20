#ifndef _BLDC_ZERO_CROSS_H_
#define _BLDC_ZERO_CROSS_H_

#include "main.h"
#include "bldc_ctrl_param.h"

#define OPA_CHANNEL     (OPA->CR)
#define OPA_SELECT_U    (0x0050)
#define OPA_SELECT_V    (0x0010)
#define OPA_SELECT_W    (0x0001)
#define OPA_SELECT_NONE (0x0000)

void bldc_zero_cross(bldc_ctrl_param_t *param);
void bldc_zero_cross_enable(void);
void bldc_zero_cross_disable(void);

#endif

