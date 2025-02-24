//master truyen

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define SCL GPIO_Pin_6  
#define SDA GPIO_Pin_7  

#define SLAVE 0x44

uint8_t arr[4] = {1, 3, 5, 7};
void RCC_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}
void GPIO_Config()
{
	  GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = SCL | SDA; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_Config() {
    I2C_InitTypeDef I2C_InitStructure;
	
    I2C_InitStructure.I2C_ClockSpeed = 100000;  // Toc do  truyen I2C 100kHz
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);

    I2C_Cmd(I2C1, ENABLE);
}
void I2C_Star()
{
	 I2C_GenerateSTART(I2C1, ENABLE); //tao bit start
	
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));// check bit start hoat dong chua ?
}
void I2C_Stop()
{
	 I2C_GenerateSTOP(I2C1, ENABLE); //tao bit stop

}
void I2C_Transmit(uint8_t *data, uint8_t length) {
    I2C_Star();
	
    I2C_Send7bitAddress(I2C1, 0x44, I2C_Direction_Transmitter);// gui dia chi slave
	
	
	  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); // cho slave tra ack
	
    for (uint8_t i = 0; i < length; i++) 
	       {
        I2C_SendData(I2C1, data[i]);
						
        while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF)); // Cho du lieu truyen hoan toan
        }
		
		I2C_Stop();
  
}

int main() {
	  RCC_Config();
	  GPIO_Config();
    I2C_Config();

    while (1)
		{
			I2C_Transmit(arr, 4);
		}
}
