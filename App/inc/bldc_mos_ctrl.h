#ifndef _BLDC_MOS_CTRL_
#define _BLDC_MOS_CTRL_

#include "bldc_ctrl.h"

void bldc_change_phase(bldc_ctrl_t *ctrl);
void bldc_update_upper_duty(bldc_ctrl_t *ctrl);
void bldc_close_upper_lower(bldc_ctrl_t * ctrl);

#endif

