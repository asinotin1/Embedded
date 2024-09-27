#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stdbool.h"

#define TX_PIN 		GPIO_Pin_9
#define RX_PIN 		GPIO_Pin_10

#define UART_PORT GPIOA
#define BAUD_RATE 9600

void delay_us(float us){ 		//72,000,000, 100hz/100ms
	while(us--);
}

void UART_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//cau hinh chan TX la output
	GPIO_InitStructure.GPIO_Pin = TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_PORT, &GPIO_InitStructure);
	
	// Cau hinh chan RX la input
	GPIO_InitStructure.GPIO_Pin = RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART_PORT, &GPIO_InitStructure);
}

void UART_SendChar(char c){
	
	while(GPIO_ReadInputDataBit(UART_PORT, TX_PIN) != Bit_RESET); // check kiem tra xem co phai muc 1 khong neu khong phai thi se khong truyen
	
		// Bat dau truyen bit start
		GPIO_ResetBits(UART_PORT, TX_PIN);
		
		delay_us(0.104167); 								// 9600bits/1000ms ->1bit = 0.104167 ms
		
		// Gui 8 bit du lieu
		for(int i =0; i< 8; i++){
			if(c & 1){												// 1 = 0b00000001
				GPIO_SetBits(UART_PORT, TX_PIN);
			}
			else{
				GPIO_ResetBits(UART_PORT, TX_PIN);
			}
			delay_us(0.104167);
			
			c >>= 1;
		}
		
		GPIO_SetBits(UART_PORT, TX_PIN);
		delay_us(0.104167);

}


char UART_ReceiveChar(void){
	char c;
	while(GPIO_ReadInputDataBit(UART_PORT, RX_PIN) != Bit_SET);
		
	delay_us(0.104167 / 2);						// Cho nua chu ki
		
	for(int i = 0; i < 8; i++){
		
		delay_us(0.104167);
		
		c >>=1;
		if(GPIO_ReadInputDataBit(UART_PORT, RX_PIN) == Bit_SET){
			c |= 0x80;										// 01010000
			
		}
	}
	
			delay_us(0.104167);
		
		return c;
}


// Dinh nghia cac che do cua parity
typedef enum{
	PARITY_NONE,
	PARITY_EVEN,
	PARITY_ODD
}ParityType;

// Dinh nghia stop bit
typedef enum{
	STOPBITS_1,
	STOPBITS_2
}StopBits;

// Dinh nghia data bit
typedef enum{
	DATA_5 = 5,
	DATA_6,
	DATA_7,
	DATA_8,
	DATA_9
}DataBits;
 
// dinh nghia config UART
typedef struct {
	uint32_t baud_rate;
	ParityType parity;
	StopBits stop_bits;
	DataBits data_bits;
}UART_Config;

static UART_Config global_uart_config;

void uart_init(uint32_t baud, ParityType parity, StopBits stop_bits, DataBits data_bits){

	global_uart_config.baud_rate = baud;
	global_uart_config.parity = parity;
	global_uart_config.data_bits = data_bits ;
	global_uart_config.stop_bits = stop_bits;
	
}

void UART_Transmit(uint16_t data){
	
	//Tinh toan thoi gian delay
	uint32_t delay_time = global_uart_config.baud_rate; // tinh toan 
	
	while(GPIO_ReadInputDataBit(UART_PORT, TX_PIN) != Bit_RESET);
	
		// Bat dau truyen bit start
		GPIO_ResetBits(UART_PORT, TX_PIN);
		
		delay_us(delay_time); 								// 9600bits/1000ms ->1bit = 0.104167 ms
		
		// Gui bit du lieu
		for(int i =0; i< global_uart_config.data_bits; i++){
			if(data & 1){												// 1 = 0b00000001
				GPIO_SetBits(UART_PORT, TX_PIN);
			}
			else{
				GPIO_ResetBits(UART_PORT, TX_PIN);
			}
			delay_us(delay_time);
			
			data >>= 1;
		}
		
		
		 
		// Truyen bit chan le
		if(global_uart_config.parity != PARITY_NONE){
		// chuaxong
		}
		
		
		
		//Truyen stop bit
	
		GPIO_SetBits(UART_PORT, TX_PIN);
		delay_us(delay_time);
		
		if(global_uart_config.stop_bits == STOPBITS_2){
			GPIO_SetBits(UART_PORT, TX_PIN);
			delay_us(delay_time);
		}
	}		
		
int main(){
	
	UART_Init();
	//
	uart_init (9600, PARITY_NONE, STOPBITS_1, DATA_8);
	
	while(1){
		
		//UART_SendChar('A');
		UART_Transmit('H');
		
		char Received = UART_ReceiveChar();
	}
	
	return 0;
}