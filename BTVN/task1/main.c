#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

void delay (int timer);
void chaseLed(int n);
void chaseLed2(int n);
void offLed(void);
void GPIO_Config (void);

void NVIC_Config(void);
void EXTI_Config(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);

uint8_t sangdan[5] = {GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6 ,GPIO_Pin_7 };
uint8_t  tatdan[6] = { 0x00, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6 };

int main(void){
		GPIO_Config();
		NVIC_Config();
		EXTI_Config();
	
		GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		
	while(1){
		GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		delay(1000000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		delay(1000000);
		
	}

	
	return 0;
}
	
	
void GPIO_Config (void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	
	//config
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//config led
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//config button
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void delay (int timer){
	int i;
		for( i = 0; i  < timer; i++);
}

void chaseLed(int n) {
    uint32_t i;
	for( i = 0; i < n; i++){
		GPIO_SetBits(GPIOA, GPIO_Pin_3);
		delay(200000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		delay(200000);
		
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		delay(200000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		delay(200000);
		
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		delay(200000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		delay(200000);
		
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
		delay(200000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		delay(200000);
		
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
		delay(200000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		delay(200000);
	}

}

void chaseLed2(int n){
	int i, j;
	for(i = 0 ; i <= n; i++){
		
	for(j= 0; j< 5; j++){
	GPIO_Write(GPIOA, sangdan[j]);
		delay(400000);
	}
	
	for(j= 5; j >=0; j--){
	GPIO_Write(GPIOA, tatdan[j]);
		delay(400000);
	}
	}
}
void offLed(void){

	GPIO_ResetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

}
	// Config NVIC
void NVIC_Config(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
	// config EXTI

void EXTI_Config(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode =  EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode =  EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

// xu ly ngat
void EXTI0_IRQHandler(void){
		if(EXTI_GetFlagStatus(EXTI_Line0) != RESET){
			
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) {
				chaseLed2(2);
			}
		EXTI_ClearITPendingBit(EXTI_Line0);
		}
}

void EXTI1_IRQHandler(void){
		if(EXTI_GetFlagStatus(EXTI_Line1) != RESET){
			
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0) {
				offLed();
			}
		EXTI_ClearITPendingBit(EXTI_Line1);
		}
}
