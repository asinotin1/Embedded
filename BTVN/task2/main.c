#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "cjson.h"
 cJSON *json = cJSON_CreateObject(); 
   cJSON_AddStringToObject(json, "name", "John Doe"); 
   cJSON_AddNumberToObject(json, "age", 30); 
   cJSON_AddStringToObject(json, "email", "john.doe@example.com"); 
  
   // convert the cJSON object to a JSON string 
   char *json_str = cJSON_Print(json); 
  



// �?nh nghia c�c ch�n SPI
#define SD_SPI           SPI1
#define SD_SPI_PIN_SCK   GPIO_Pin_5
#define SD_SPI_PIN_MISO  GPIO_Pin_6
#define SD_SPI_PIN_MOSI  GPIO_Pin_7

// �?nh nghia ch�n CS (Chip Select)
#define SD_CS_PORT       GPIOA
#define SD_CS_PIN        GPIO_Pin_4

// H�m thi?t l?p SPI
void SPI_Configuration(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // B?t clock cho SPI v� GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    // Thi?t l?p ch�n SPI
    GPIO_InitStructure.GPIO_Pin = SD_SPI_PIN_SCK | SD_SPI_PIN_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SD_SPI_PIN_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Thi?t l?p ch�n CS
    GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SD_CS_PORT, &GPIO_InitStructure);

    // Thi?t l?p SPI
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SD_SPI, &SPI_InitStructure);

    // B?t SPI
    SPI_Cmd(SD_SPI, ENABLE);
}

// H�m ch?n th? SD
void SD_CS_Enable(void)
{
    GPIO_ResetBits(SD_CS_PORT, SD_CS_PIN);
}

// H�m b? ch?n th? SD
void SD_CS_Disable(void)
{
    GPIO_SetBits(SD_CS_PORT, SD_CS_PIN);
}

int main(void)
{
    FATFS fs;
    FIL file;
    FRESULT res;

    // Kh?i t?o SPI
    SPI_Configuration();

    // Kh?i t?o th? SD
    if (disk_initialize(0) != RES_OK)
    {
        // L?i kh?i t?o th? SD
        while (1);
    }

    // M? ho?c t?o file m?i
    res = f_open(&file, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (res == FR_OK)
    {
        // D? li?u c?n ghi v�o file
        char data[] = "Hello, world!";

        // Ghi d? li?u v�o file
        UINT bytesWritten;
        res = f_write(&file, data, sizeof(data) - 1, &bytesWritten);
        if (res == FR_OK)
        {
            // Ghi th�nh c�ng
        }

        // ��ng file
        f_close(&file);
    }

    while (1)
    {
        // V�ng l?p ch�nh
    }
}