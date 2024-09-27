#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void RCC_Config(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	
}

void Delay1Ms(void) {
    TIM_SetCounter(TIM4,0);
    while (TIM_GetCounter(TIM4) < 1000) {
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

void servo_PWM_Config() {
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1000; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
}


void TIM4_Delay_Init() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period =  0xFFFF;  
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM4,ENABLE);
}
void GPIO_Config() {
	// chan pwm servo
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	// chan trig
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	   GPIO_Init(GPIOB,&GPIO_InitStructure);
	// chan echo
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	   GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}
void HCSR04_config()
{
	
		TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
		TIM_TimBaseInitStructure.TIM_Period = 0xFFFF;
		TIM_TimBaseInitStructure.TIM_Prescaler = 72-1;
		TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM3,&TIM_TimBaseInitStructure);
	
	//pwm trig
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 15; //us
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3,&TIM_OCInitStruct);
	
	//capture echo
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);
	
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
}
uint16_t HCSR04_GetDistance() 
{
	//thiet lap bo dem thoi gian dua tim3 ve 0
    TIM3->CNT = 0;
    
    // kich hoat tim3 
    TIM_Cmd(TIM3, ENABLE);
	
  // doi cho co nay duoc set len
    while(!TIM_GetFlagStatus(TIM3, TIM_FLAG_Update));
	
// lay gia tri cua 2 kenh
    uint16_t capture1 = TIM_GetCapture1(TIM3);
    uint16_t capture2 = TIM_GetCapture2(TIM3);
    
    // tat bo dem tim3
    TIM_Cmd(TIM3, DISABLE);
    
    // xoa co
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    
    //tinh khoang cach dua tren thoi gian giua 2 su kien 
    // Kho?ng cách tính d?a trên công thuc distance = pulseWidth * 0.177 (do 1 tick = 1µs = 0.1724mm)
    uint16_t pulseWidth = capture2 - capture1;
    uint16_t distance = pulseWidth * 0.177;
    
    return distance;
}

int main(void) {
    RCC_Config();
    GPIO_Config();
    servo_PWM_Config();
    TIM_Config();
    TIM4_Delay_Init();
    HCSR04_config();
	uint16_t space = 0;
    while(1) 
	{
		space = HCSR04_GetDistance();
		if(space <=20)
		{
    TIM_SetCompare1(TIM2, 1000); 
    Delay_Ms(1000); 
    
    TIM_SetCompare1(TIM2, 1500); 
    Delay_Ms(1000); 
	  }
	}
     
    return 0;
}
