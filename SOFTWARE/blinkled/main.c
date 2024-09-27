#include<stm32f10x.h>
#define HIGH 1
#define LOW 0

void delay(int time)
{
	for(int i=0;i<time;i++);
}
void digitalWrite(uint16_t pin,uint8_t state)
{
	if(state == HIGH)
	{
		GPIOC->BSRR =(1<<pin);// obxxxxxxxxxxxxxxxxxxx1
  }
	else{
		
	GPIOC->BSRR=(1<<pin+16);//0bxxxxxxxxx1xxxxxxxxx
  }
}

int main(void){

	// bat xung cho GPIO
	// RCC là giup reset và phát tín hieu cho các chân clock dam bao hoat dong chinh xac cua stm32
	RCC ->APB2ENR |= RCC_APB2ENR_IOPCEN; // kich hoat bat len bang 1
	
	//reset chan 13
	GPIOC ->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);// CRH cau hinh chan 8->15
	
	//OUTPUT
	GPIOC->CRH |= GPIO_CRH_MODE13_0; // xuat tin hieu cho vi dieu khien  muc 0 *luu y : muc 1 la input
	
	while(1)
	{
		digitalWrite(13,HIGH);
		delay(1000000);
		digitalWrite(13,LOW);
		delay(1000000);
	}
}