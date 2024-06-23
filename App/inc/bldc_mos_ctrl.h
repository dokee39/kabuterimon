#ifndef _BLDC_MOS_CTRL_
#define _BLDC_MOS_CTRL_

#include "bldc_ctrl.h"

void bldc_mos_change_phase(bldc_ctrl_t *ctrl);
void bldc_mos_update_duty(bldc_ctrl_t * ctrl);
void bldc_mos_stop(bldc_ctrl_t *ctrl);

#endif

