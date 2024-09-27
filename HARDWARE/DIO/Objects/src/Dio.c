/*
* File: Dio.c
* Author: Tran Quang Luan
* Date: --/--/--
* Description: Specification of DIO Driver.
*/

#include "Dio.h"
// Create a list of pointer to all ports of STM32F1.
const GPIO_TypeDefPtr GPIO_ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF };

/*
* Function: Dio_ReadPort
* Description: Returns the level of all channels of that port.
* Input: 
	PortId - ID of DIO Port.
* Output: 
	Return - Level of all channels of that port.
*/
Dio_PortLevelType Dio_ReadPort (Dio_PortType PortId){
	Dio_LevelType Level = 0x0000;
	Level = GPIO_ReadInputData(GPIO_ports[PortId]);
	return Level;
}

/*
* Function: Dio_WritePort
* Description: Service to set a value of the port.
* Input: 
	PortId - ID of DIO Port.
	Level - Value to be written.
* Output: 
	Return - None.
*/
void Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level){
	GPIO_Write(GPIO_ports[PortId], Level);
}

/*
* Function: Dio_ReadChannel
* Description: Returns the value of the specified DIO channel.
* Input: 
	PortId - ID of DIO channel.
* Output: 
	Return - None.
*/
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId){
	Dio_LevelType Level;
	Dio_PortLevelType portVal = Dio_ReadPort(DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId));
	Dio_PortLevelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);
	
	if ((portVal & bit) != STD_LOW){
		Level = STD_HIGH;
	} else{
		Level = STD_LOW;
	}
	
	return Level;
}

/*
* Function: Dio_WriteChannel
* Description: Service to set a level of a channel.
* Input: 
	PortId - ID of DIO channel.
	Level - Value to be written.
* Output: 
	Return - None.
*/
void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level){
	Dio_PortType port = DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId);
	Dio_ChannelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);
	
	Dio_PortLevelType portVal = Dio_ReadPort(port);
	if(Level == STD_HIGH){
		portVal |= bit;
	} else {
		portVal &=~bit;
	}
	Dio_WritePort(port, portVal);
}

/*
* Function: Dio_WriteChannelGroup
* Description: Service to set a subset of the adjoining bits of a port to a specified level.
* Input:
	ChannelGroupIdPtr - Pointer to ChannelGroup.
	Level - Value to be written.
* Output: 
	Return - None.
*/
void Dio_WriteChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level){
	// Shift the Value and apply the Mask to choose the wanted bit.
	Level = (Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask; // dich chuyen cac gia tri sang trai sau do chon cac bit mong muon
	// Read the port so clear out the masked bits
	/*doc cac gia tri cua cong sau do xoa cac gia tri khong mong muon =>portval duoc cac gia tri mong muon*/
	Dio_PortLevelType portVal = Dio_ReadPort(ChannelGroupIdPtr->port) & (~ChannelGroupIdPtr->mask);
	// Apply to update the level.
	portVal |= Level;

	Dio_WritePort(ChannelGroupIdPtr->port, portVal);
}

/*
* Function: Dio_ReadChannelGroup
* Description: This Service reads a subset of the adjoining bits of a port.
* Input:
	ChannelGroupIdPtr - Pointer to ChannelGroup.
	Level - Value to be written.
* Output: 
	Return - Level of a subset of the adjoining bits of a port.
*/
Dio_PortLevelType Dio_ReadChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr){
	Dio_LevelType Level;
	// Update masked values
	Level = Dio_ReadPort(ChannelGroupIdPtr->port) & ChannelGroupIdPtr->mask;
	
	// Shift down
	Level = Level >> ChannelGroupIdPtr->offset;
	
	return Level;
}

/*
* Function: Dio_FlipChannel
* Description: Service to flip (change from 1 to 0 or from 0 to 1) the level of a channel and return
			   the level of the channel after flip
* Input:
	ChannelId - ID of DIO channel.
* Output: 
	Return - STD_HIGH: The physical level of the corresponding Pin is STD_HIGH.
		   - STD_LOW: The physical level of the corresponding Pin is STD_LOW.
*/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId){
	Dio_LevelType Level;
	//Read curent value of Channel.
	Level = Dio_ReadChannel(ChannelId);
	//Flip the value
	if(Level == STD_HIGH){
		Level = STD_LOW;
	} else {
		Level = STD_HIGH;
	}
	//Wrtie it back
	Dio_WriteChannel(ChannelId, Level);
	
	return Level;
}

/*
* Function: Dio_MaskedWritePort
* Description: Service to set the value of a given port with required mask.
* Input:
	PortId - ID of DIO Port.
	Level - Value to be written.
	Mask - Channels to be masked in the port.
* Output: 
	Return - None.
*/
void Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask){
	// Read the current value of the port
	Dio_PortLevelType currentValue = Dio_ReadPort(PortId);
	// Apply the mask to the level
	Dio_PortLevelType maskedLevel = (currentValue & ~Mask) | (Level & Mask);
	// Write the modified value back to the port
	Dio_WritePort(PortId, maskedLevel);
}

/*
* Function: Dio_GetVersionInfo
* Description: Service to get the version information of this module.
* Input:
	VersionInfo - Pointer to where to store the version information of this module.
* Output: 
	Return - None.
*/
void Dio_GetVersionInfo (Std_VersionInfoType* VersionInfo){
	if (VersionInfo != 0) {
        /* Fill VersionInfo structure with module information */
        VersionInfo->moduleID = DIO_MODULE_ID;
        VersionInfo->sw_major_version = DIO_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = DIO_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = DIO_SW_PATCH_VERSION;
        VersionInfo->vendorID = DIO_VENDOR_ID;
    } else {
        /* Do nothing if VersionInfo pointer is NULL (as per AUTOSAR) */
    }	
	
}