#include<stm32f10x.h>
#include<stm32f10x_gpio.h>
#include<stm32f10x_can.h>
#include<stm32f10x_rcc.h>
#define Can_TX GPIO_Pin_12
#define Can_RX GPIO_Pin_11

void delay(uint16_t time)
{
	for(uint8_t i=0;i<time;++i)
	{
	}
}

void RCC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
}
void GPIO_Config()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Can_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
	GPIO_InitStructure.GPIO_Pin = Can_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Can_Config()
{
	 CAN_InitTypeDef         CAN_InitStructure;
   CAN_FilterInitTypeDef   CAN_FilterInitStructure;
	
	  CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	   
	// toc do truyen can =  1 Mbps      
	  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;      
    CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;     
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;   
    CAN_InitStructure.CAN_Prescaler = 4;
    CAN_Init(CAN1, &CAN_InitStructure);	
	
	  CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;//0x123xxxxx
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF; 
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

void sendSpeedData(uint16_t SpeedData){
  CanTxMsg TxMessage;

    TxMessage.StdId = 0x123;
    TxMessage.ExtId = 0x00;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;// ID theo tiêu chu?n
    TxMessage.DLC = 2;
		TxMessage.Data[0] =(uint8_t)(SpeedData & 0xFF);
		TxMessage.Data[1] =(uint8_t)((SpeedData >> 8) & 0xFF);
    
    if(CAN_Transmit(CAN1, &TxMessage) != CAN_TxStatus_Ok){
				//logError("Error while transmit speed data") ARM có thanh ghi  log này 
		}
		
		for(uint8_t i = 0; i < 3; i++){
			if(CAN_Transmit(CAN1, &TxMessage) != CAN_TxStatus_Ok){
				break;
			}
			delay(100);	
		}
}
int main()
{
	RCC_Config();
	GPIO_Config();
	Can_Config();
	
	uint16_t speedData = 500;

	while(1)
	{
		sendSpeedData(speedData);
		delay(100);
	}
	return 0;
}