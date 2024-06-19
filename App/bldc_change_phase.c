#include <stdlib.h>
#include "bldc_change_phase.h"

static void bldc_change_phase(void *handle);

void bldc_change_phase_init(void *handle, bldc_control_param_t *control_param)
{
    bldc_change_phase_t *change_phase = (bldc_change_phase_t *)handle;
    change_phase->param = control_param;
    change_phase->operation = bldc_change_phase_operation;
}

void bldc_change_phase_operation(void *handle)
{
    bldc_change_phase_t *change_phase = (bldc_change_phase_t *)handle;

    switch (change_phase->param->status) {
        case ALIGNMENT:
            break;
        case DRAG:
            break;
        case CLOSED_LOOP:
            break;
        default:
            break;
    }
}

static void bldc_change_phase(void *handle)
{
    bldc_change_phase_t *change_phase = (bldc_change_phase_t *)handle;
}



