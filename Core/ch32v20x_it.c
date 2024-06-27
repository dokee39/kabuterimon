#include "ch32v20x_it.h"
#include "ch32v20x.h"
#include "timeslice_task.h"
#include "bldc_callback.h"

extern bldc_ctrl_t ctrl;

void NMI_Handler(void)       __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM3_IRQHandler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM4_IRQHandler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI2_IRQHandler(void)  __attribute__((interrupt("WCH-Interrupt-fast")));

void NMI_Handler(void)
{
    
}

void SysTick_Handler(void)
{
    timeslice_tick();
    timeslice_exec();

    SysTick->SR = 0;
}

void HardFault_Handler(void)
{
    __disable_irq();
    TIM1->CCER = 0;
    while (1)
    {
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_FLAG_CC1) != RESET)
    {
        bldc_pwm_input_callback(&ctrl);
        TIM2->INTFR = ~TIM_FLAG_CC1;
    }
    if (TIM_GetITStatus(TIM2, TIM_FLAG_CC4) != RESET)
    {
        bldc_pwm_input_timeout_callback(&ctrl);
        TIM2->INTFR = ~TIM_FLAG_CC4;
    }
}

void TIM3_IRQHandler(void)
{
    if (OPA_CHANNEL == OPA_SELECT_NONE) {
        bldc_change_phase_callback(&ctrl);
    } else {
        bldc_time_out_callback(&ctrl);
    }
    TIM3->INTFR = 0;
}

void TIM4_IRQHandler(void)
{
    
    TIM4->INTFR = 0;
}

void EXTI2_IRQHandler(void)
{
    bldc_zero_cross_callback(&ctrl);
    EXTI->INTFR = ctrl.GPIO_Pin_EXTI;
}
