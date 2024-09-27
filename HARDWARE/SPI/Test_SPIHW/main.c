#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_rcc.h>

#define SPI1_SCK    GPIO_Pin_5
#define SPI1_MISO   GPIO_Pin_6
#define SPI1_MOSI   GPIO_Pin_7
#define SPI1_NSS    GPIO_Pin_4
#define SPI1_GPIO   GPIOA

void delay(uint8_t n)
{
	for(uint8_t i=0;i<n;i++)
	{
		
	}
}
void RCC_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
}

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MOSI | SPI1_NSS ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = SPI1_MISO;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}

void SPI_Config(void) {
    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}

void SPI_Trans(uint8_t data) {
	  GPIO_ResetBits(GPIOA,SPI1_NSS);
	  SPI_I2S_SendData(SPI1, data);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	  GPIO_SetBits(GPIOA,SPI1_NSS);
}
static uint8_t arr[4]= {1,3,5,2};
 
int main(void) {
   
   
    RCC_Config();
    GPIO_Config();
    SPI_Config();

    while (1) {
        for (uint8_t i = 0; i < 3; i++) {
            SPI_Trans(arr[i]);
           delay(100);
        }
    }
}
