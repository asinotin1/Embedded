// ngat spi truyen data
#include  "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "misc.h"


#define SS_PIN                  GPIO_Pin_4
#define SCK_PIN									GPIO_Pin_5
#define MISO_PIN								GPIO_Pin_6
#define MOSI_PIN								GPIO_Pin_7


volatile uint8_t data=11;

void GPIO_Config(){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SCK_PIN | MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void SPI_Config(){
	
	SPI_InitTypeDef SPI_InitStructure;
	
	// SPI_MASTER config
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
	
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);// kich hoat ngat truyen
}


void NVIC_Config(){
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);

}
// xu ly khi truyen xong (gui du lieu tiep theo)
void SPI1_IRQHandler(){
	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) != RESET)
	{
		
		SPI_I2S_SendData(SPI1,data+1);
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
	}
}


int main(){
	GPIO_Config();
	SPI_Config();
	NVIC_Config();
	
	SPI_I2S_SendData(SPI1,data);
	while(1){
	
	}
	return 0;
}
