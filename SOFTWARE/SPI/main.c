#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// �?nh nghia c�c ch? d? SPI

typedef enum {
    SPI_MODE0 = 0,   // CPOL = 0, CPHA = 0
    SPI_MODE1,       // CPOL = 0, CPHA = 1
    SPI_MODE2,       // CPOL = 1, CPHA = 0
    SPI_MODE3        // CPOL = 1, CPHA = 1
} SPI_Mode;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t sck_pin;
    uint16_t mosi_pin;
    uint16_t miso_pin;
    SPI_Mode mode;
    uint32_t speed_kHz;
} SPI_Config;

// H�m delay micro gi�y d? d�ng xung clock, 72Mhz

void DelayUs(uint32_t us) {
    us *= 72;
    while (us--);
}

// Kh?i t?o c�c ch�n SPI

void spi_init(SPI_Config* spiConfig) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // B?t clock cho GPIO

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // C?u h�nh SCK v� MOSI output ch? push-pull

    GPIO_InitStructure.GPIO_Pin = spiConfig->sck_pin | spiConfig->mosi_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(spiConfig->port, &GPIO_InitStructure);

    // C?u h�nh MISO l� input

    GPIO_InitStructure.GPIO_Pin = spiConfig->miso_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(spiConfig->port, &GPIO_InitStructure);
}

// Truy?n 1 byte nh?n 1 byte

uint8_t SPI_TransferByte(SPI_Config* spiConfig, uint8_t byteToSend) {
    uint8_t byteReceived = 0;

    uint32_t spi_delay_us = 1000 / spiConfig->speed_kHz;

    for (int i = 0; i < 8; i++) {
        switch (spiConfig->mode) { // truyen
            // C�c ch? d? SPI
            case SPI_MODE0:
            case SPI_MODE2:
                // Trong c�c ch? d? SPI_MODE0 v� SPI_MODE2,
                // t�n hi?u SCK c?n du?c dua v? m?c logic 0 tru?c khi truy?n d? li?u
                GPIO_WriteBit(spiConfig->port, spiConfig->sck_pin, Bit_RESET); // M?c 0
                break;
            case SPI_MODE1:
            case SPI_MODE3:
                // Trong c�c ch? d? SPI_MODE1 v� SPI_MODE3,
                // t�n hi?u SCK c?n du?c d?t ? m?c logic 1 tru?c khi truy?n d? li?u
                GPIO_WriteBit(spiConfig->port, spiConfig->sck_pin, Bit_SET); // M?c 1
                break;
            default:
                break;
        }

        if (byteToSend & (1 << (7 - i))) { // Truy?n d? li?u
            GPIO_SetBits(spiConfig->port, spiConfig->mosi_pin);
        } else {
            GPIO_ResetBits(spiConfig->port, spiConfig->mosi_pin);
        }

        DelayUs(spi_delay_us / 2); // Delay n?a chu k?

        switch (spiConfig->mode) { // nhan
            // Trong c�c ch? d? SPI_MODE0 v� SPI_MODE1,
            // t�n hi?u SCK c?n du?c d?t ? m?c logic 1 sau khi truy?n d? li?u
            case SPI_MODE0:
            case SPI_MODE1:
                GPIO_WriteBit(spiConfig->port, spiConfig->sck_pin, Bit_SET);
                break;
            case SPI_MODE2:
            case SPI_MODE3:
                // Trong c�c ch? d? SPI_MODE2 v� SPI_MODE3,
// t�n hi?u SCK c?n du?c d?t v? m?c logic 0 sau khi truy?n d? li?u
                GPIO_WriteBit(spiConfig->port, spiConfig->sck_pin, Bit_RESET);
                break;
            default:
                break;
        }

        // �?c gi� tr? t? MISO

        if (GPIO_ReadInputDataBit(spiConfig->port, spiConfig->miso_pin)) {
            byteReceived |= (1 << (7 - i));
        }

        DelayUs(spi_delay_us /2);
    }

    return byteReceived;
}

void SPI_TransferBytes(SPI_Config* spiConfig, uint8_t* dataToSend, uint8_t* dataReceived, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        dataReceived[i] = SPI_TransferByte(spiConfig, dataToSend[i]);
    }
}

void SPI_Transmit(SPI_Config* spiConfig, uint8_t* dataToSend, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        SPI_TransferByte(spiConfig, dataToSend[i]);
    }
}

void SPI_Receive(SPI_Config* spiConfig, uint8_t* dataReceived, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        dataReceived[i] = SPI_TransferByte(spiConfig, 0xFF);
    }
}

int main() {
    SPI_Config spi = {
        .port = GPIOA,
        .sck_pin = GPIO_Pin_5,
        .mosi_pin = GPIO_Pin_7,
        .miso_pin = GPIO_Pin_6,
        .mode = SPI_MODE0,
        .speed_kHz = 1000
    };

    // Kh?i t?o SPI
    spi_init(&spi);

    uint8_t dataToSend[5] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
    uint8_t dataReceived[5];

    SPI_TransferBytes(&spi, dataToSend, dataReceived, 5);

    while (1) {
        // Code
    }
}