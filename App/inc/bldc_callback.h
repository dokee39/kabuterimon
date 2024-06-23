#ifndef _BLDC_CALLBACK_H_
#define _BLDC_CALLBACK_H_

#include "bldc_ctrl.h"

void bldc_change_phase_callback(bldc_ctrl_t *ctrl);
void bldc_zero_cross_callback(bldc_ctrl_t *ctrl);
void bldc_time_out_callback(bldc_ctrl_t *ctrl);

#endif

