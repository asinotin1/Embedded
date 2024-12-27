
#include<stm32f10x.h>
#include<stm32f10x_gpio.h>
#include<stm32f10x_rcc.h>
#include<misc.h>
#include <stm32f10x_exti.h>
// cau hinh gpio
void RCC_config(){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

}
void GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_Initstructure;
// cau hinh pA0	la chan ngat ngoai 
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
	/*Ch�n GPIO duoc cau h�nh l�m dau v�o v� o trang th�i noi (kh�ng pull-up hay pull-down). 
	�ieu n�y c� nghia l� ch�n kh�ng duoc ket noi co dinh voi mc cao (VDD) hoac muc thap (GND) th�ng qua dien tro*/
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	
	GPIO_Initstructure.GPIO_Pin =GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	
}
//cau hinh NVIC(ngat)
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_Initstructure;
	/*NVIC_PriorityGroup_1 : ngat nay la chong chen ngang no chay xong ngat 1 roi no moi xet toi ngat nao tiep theo
	- co 1 loai ngat nua la ngat chay theo tuan tu la xet do uu tien giua cac ngat de chay*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // la ham duoc su dung de cau hinh nhom uu tien cua cac ngat trong vi dieu khien 
	NVIC_Initstructure.NVIC_IRQChannel = EXTI0_IRQn; // la kenh ngat can duoc cau hinh  EXTI0_IRQn: chon  chon GPIO0 lam ngat ngoai
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 0; // xet do uu tien (muc 0 la cao nhat) uu tien chinh
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; //   uu tien phu:  neu co nhieu ngat cung = 0 thi se xet xem ngat phu nao nho nhat
  NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_Initstructure);	
}

// cau hinh EXTI(neu day la ngat uart thi cho nay la uart hoac timer cung vay)
void EXTI_Configuration()
{
	EXTI_InitTypeDef EXTI_Initstrucute;
	
	// ket noi chan POA voi line ngat
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_Initstrucute.EXTI_Line = EXTI_Line0;  // xac dinh xem line ngat nao duoc dung de kich hoat ngat. moi line chi co 1 ngat( co 19 line)
	EXTI_Initstrucute.EXTI_Mode = EXTI_Mode_Interrupt;   // khi co su kien ben ngoai xay ra thi se chuyen sang ngat     
	EXTI_Initstrucute.EXTI_Trigger = EXTI_Trigger_Rising; // che do ngat(co 3 che do ) EXTI_Trigger_Rising: chuyen tu muc thap sang muc cao
	EXTI_Initstrucute.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_Initstrucute);
}
// ham thuc hien ngat
void EXTI0_IRQHandler()
{
	if (EXTI_GetFlagStatus(EXTI_Line0) == SET)//kiem tra ham ngat co duoc kich hoat hay khong
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		EXTI_ClearITPendingBit(EXTI_Line0); // XOA CO NGAT
	}
}
int main()
{
	RCC_config();
	GPIO_Configuration();
	NVIC_Configuration();
	EXTI_Configuration();
	while(1)
	{
	}
	return 0;
}

