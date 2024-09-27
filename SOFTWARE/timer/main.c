#include<stm32f10x.h>
#include<stm32f10x_rcc.h>
#include<stm32f10x_tim.h>


void timer_Config(void);

void delay_ms(uint16_t time);


int LED = 0;

int main(void)
{
	timer_Config();
	while(1)
	{
		LED = 0;
		delay_ms(100);
		LED = 1;
		delay_ms(100);
		
   }
 }

void timer_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);// CAP XUNG CHO TIME 2
	
	//thoi gian dem xung clock
	//72MHZ / 720 = 100khz       1ms = 100 xung
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // set up dem len
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; // l� k dung bo chia
	TIM_TimeBaseStructure.TIM_Period = 999; // chu ky timer
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2,ENABLE);
}

// couter : la cu 1 ms se dem len 1
void delay_ms(uint16_t time)
	{
		TIM_SetCounter(TIM2,0); // reset de timer2 dem lai tu vi tri 0( d�ng  timer l� chu yeu)

		while(TIM_GetCounter(TIM2)<time);	 // getcouter h�m n�y d�ng d� doc gia tri hien tai cua thang TIM ban dang tro toi
    
  }

