/*
* File: Dio.h
* Author: Tran Quang Luan
* Date: --/--/--
* Description: Specification of DIO Driver.
*/

#ifndef DIO_H
#define DIO_H
#include "stm32f10x_gpio.h"
#include<stm32f10x.h>
#include<stm32f10x_rcc.h>
// Types Imported from Std_Types.h file
typedef struct{
	uint16_t vendorID;
	uint16_t moduleID;
	uint8_t sw_major_version;
	uint8_t sw_minor_version;
	uint8_t sw_patch_version;
} Std_VersionInfoType;
//

/* 
* Define all the channels for Autosar. 
* Channels will be numbered from 0 to the max number of Pin.
* For example, ChannelId DIO_CHANNEL_0 to 15 will be the pin from 0 to 15 in GPIOA, 
  ChannelId 16 to 31 will be in GPIOB and so on...
*/
// DIO_CHANNEL_ : channel_id
#define DIO_CHANNEL_0 (Dio_ChannelType)0 //GPIOA, Pin 0
#define DIO_CHANNEL_1 (Dio_ChannelType)1
#define DIO_CHANNEL_2 (Dio_ChannelType)2
#define DIO_CHANNEL_3 (Dio_ChannelType)3
#define DIO_CHANNEL_4 (Dio_ChannelType)4
#define DIO_CHANNEL_5 (Dio_ChannelType)5
#define DIO_CHANNEL_6 (Dio_ChannelType)6
#define DIO_CHANNEL_7 (Dio_ChannelType)7
#define DIO_CHANNEL_8 (Dio_ChannelType)8
#define DIO_CHANNEL_9 (Dio_ChannelType)9
#define DIO_CHANNEL_10 (Dio_ChannelType)10
#define DIO_CHANNEL_11 (Dio_ChannelType)11
#define DIO_CHANNEL_12 (Dio_ChannelType)12
#define DIO_CHANNEL_13 (Dio_ChannelType)13
#define DIO_CHANNEL_14 (Dio_ChannelType)14
#define DIO_CHANNEL_15 (Dio_ChannelType)15 //GPIOA, Pin 15

#define DIO_CHANNEL_16 (Dio_ChannelType)16 //GPIOB, Pin 0
#define DIO_CHANNEL_17 (Dio_ChannelType)17
#define DIO_CHANNEL_18 (Dio_ChannelType)18
#define DIO_CHANNEL_19 (Dio_ChannelType)19
#define DIO_CHANNEL_20 (Dio_ChannelType)20
#define DIO_CHANNEL_21 (Dio_ChannelType)21
#define DIO_CHANNEL_22 (Dio_ChannelType)22
#define DIO_CHANNEL_23 (Dio_ChannelType)23
#define DIO_CHANNEL_24 (Dio_ChannelType)28
#define DIO_CHANNEL_25 (Dio_ChannelType)25
#define DIO_CHANNEL_26 (Dio_ChannelType)26
#define DIO_CHANNEL_27 (Dio_ChannelType)27
#define DIO_CHANNEL_28 (Dio_ChannelType)28
#define DIO_CHANNEL_29 (Dio_ChannelType)29
#define DIO_CHANNEL_30 (Dio_ChannelType)30
#define DIO_CHANNEL_31 (Dio_ChannelType)31 //GPIOB, Pin 15

// The same to remained Ports.

/* 
* DIO_GET_PORT_FROM_CHANNEL_ID(_channelId) (_channelId/16)
* Get DIO Port.
* For example, The result will be 0 if ChannelId in range of 0-15, which means it's in GPIOA.
*/ 
#define DIO_GET_PORT_FROM_CHANNEL_ID(_channelId) (_channelId/16)

/* 
* DIO_GET_BIT_FROM_CHANNEL_ID(_channelId) (1<<(_channelId%16))
* Get the DIO Bit.
* For example, if result of "%" is 2, it means the channel will be 2 or 18, 
  so after shifting we have the bit of that channel in Port.
*/
#define DIO_GET_BIT_FROM_CHANNEL_ID(_channelId) (1<<(_channelId%16))
/*Dio_ChannelType: tuong ung voi so chan nhu stm32*/
#define Dio_ChannelType uint8_t

/*Dio_PortType: so thu tu cua port */
#define Dio_PortType uint16_t

/*Dio_PortLevelType : moi port se duoc dieu khien boi thanh ghi khac nhau
-thuong se chon thanh ghi lon nhat(16 ghi) .Duoc dinh nghia lai la uint16*/
#define Dio_PortLevelType uint16_t

/*Dio_LevelType : co 2 kieu tra ve la STD_LOW va STD_HIGH*/
#define Dio_LevelType uint8_t

#define STD_LOW (uint8_t)0x00
#define STD_HIGH (uint8_t)0x01

/* Define your module's version information */
#define DIO_MODULE_ID  (123)  // Example module ID
#define DIO_VENDOR_ID  (456)  // Example vendor ID
#define DIO_SW_MAJOR_VERSION   (1) // Example software major version
#define DIO_SW_MINOR_VERSION   (0) // Example software minor version
#define DIO_SW_PATCH_VERSION   (0) // Example software patch version

// Define the Type for GPIO_TypeDef Pointer.
typedef GPIO_TypeDef* GPIO_TypeDefPtr;

typedef struct {
	uint16_t mask; // moi bit tuong ung voi moi chanel 
	uint8_t offset; // cho biet vi tri ma mask tro toi trong thanh ghi
	Dio_PortType port; // la bien xac dinh port
}Dio_ChannelGroupType;


// Dio_ReadChannel : doc gia tri cua chanel
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId);
// Dio_WriteChannel : viet gia tri cua chanel
void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level);
// Dio_ReadPort : doc tat ca gia tri cua nhieu chanel trong 1 port
Dio_PortLevelType Dio_ReadPort (Dio_PortType PortId);
// Dio_WritePort : viet cac gia tri cua tat ca chanel trong 1 port
void Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level);
// Dio_WriteChannelGroup : viet cho nhieu chanel ma minh chi dinh trong 1 port
void Dio_WriteChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level);
// Dio_ReadChannelGroup : doc nhieu chanel ma minh chi dinh trong 1 port
Dio_PortLevelType Dio_ReadChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr);

//Dio_GetVersionInfo : lay thong so cua gia tri DIo cap nhat lan cuoi
void Dio_GetVersionInfo (Std_VersionInfoType* VersionInfo);

//Dio_FlipChannel: ham dao gia tri cua 1 channel
Dio_LevelType Dio_FlipChannel (Dio_ChannelType ChannelId);

// Dio_MaskedWritePort : ham tra ve gia tri cua port voi cac gia tri mask kem theo
void Dio_MaskedWritePort (Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask);

#endif 
