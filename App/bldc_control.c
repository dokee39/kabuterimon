#include "bldc_control.h"
#include "bldc_control_param.h"
#include "bldc_change_phase.h"

typedef struct {
    bldc_control_status_e status;
    bldc_control_param_t param;
    bldc_change_phase_t change_phase;
} bldc_control_t;

bldc_control_t control;

void bldc_control_init(void)
{
    bldc_change_phase_init(&control.change_phase, &control.param);
}

void bldc_control_task(void)
{
    
}

