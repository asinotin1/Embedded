#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define SDA_PIN GPIO_Pin_7
#define SCL_PIN GPIO_Pin_6

#define I2C_PORT GPIOB

void I2C_ACK(void);
void I2C_NACK(void);
uint8_t i2c_ReadACK(void);
void I2C_SOFWARE_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStruture;

    // c?u hình chân scl và SDA
    GPIO_InitStruture.GPIO_Pin = SDA_PIN | SCL_PIN; // c?u hình output
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruture.GPIO_Speed = GPIO_Speed_50MHz; // d? t?c d? cao nh?t
    GPIO_Init(I2C_PORT,&GPIO_InitStruture);


    // d?t tr?ng thái ban d?u là hight cho SDA và SCL

    GPIO_SetBits(I2C_PORT,SDA_PIN);
    GPIO_SetBits(I2C_PORT,SCL_PIN);
    
}

void I2C_Start(void)
{
    // g?i l?i d? xem có b? l?i hay không . không quan tr?ng l?m
    GPIO_SetBits(I2C_PORT,SDA_PIN);
    GPIO_SetBits(I2C_PORT,SCL_PIN);


// start thì SDA v? 0 tru?c r?i m?i SCL
    GPIO_ResetBits(I2C_PORT,SDA_PIN);
    GPIO_ResetBits(I2C_PORT,SCL_PIN);

}
void I2C_Stop(void)
{

    GPIO_ResetBits(I2C_PORT,SDA_PIN);
    GPIO_ResetBits(I2C_PORT,SCL_PIN);

// stop thì SCL v? 1 tru?c r?i m?i t?i SDA

    GPIO_SetBits(I2C_PORT,SCL_PIN);
    GPIO_SetBits(I2C_PORT,SDA_PIN);
}

// các xung SCL d? d?y byte di 
// SDA thì có nhi?m v? ghi nh?n byte
void I2C_SendByte(uint8_t byte) // truy?n 1 byte
{

    for(int i = 0;i<8;i++)
    {
         GPIO_ResetBits(I2C_PORT,SCL_PIN); // chân scl ph?i low

         /* ki?m tra xem byte có ph?i là 1 hay 0 vì khi byte là 1 thì and v?i 0x80 nó v?n s? là 1 còn ngu?c l?i là 0    */
         if(byte & 0x80) 
         {
            GPIO_SetBits(I2C_PORT,SDA_PIN); // g?i byte 1
         }
         else
         {
            GPIO_ResetBits(I2C_PORT,SDA_PIN); // g?i byte 0
         }
          GPIO_SetBits(I2C_PORT,SCL_PIN);
          byte <<= 1; // d?ch byte sang trái d? g?i byte ti?p theo
    }
    GPIO_ResetBits(I2C_PORT,SCL_PIN); // kéo xu?ng(không quan tr?ng l?m)
}

void I2C_SendByteS(uint8_t *data,uint32_t lenght) // truy?n nhi?u byte
{
    for (int  i = 0; i < lenght; i++)
    {
       I2C_SendByte(data[i]);
       if(i2c_ReadACK())
       {
        // x? lý l?i
        break;
       }
    }

}
uint8_t I2C_ReadByte(void) // d?c 1 byte
{
    uint8_t byte = 0;
    // d?t SDA thành input nên ph?i c?u hình l?i
    GPIO_InitTypeDef GPIO_InitStruture;
    GPIO_InitStruture.GPIO_Pin = SDA_PIN;
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(I2C_PORT,&GPIO_InitStruture);

    /*GPIO_Mode_IPD:
Mô t?: Input with Pull-down.
Gi?i thích: Chân GPIO du?c c?u hình làm d?u vào v?i m?t di?n tr? pull-down n?i b? kích ho?t.
Khi không có tín hi?u nào du?c áp d?ng lên chân này, nó s? du?c kéo v? m?c th?p (GND).
*/
    for(int i=0 ;i<8;i++)
    {
    byte <<= 1;//d?ch byte sang trái
    GPIO_SetBits(I2C_PORT,SCL_PIN);
    if(GPIO_ReadInputDataBit(I2C_PORT, SDA_PIN)) // n?u là bit 1 thì s? vào còn bit 0 s? không vào
    {
        byte |=1;
    }
     GPIO_ResetBits(I2C_PORT,SCL_PIN); // nh?n byte xong thì kéo xu?ng m?c th?p
    }
    // d?t SDA thành output  
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(I2C_PORT,&GPIO_InitStruture);

    return byte;    
}

void I2C_ReadByteS(uint8_t *buffer,uint32_t lenght)// d?c nhi?u byte
{
    for (int  i = 0; i < lenght; i++)
    {
        
        buffer[i]= I2C_ReadByte();

        if(buffer[i] != '\0')
        {
            I2C_ACK();
        }else
        {
            I2C_NACK();
        }
    }
    
}

void I2C_ACK(void)
{
    
    GPIO_ResetBits(I2C_PORT,SCL_PIN); // SCL LOW
    GPIO_ResetBits(I2C_PORT,SDA_PIN); // SDA LOW

    GPIO_SetBits(I2C_PORT,SCL_PIN);   // SCL hight 
    GPIO_ResetBits(I2C_PORT,SCL_PIN); // SCL  low

}

void I2C_NACK(void)
{
    GPIO_ResetBits(I2C_PORT,SCL_PIN); // SCL LOW
    GPIO_SetBits(I2C_PORT,SDA_PIN); // SDA hight

    GPIO_SetBits(I2C_PORT,SCL_PIN);   // SCL hight 
    GPIO_ResetBits(I2C_PORT,SCL_PIN); // SCL  low
}

uint8_t i2c_ReadACK(void)
{
	uint8_t ACK;
    // SDA Là input
    GPIO_InitTypeDef GPIO_InitStruture;
    GPIO_InitStruture.GPIO_Pin = SDA_PIN;
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(I2C_PORT,&GPIO_InitStruture);

    GPIO_SetBits(I2C_PORT,SCL_PIN); //  SCL hight
    ACK = GPIO_ReadInputDataBit(I2C_PORT, SDA_PIN);
    GPIO_ResetBits(I2C_PORT,SCL_PIN); 
    
    // d?t SDA thành output  
    GPIO_InitStruture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(I2C_PORT,&GPIO_InitStruture);

    return ACK; // 0 là ack , 1 là nack
}

int main(int argc, char const *argv[])
{
    while(1)
    {
        //
    }
    return 0;
}
