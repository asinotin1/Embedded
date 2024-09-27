#include<stm32f10x.h>
#include<stm32f10x_gpio.h>
#include<stm32f10x_can.h>
#include<stm32f10x_rcc.h>
#define Can_TX GPIO_Pin_12
#define Can_RX GPIO_Pin_11

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

void requestTemperatureData(uint32_t id){
  CanTxMsg TxMessage;

    TxMessage.StdId = id;
    TxMessage.ExtId = 0x00;
    TxMessage.RTR = CAN_RTR_REMOTE;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 0;
    
    CAN_Transmit(CAN1, &TxMessage);
		/* Wait until the current message frame is successfully transmitted */
		while (CAN_TransmitStatus(CAN1, CAN_FIFO0) != CAN_TxStatus_Ok);
}

void read_Respond_Data(uint8_t *data){
	CanRxMsg RxMessage;
	
	 while (CAN_MessagePending(CAN1, CAN_FIFO0) == 0 );
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		
    for (int i = 0; i < RxMessage.DLC; i++)
    {
			 if(RxMessage.StdId == 0x321)
					{
						data[i] = RxMessage.Data[i];
					}
    }
}


int main()
{
	
	RCC_Config();
	GPIO_Config();
	Can_Config();
	
	uint16_t data[3]={0};
	requestTemperatureData(0x123);
	read_Respond_Data(data);
	while(1)
	{
		
	}
	return 0;
}