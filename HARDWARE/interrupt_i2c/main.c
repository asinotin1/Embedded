#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
#include "misc.h"

#define SCL GPIO_Pin_6  
#define SDA GPIO_Pin_7 

void GPIO_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SCL| SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	
}


void I2C_Configuration(void)
{
	 I2C_InitTypeDef I2C_InitStructure;
	  
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;

    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);
    
	 I2C_ITConfig(I2C1,I2C_IT_RXNE,ENABLE);
}

void I2C1_EV_IRQHandle(void)
{
	if(I2C_GetITStatus(I2C1,I2C_IT_RXNE) != RESET) // khi nhan dc data thi se vao ngat
	{
		  uint8_t data = I2C_ReceiveData(I2C1);
		
		I2C_ClearITPendingBit(I2C1,I2C_IT_RXNE);
	}
}

int main()
{
	GPIO_Configuration();
	NVIC_Configuration();
	I2C_Configuration();
	while(1)
	{
		// code
	}
	return 0;
}
