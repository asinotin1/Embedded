#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

uint8_t data;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );
}
void GPIO_Config()
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AIN; // mode analog
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void ADC_Config()
{
	ADC_InitTypeDef ADC_InitStructure;
	/* ADC_Mode : cau hinh adc don kenh hay da kenh ngoai ra con co che do tuan tu cac kenh */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// don kenh
	ADC_InitStructure.ADC_NbrOfChannel = 1; // so kenh adc cau hinh
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // cau hinh de quet adc lan luot tung kenh
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ENABLE de su dung tin hieu trigger de kich hoat adc. DISABLE neu khong su dung 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // cau hinh adc co chuyen doi lien tuc hay khong
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// cau hinh can le cho data (co the chon ben trai)
	
	ADC_Init(ADC1,&ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//cau hinh lay mau, thu tu kenh adc khi quet
	/*sampletime : thoi gian lay mau :*/
	ADC_Cmd(ADC1,ENABLE);
	
	/*hieu chuan giup tang do chinh xac cua adc*/
	ADC_ResetCalibration(ADC1); // Reset hieu chuan
  while(ADC_GetResetCalibrationStatus(ADC1)); // doi reset hieu chuan
  ADC_StartCalibration(ADC1); // start hieu chuan
  while(ADC_GetCalibrationStatus(ADC1)); // doi hieu chuan ket thuc
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//bat dau qua trinh chuyen doi
}

uint16_t ADC_Read()
{
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // doi chuyen doi hoan tat

    // tra ve gia tri adc
    return ADC_GetConversionValue(ADC1);
}
int main()
{
	RCC_Config();
  GPIO_Config();
	ADC_Config();
	 while(1)
		{
			data = ADC_Read();
    }
    return 0;
}