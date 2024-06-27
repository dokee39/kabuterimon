#include  <stdint.h>
#include "main.h"
#include "ch32v20x.h"
#include "timeslice_task.h"
#include "led_task.h"
#include "bldc_ctrl.h"

TimesliceTaskObj obj_led_task;
TimesliceTaskObj obj_bldc_ctrl_task;

static void peripheral_init(void);

int main(void)
{
    __disable_irq();

    peripheral_init();

    led_init();
    // timeslice_task_init(&obj_led_task, led_task, 1, US_TO_TICK(LED_TASK_INTERVAL));
    // timeslice_task_add(&obj_led_task);

    bldc_ctrl_init();
    timeslice_task_init(&obj_bldc_ctrl_task, bldc_ctrl_task, 1, US_TO_TICK(BLDC_CTRL_TASK_INTERVAL));
    timeslice_task_add(&obj_bldc_ctrl_task);

    __enable_irq();

    while(1)
    {
        
    }
}

static void peripheral_init(void)
{
    GPIO_InitTypeDef        GPIO_InitStruct           = {0};
    EXTI_InitTypeDef        EXTI_InitStructure        = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_BDTRInitTypeDef     TIM_BDTRInitStructure     = {0};
    TIM_ICInitTypeDef       TIM_ICInitStructure       = {0};
    TIM_OCInitTypeDef       TIM_OCInitStructure       = {0};

    // set vextor table free
    extern void TIM3_IRQHandler(void);
    extern void EXTI2_IRQHandler(void);
    extern void TIM1_UP_IRQHandler(void);
    extern void SysTick_Handler(void);
    SetVTFIRQ((uint32_t)EXTI2_IRQHandler,   EXTI2_IRQn,   0, ENABLE);
    SetVTFIRQ((uint32_t)TIM3_IRQHandler,    TIM3_IRQn,    1, ENABLE);
    SetVTFIRQ((uint32_t)TIM1_UP_IRQHandler, TIM1_UP_IRQn, 2, ENABLE);
    SetVTFIRQ((uint32_t)SysTick_Handler,    SysTicK_IRQn, 3, ENABLE);
    
    // pit init
    SysTick->CTLR = 0;
    SysTick->SR   = 0;
    SysTick->CNT  = 0;
    SysTick->CMP  = SystemCoreClock / (1000000 / SYSTICK_PERIOD_US) - 1;
    SysTick->CTLR = 0xF;
    NVIC_SetPriority(SysTicK_IRQn, 0xE0);

    // led pin init
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin   = LED_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PIN_PORT, &GPIO_InitStruct);
    LED_PIN_PORT->BSHR = LED_PIN; // led on

    // opama init
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    OPA->CR = 0x0000; // not select opa
    
    // opama output exit init
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line2);
    NVIC_SetPriority(EXTI2_IRQn, 0x00);
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_EnableIRQ(SysTicK_IRQn);

    // three-phase pin init
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin   = PHASE_A_GPIO_LOW;
    GPIO_Init(PHASE_A_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = PHASE_B_GPIO_LOW;
    GPIO_Init(PHASE_B_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = PHASE_C_GPIO_LOW;
    GPIO_Init(PHASE_C_GPIO_PORT_LOW, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = PHASE_A_GPIO_HIGH;
    GPIO_Init(PHASE_A_GPIO_PORT_HIGH, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = PHASE_B_GPIO_HIGH;
    GPIO_Init(PHASE_B_GPIO_PORT_HIGH, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = PHASE_C_GPIO_HIGH;
    GPIO_Init(PHASE_C_GPIO_PORT_HIGH, &GPIO_InitStruct);

    // timer 1 for three-phase output
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_DeInit(TIM1);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period            = BLDC_DUTY_MAX + BLDC_DUTY_CHARGE;
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 0;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC1FastConfig(TIM1, TIM_OCFast_Disable);
    TIM_OC2FastConfig(TIM1, TIM_OCFast_Disable);
    TIM_OC3FastConfig(TIM1, TIM_OCFast_Disable);
    TIM_BDTRStructInit(&TIM_BDTRInitStructure);
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 1;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_Break, DISABLE);
    TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
    TIM_ITConfig(TIM1, TIM_IT_Trigger, DISABLE);
    TIM_ITConfig(TIM1, TIM_IT_COM, DISABLE);
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
    TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
    TIM_Cmd(TIM1, ENABLE);
    TIM1->CH1CVR = 0;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = 0;
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_GenerateEvent(TIM1, TIM_EventSource_Update);

    // pwm input pin init
    GPIO_InitStruct.GPIO_Pin   = INPUT_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(INPUT_GPIO_PORT, &GPIO_InitStruct);

    // timer 2 for pwm input capture
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period            = 0xffff;
    TIM_TimeBaseInitStructure.TIM_Prescaler         = SystemCoreClock / 1000000 - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICFilter    = 15;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInitStructure.TIM_ICFilter    = 15;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_Pulse        = TIM_TimeBaseInitStructure.TIM_Period - 1;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    // TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    NVIC_SetPriority(TIM2_IRQn, 0x00);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_Cmd(TIM2, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE );
    TIM_TimeBaseInitStructure.TIM_Period            = 0xFFFF;
    TIM_TimeBaseInitStructure.TIM_Prescaler         = SystemCoreClock / (1000000 * 10 / TIM_CNT_PERIOD_1_10_US) - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ITConfig(TIM3, TIM_IT_Break, DISABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    TIM_ITConfig(TIM3, TIM_IT_Trigger, DISABLE);
    TIM_ITConfig(TIM3, TIM_IT_COM, DISABLE);
    NVIC_SetPriority(TIM3_IRQn, 0x00);
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM_Cmd(TIM3, ENABLE);

    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE );
    // TIM_TimeBaseInitStructure.TIM_Period            = 0xFFFF;
    // TIM_TimeBaseInitStructure.TIM_Prescaler         = (SystemCoreClock / 2 / 1000000) - 1;
    // TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    // TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    // TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
    // TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    // TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    // NVIC_SetPriority(TIM4_IRQn, 0x02);
    // NVIC_EnableIRQ(TIM4_IRQn);
    // TIM_Cmd(TIM4, ENABLE);

}

