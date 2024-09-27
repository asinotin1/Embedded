#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void RCC_Config(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}

void Delay1Ms(void) {
    TIM_SetCounter(TIM3, 0);
    while (TIM_GetCounter(TIM3) < 1000) {
    }
}

void Delay_Ms(uint32_t u32DelayInMs) {
    while (u32DelayInMs) {
        Delay1Ms();
        --u32DelayInMs;
    }
}

void TIM_Config() {
    TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
    TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimBaseInitStructure.TIM_Prescaler = 72-1;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimBaseInitStructure);
    
    TIM_Cmd(TIM2, ENABLE);
}

void TIM3_Delay_Init() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period =  0xFFFF;  
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM3, ENABLE);
}

void GPIO_Config() {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void PWM_Config() {
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1000; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
}

int main() {
    RCC_Config();
    GPIO_Config();
    PWM_Config();
    TIM_Config();
    TIM3_Delay_Init();
    
    TIM_SetCompare1(TIM2, 1000); 
    Delay_Ms(1000); 
    
    TIM_SetCompare1(TIM2, 1500); 
    Delay_Ms(1000); 
    
    TIM_SetCompare1(TIM2, 1184); 
    Delay_Ms(1000); 
    
    TIM_SetCompare1(TIM2, 2000);
    Delay_Ms(1000); 
    
    while(1) {
    }
    return 0;
}
