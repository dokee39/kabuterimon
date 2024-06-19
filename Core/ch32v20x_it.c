#include "ch32v20x_it.h"
#include "ch32v20x.h"
#include "timesilce_task.h"
// #include "motor.h"
// #include "control.h"
// #include "battery.h"
// #include "led.h"
// #include "pwm_input.h"

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
#warning "after init"
    timeslice_tick();
    timeslice_exec();

    SysTick->SR = 0;
}

void HardFault_Handler(void)
{
//     uint32_t temp_reg[3];
//     motor_stop(BLDC_BRAKE_DUTY);
//     printf("HardFault_Handler\r\n");
//     temp_reg[0] = __get_MCAUSE();
//     temp_reg[1] = __get_MTVAL();
//     temp_reg[2] = __get_MEPC();
//     printf("mcause:%0x\r\n",temp_reg[0]);
//     printf("mtval:%0x\r\n",temp_reg[1]);
//     printf("mepc:%0x\r\n",temp_reg[2]);
#warning "operations"
    while (1)
    {

    }
}

void TIM2_IRQHandler(void)
{
    
}

void TIM3_IRQHandler(void)
{
    
}

void TIM4_IRQHandler(void)
{
    
}

void EXTI2_IRQHandler(void)
{
    
}
