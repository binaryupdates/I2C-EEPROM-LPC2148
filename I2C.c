/**********************************************************************************************
 File          : I2C.c
 Date          : Jan 2013
 Target      	 : LPC2148
 Description   : Functions related to the I2C			  
***********************************************************************************************/

#include <LPC214X.H>
#include <Stdio.h>
#include "Type.h"
#include "I2C.h"
#include "TIMER.h"

/***************************************************
Contains all EEPROM related functions. 
Writing to and reading from the EEPROM fuctions.
All the user data, settings and logged data
will goes into EEPROM.
I2C0 is used for interfacing.
****************************************************
*/


/*
Initialises the I2C protocol and port pins.
*/
void I2C_Init (void)
{
	// Power on I2C0 peripheral
	PCONP	|= 0x00000080;
	
	// Define port pin as SDA and SCL
	PINSEL0	|= 0x00000050 ;

	I2C0CONCLR	= 0x6C;	// clear all I2C config bits
	I2C0CONSET	= 0x40;	// set I2EN
	
	// I2C Clock Duty Cycle (high and low)	
	I2C0SCLH 	= PERIFERAL_OPERATING_FREQUENCY_IN_HZ/(2*EEPROM_OPERATING_FREQUENCY_IN_HZ);
	I2C0SCLL 	= PERIFERAL_OPERATING_FREQUENCY_IN_HZ/(2*EEPROM_OPERATING_FREQUENCY_IN_HZ);
}


/*
Waits until given status occured.
Return:	True on status occured and 
		False on time out  
*/
BOOL I2C_WaitStatus (uint8 u8status)
{	
	TIMER0_RESET();
	TIMER0_ENABLE();	
	
	while (T0TC < EEPROM_WAIT_TIME_OUT) 
	{
		if (I2C0CONSET & 8) // poll SI bit
		{
			if (I2C0STAT == u8status)
			{
				TIMER0_DISABLE();
				return TRUE;
			}
		}
	}
	TIMER0_DISABLE();
	return FALSE;	
}


/*
Reads data from EEPROM.
Return:	True on valid data and 
		False on time out or any error with device 
*/
BOOL I2C_ReadFromEEPROM (uint32 u32startAddr, uint8 *u8ptr2arr, uint32 u32len)
{
	uint32	u32i;
	// Check for upper limit
	if (u32startAddr + u32len > EEPROM_SIZE)
		return FALSE;
	for (u32i=0;u32i<u32len;u32i++)
	{
		I2C0CONSET	= 0x20;			// Start set
		if (!I2C_WaitStatus(0x08))		// 0x08: ready for device address
			return FALSE;
		
		
		I2C0DAT		= EEPROM_DEVADDR;// addr[0]=0 means I2C write
		I2C0CONCLR	= 0x2C;			// clear all except I2EN
		if (!I2C_WaitStatus(0x18))		// 0x18: ready for data byte
			return FALSE;

		// Transmit start address - Dummy byte write
		I2C0DAT		= (u32startAddr & 0x000000FF) ;
#ifndef	EEPROM_24C02
		I2C0CONCLR	= 0x2C;			// clear all except I2EN
		if (!I2C_WaitStatus(0x28))		// 0x28: ACK has been received
			return FALSE;
		I2C0CONCLR	= 0x08;			// clear SI flag
		I2C0DAT		= ((u32startAddr & 0x0000FF00)>>8) &0xFF;	
#endif	
		I2C0CONCLR	= 0x2C;			// clear all except I2EN
		if (!I2C_WaitStatus(0x28))		// 0x28: ACK has been received
			return FALSE;
		I2C0CONCLR	= 0x08;			// clear SI flag
		I2C0CONSET	= 0x10;			// generate stop condition
		
		// Read data - Sequential mode.
		I2C0CONSET	= 0x20;			// Start set
		if (!I2C_WaitStatus(0x08))		// 0x08: ready for device address
			return FALSE;
		I2C0DAT		= EEPROM_DEVADDR|0x01;	// addr[0]=1 means I2C read
		I2C0CONCLR	= 0x28;			// clear all except I2EN and AA
		if (!I2C_WaitStatus(0x40))		// 0x40: ready for data byte
			return FALSE;			
	
		I2C0CONCLR	= 0x2C;			// clear all except I2EN 
		if (!I2C_WaitStatus(0x58))		// 0x58: data byte received return ACK
			return FALSE;
			
		u8ptr2arr[u32i]	= (uint8)I2C0DAT ;
		u32startAddr++;		
		I2C0CONSET	= 0x10;			// generate stop condition	
		I2C0CONCLR	= 0x2C;
		
	}
	return TRUE;
}


/*
Writes data to EEPROM.
Return:	True on successful write and 
		False on time out or any error with device 
*/
BOOL I2C_WriteToEEPROM (uint32 u32startAddr, uint8 *u8ptr2arr, uint32 u32len)
{
	uint32	u32i,u32j;
	
	// Check for upper limit
	if (u32startAddr + u32len > EEPROM_SIZE)
		return FALSE;
	
	// write data byte wise
	for (u32i = 0; u32i < u32len; u32i++)
	{		
		I2C0CONSET	= 0x20;			// Start set
		if (!I2C_WaitStatus(0x08))		// 0x08: ready for device address
			return FALSE;
			
		
		I2C0DAT		= EEPROM_DEVADDR;// addr[0]=0 means I2C write
		I2C0CONCLR	= 0x2C;			// clear all except I2EN
		if (!I2C_WaitStatus(0x18))		// 0x18: ready for data byte
			return FALSE;
	
				
		// Transmit start address 
		I2C0DAT		= (u32startAddr & 0x000000FF);
#ifndef	EEPROM_24C02
		I2C0CONCLR	= 0x2C;			// clear all except I2EN
		if (!I2C_WaitStatus(0x28))		// 0x28: ACK has been received
			return FALSE;
		
				
		//I2C0CONCLR	= 0x08;			// clear SI flag
		I2C0DAT		= ((u32startAddr & 0x0000FF00)>>8) &0xFF;	
#endif	
		I2C0CONCLR	= 0x2C;			// clear all except I2EN
		if (!I2C_WaitStatus(0x28))		// 0x28: ACK has been received
			return FALSE;
			

		I2C0DAT		= (u8ptr2arr[u32i])&0x000000FF;
		u32startAddr++;
		I2C0CONCLR	= 0x2C;			// clear all except I2EN		
		if (!I2C_WaitStatus(0x28))		// 0x28: ACK has been received
			return FALSE;
			
		
		I2C0CONSET	= 0x10;			// generate stop condition
		I2C0CONCLR	= 0x2C;	
		
		// Poll for write done
		for (u32j=20;u32j>0;u32j--)
		{
			I2C0CONSET	= 0x20;		// Start set
			if (!I2C_WaitStatus(0x08))	// 0x08: ready for device address
				return FALSE;
			
			I2C0DAT		= EEPROM_DEVADDR;// addr[0]=0 means I2C write
			I2C0CONCLR	= 0x2C;		// clear all except I2EN
			if (I2C_WaitStatus(0x18))	// 0x18: ready for data byte
				break;	
			else				
			{
				I2C0CONCLR	= 0x2C;	
				I2C0CONSET	= 0x10;	// generate stop condition
				I2C0CONCLR	= 0x2C;			
			}				
		}	
		I2C0CONSET	= 0x10;			// generate stop condition
		I2C0CONCLR	= 0x2C;			
	}

	return TRUE;
}
