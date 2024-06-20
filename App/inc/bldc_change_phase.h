#ifndef _BLDC_CHANGE_PHASE_H_
#define _BLDC_CHANGE_PHASE_H_

#include "bldc_control_param.h"

typedef struct bldc_change_phase {
    bldc_control_param_t *param;

    void (*operation)(void *);
} bldc_change_phase_t;

void bldc_change_phase_init(void *handle, bldc_control_param_t *control_param);
void bldc_change_phase_operation(void *handle);
void bldc_close_upper(bldc_control_param_t *param);
void bldc_update_upper_duty(bldc_control_param_t *param);
void bldc_close_lower(bldc_control_param_t *param);
void bldc_update_lower(bldc_control_param_t *param);

#endif

