#include<stm32f10x.h>
#include<stm32f10x_gpio.h>
#include<stm32f10x_can.h>
#include<stm32f10x_rcc.h>

#define Can_TX GPIO_Pin_12
#define Can_RX GPIO_Pin_11

void RCC_Config()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
}

void GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    
    GPIO_InitStructure.GPIO_Pin = Can_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
    
    GPIO_InitStructure.GPIO_Pin = Can_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Can_Config()
{
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

    // toc do truyen  CAN = 1 Mbps      
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;      
    CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;     
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;   
    CAN_InitStructure.CAN_Prescaler = 4;
    CAN_Init(CAN1, &CAN_InitStructure);	
	  
    // cau hinh bo loc 0x123
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (0x123 << 5);  // D?ch sang tr�i 5 bit n� dua c�c bit id l�n d?u ti�n c�n 5 bit tr?ng l� 1RTR, 1IDE, 3r0
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF; // Mat na cho ph�p nhan d�ng ID 0x123
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

void receive_Can_message(uint8_t *data)
{
    CanRxMsg RxMessage;

    // doi den khi co du lieu
    while (CAN_MessagePending(CAN1, CAN_FIFO0) == 0);

    // nhan data tu FIFO0
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

   
    if (RxMessage.StdId == 0x123)
    {
        for (int i = 0; i < RxMessage.DLC; i++)
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
	 
    uint8_t testdata[8] = {0};

    while(1)
    {
        receive_Can_message(testdata);

    }

    return 0;
}
