#include<stm32f10x.h>
#include<stm32f10x_rcc.h>
#include<stm32f10x_usart.h>
#include<stm32f10x_gpio.h>

#define TX GPIO_Pin_9
#define RX GPIO_Pin_10	
void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1,ENABLE);
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
void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);

}
void UART_Config()
{
	  USART_InitTypeDef USART_InitStructure;
	
    USART_InitStructure.USART_BaudRate = 9600; // toc do truyen uart
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8 bit data
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1 bit stop
    USART_InitStructure.USART_Parity = USART_Parity_No; // khong parity
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // khong su dung luong dieu khien phan cung
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

}
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)// khi uart nhan dc data se vao ham nay
    { 
			   
        uint8_t data = USART_ReceiveData(USART1);

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
int main()
{
	RCC_Config();
	GPIO_Config();
	NVIC_Config();
	UART_Config();
	while(1)
	{
	}
	return 0;
}