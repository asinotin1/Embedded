#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void GPIO_Config(void);
void delay(uint16_t time);

int main(void)
{
    GPIO_Config();
    while(1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13); // Đèn sáng
        delay(5000); // Đợi 5000 mili giây (5 giây)

        GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Đèn tắt
        delay(5000); // Đợi 5000 mili giây (5 giây)
    }
}

void delay(uint16_t time)
{
    volatile uint32_t i, j; // Sử dụng volatile để tránh việc compiler tối ưu hóa đi vòng lặp
    for (i = 0; i < time; i++)
        for (j = 0; j < 7200; j++); // Tạo ra một thời gian trễ hợp lý
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // Bật xung clock cho GPIOC

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Đặt chân GPIOC-13 là chế độ đầu ra Push-Pull
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
