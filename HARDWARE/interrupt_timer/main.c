#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


void RCC_Config() 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
void NVIC_Config()
{
	  NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}
void TIM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_Prescaler=3599;
	TIM_InitStruct.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period=9999;
	TIM_InitStruct.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
  TIM_Cmd(TIM2, ENABLE);  
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
}
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	}
}	
int main()
{
	RCC_Config();
	GPIO_Config();
	NVIC_Config();
	TIM_Config();
	
	while(1)
	{
		 GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	}
	return 0;
}
