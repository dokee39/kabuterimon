#include "bldc_ctrl.h"
#include "bldc_mos_ctrl.h"

typedef struct {
    bldc_ctrl_param_t param;
    void (*change_phase)(bldc_ctrl_param_t *);
    void (*stop)(bldc_ctrl_param_t *);
} bldc_ctrl_t;

bldc_ctrl_t ctrl;

static void bldc_ctrl_param_clear(bldc_ctrl_param_t *param);

void bldc_ctrl_init(void)
{
    ctrl.param.hTIM     = TIM1;
    ctrl.param.GPIO_Pin_UL   = PHASE_A_GPIO_LOW;
    ctrl.param.GPIO_Pin_VL   = PHASE_B_GPIO_LOW;
    ctrl.param.GPIO_Pin_WL   = PHASE_C_GPIO_LOW;
    ctrl.param.hGPIO_Port_UL = PHASE_A_GPIO_PORT_LOW;
    ctrl.param.hGPIO_Port_VL = PHASE_B_GPIO_PORT_LOW;
    ctrl.param.hGPIO_Port_WL = PHASE_C_GPIO_PORT_LOW;

    ctrl.param.dir            = BLDC_DIR;
    ctrl.param.step           = 0;
    ctrl.param.duty_max       = BLDC_DUTY_MAX;
    ctrl.param.alignment_duty = BLDC_DUTY_MAX * ALIGNMENT_DUTY_RATIO;
    ctrl.param.alignment_time = ALIGNMENT_TIME;
    
    ctrl.param.clear  = bldc_ctrl_param_clear;
    ctrl.change_phase = bldc_change_phase;
    ctrl.stop = bldc_close_upper_lower;
    
    ctrl.param.clear(&ctrl.param);

}

void bldc_ctrl_task(void)
{
    switch (ctrl.param.status) {
        case IDLE:
            ctrl.param.clear(&ctrl.param);
            ctrl.stop(&ctrl.param);
            break;
        case ALIGNMENT:
            if (ctrl.param.step_time[ctrl.param.step] == 0) {
                ctrl.change_phase(&ctrl.param);
            }
            if (ctrl.param.step_time[ctrl.param.step] > ctrl.param.alignment_time) {
                ctrl.param.duty = 0;
                ctrl.param.status = DRAG;
            }
            ctrl.param.step_time[ctrl.param.step] += BLDC_CTRL_TASK_INTERVAL;
            break;
        case DRAG:
            break;
        case CLOSED_LOOP:
            break;
        default:
            break;
    }
    
}

static void bldc_ctrl_param_clear(bldc_ctrl_param_t *param)
{
    param->status        = IDLE;
    param->duty          = 0;
    param->speed         = 0;
    param->step_time_sum = 0;

    for (int i = 0; i < 5; i++) {
        param->step_time[i] = 0;
    }
}


