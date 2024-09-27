#include "stm32f10x.h"

void GPIO_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // B?t clock cho GPIOC
		
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void timer_Config(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // B?t clock cho TIM2
	
    // Th?i gian d?m xung clock
    // 72MHz / 72 = 1MHz => 1us x 1000 = 1000us = 1ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // C�i d?t d?m l�n
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; // L�m tr�n s? nguy�n, kh�ng d�ng b? chia
    TIM_TimeBaseStructure.TIM_Period = 999; // Chu k? timer
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM2, ENABLE);
}

void delay1MS() {
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < 1000) {}
}

void delayMS(uint32_t delay_ms) {
    while (delay_ms) {
        delay1MS();
        --delay_ms;
    }
}

int main(void) {
    GPIO_Config();
    timer_Config();
    while (1) {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        delayMS(200);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        delayMS(200);
    }
}
