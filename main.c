/***************************************************************************************************************
 Project       : I2CLPC2148
 File					 : main.c
 Date          : May 2016
 Target      	 : LPC2148
 Description   : Functions related to the I2C EEPROM. The program reads and write the data in EEPROM through I2C
				  
****************************************************************************************************************/
#include <LPC214X.H>
#include <Stdio.h>
#include "Type.h"
#include "uart.h"
#include "I2C.h"
#include "TIMER.h"

/***************************************************
Main function.
***************************************************/

int main(void)
{
	uint8 write_buffer[20] = {'B', 'I', 'N', 'A', 'R', 'Y',0};
	uint8 read_buffer[20];
	uint32 delay;
	
	PINSEL0 |= 0x00000005 ;
		
	UART0_Init(); // Initialize UART0
	I2C_Init();		// Initialize I2C0
	TIMER_Init();	// Initialize Timer
	
	UART0_Write_Text("********* LPC2148 ARM7 I2C EEPROM Demo **********\n\n\r");
	UART0_Write_Text("Initialization done. \n\r\n");
	
	while(1)
	{
		if (!I2C_WriteToEEPROM(0,  write_buffer, 20))		// write into EEPROM
			UART0_Write_Text("\nMemory write error.");
			
		if (!I2C_ReadFromEEPROM(0, read_buffer, 20))		// read from EEPROM
			UART0_Write_Text("\nMemory Read error..");
		
		UART0_Write_Text("\n\r The Read Data are: \t");
		UART0_Write_Text((char *)read_buffer);					// display data on serial port				
		UART0_Write_Text("\n\r");
		
		for(delay=0;delay<=5000000;delay++);
	}
	
	//return 0 ;
}
