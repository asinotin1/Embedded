#include<stm32f10x.h>
#include<stm32f10x_rcc.h>
#include<stm32f10x_usart.h>
#include<stm32f10x_gpio.h>

#define TX GPIO_Pin_9
#define RX GPIO_Pin_10

uint8_t data[4]= {1,3,6,7};

void delay(uint8_t n)
{
	for(uint8_t i=0;i<n;i++)
	{
	}
}
void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
}
void GPIO_Config()
{
	 GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = TX; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RX; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART_Init() {
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = 9600; // toc do truyen uart
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8 bit data
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1 bit stop
    USART_InitStructure.USART_Parity = USART_Parity_No; // khong parity
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // khong su dung luong dieu khien phan cung
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
}
void UART_Transmit(uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);// kiem tra xem co data trong bo nho dem khong
        
        USART_SendData(USART1, data[i]);
    }
    
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // doi cho den khi truyen xong data
}
int main() {
	  RCC_Config();
	  GPIO_Config();
    UART_Init();

    while(1) {
			for(uint8_t i=0;i<4;i++)
			{
				UART_Transmit(data, 4);
				delay(1000);
			}
    }
}
