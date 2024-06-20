#include <stdlib.h>
#include "bldc_change_phase.h"

typedef struct {
    uint16_t upper_phase_duty_mask[3];
    uint16_t lower_phase_output_mask[3];
    bldc_phase_e adc_bemf_phase;
} bldc_six_step_t;

bldc_six_step_t six_step[6] = {
    { {0xFF, 0x00, 0x00}, {0x00, 0x00, 0xFF}, PHASE_V },
    { {0xFF, 0x00, 0x00}, {0x00, 0xFF, 0x00}, PHASE_W },
    { {0x00, 0x00, 0xFF}, {0x00, 0xFF, 0x00}, PHASE_U },
    { {0x00, 0x00, 0xFF}, {0xFF, 0x00, 0x00}, PHASE_V },
    { {0x00, 0xFF, 0x00}, {0xFF, 0x00, 0x00}, PHASE_W },
    { {0x00, 0xFF, 0x00}, {0x00, 0x00, 0xFF}, PHASE_U },
};

static void bldc_change_phase(bldc_control_param_t *param);

void bldc_change_phase_init(void *handle, bldc_control_param_t *control_param)
{
    bldc_change_phase_t *change_phase = (bldc_change_phase_t *)handle;
    change_phase->param = control_param;
    change_phase->operation = bldc_change_phase_operation;
}

void bldc_change_phase_operation(void *handle)
{
    bldc_change_phase_t *change_phase = (bldc_change_phase_t *)handle;

#warning "stall protection"
    switch (change_phase->param->status) {
        case ALIGNMENT:
            change_phase->param->duty = ALIGNMENT_DUTY;
            bldc_change_phase(change_phase->param);
            // change_phase->param->duty = 0;
            break;
        case DRAG:
            break;
        case CLOSED_LOOP:
            break;
        default:
            break;
    }
}

static void bldc_change_phase(bldc_control_param_t *param)
{
    param->step += param->dir == CW ? 1 : -1;
    if (param->step > 5) {
        param->step = 0;
    } else if (param->step < 0) {
        param->step = 5;
    }

    bldc_close_upper(param);
    bldc_close_lower(param);
    bldc_update_upper_duty(param);
    bldc_update_lower(param);
}

void bldc_close_upper(bldc_control_param_t *param)
{
    param->hTIM->CH1CVR = 0x00;
    param->hTIM->CH2CVR = 0x00;
    param->hTIM->CH3CVR = 0x00;
}

void bldc_update_upper_duty(bldc_control_param_t *param)
{
    param->hTIM->CH1CVR = param->duty & six_step[param->step].upper_phase_duty_mask[0];
    param->hTIM->CH2CVR = param->duty & six_step[param->step].upper_phase_duty_mask[1];
    param->hTIM->CH3CVR = param->duty & six_step[param->step].upper_phase_duty_mask[2];
}

void bldc_close_lower(bldc_control_param_t *param)
{
    param->hGPIO_Port_UL->REG_OFF = param->GPIO_Pin_UL;
    param->hGPIO_Port_VL->REG_OFF = param->GPIO_Pin_VL;
    param->hGPIO_Port_WL->REG_OFF = param->GPIO_Pin_WL;
}

void bldc_update_lower(bldc_control_param_t *param)
{
    param->hGPIO_Port_UL->REG_OFF = param->GPIO_Pin_UL & (!six_step[param->step].lower_phase_output_mask[0]);
    param->hGPIO_Port_VL->REG_OFF = param->GPIO_Pin_VL & (!six_step[param->step].lower_phase_output_mask[1]);
    param->hGPIO_Port_WL->REG_OFF = param->GPIO_Pin_WL & (!six_step[param->step].lower_phase_output_mask[2]);
    param->hGPIO_Port_UL->REG_ON = param->GPIO_Pin_UL & six_step[param->step].lower_phase_output_mask[0];
    param->hGPIO_Port_VL->REG_ON = param->GPIO_Pin_VL & six_step[param->step].lower_phase_output_mask[1];
    param->hGPIO_Port_WL->REG_ON = param->GPIO_Pin_WL & six_step[param->step].lower_phase_output_mask[2];
}



