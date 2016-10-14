/**********************************************************************************************
 File          : UART.c
 Date          : Jan 2013
 Target      	 : LPC2148
 Description   : Functions related to the UART)		  
***********************************************************************************************/
#include <LPC21xx.h>
#include "TYPE.h"
#include "UART.h"

/***************UART-0 Functions**************/
void UART0_Init(void)
{
	PINSEL0 |= 0x00000005;			//P0.0 as TX0 and P0.1 as RX0
	U0LCR = 0x83;								//Enable access to Divisor Latches
	//and Set 8 bit Character Length with 1 Stop bit and Parity Disabled
	//Access to Divisor Latches is Enabled, in order to write Baud Rate Generator Registers
	
	//Values to be written in Baud Rate Registers U0DLM and U0LL
	/*
	Formula is
	
	Baud_Rate = PCLK*MulVal / [(16*(256*U0DLM+U0DLL)*(MulVal + DivAddVal))]
	
	Example:-
	MulVal = 1;
	DivAddVal = 0;
	Baud_Rate = 9600;
	PCLK = 15MHz
	U0DLM = 0;
	
	Hence,
	U0DLL = 15000000/(9600*16) = 97.65625 = 98
	U0DLL = 98 = 0x62
	*/
	U0DLM = 0x00;
	U0DLL = 0x62;									//Baud Rate of 9600
	U0LCR = 0x03;									//Disable Access to Divisor Latches
}

void UART0_Write(unsigned char value)
{
	/*	
	THRE bit can be extracted by this U0LSR & 0x20
	THRE = 0 means data is present.
	THRE = 1 means register is empty.
	In order to transmit data, we have to wait will the THRE = 1,
	then only we can transmit data.
	*/
	
	while(!(U0LSR&0x20));						//THRE = 0 stay here
	U0THR = value;
}

void UART0_Write_Text(char * msg)
{
	while(*msg)
	{
		UART0_Write(*msg);
		msg++;
	}
}

unsigned char UART0_Read(void)
{
	/*
	Receiver Data Ready = U0LSR.0 bit
	RDR bit can be extracted by this U0LSR & 0x01
	RDR = 0 means no Data is Received in U0RBR
	RDR = 1 means that Data is present in U0RBR
	*/
	while(!(U0LSR & 0x01));						//RDR = 0 stay here
	return (U0RBR);
}

/***************UART-1 Functions**************/
void UART1_Init(void)
{
	PINSEL0 |= 0x00050000;			//P0.8 as TX1 and P0.9 as RX1
	U1LCR = 0x83;								//Enable access to Divisor Latches
	//and Set 8 bit Character Length with 1 Stop bit and Parity Disabled
	//Access to Divisor Latches is Enabled, in order to write Baud Rate Generator Registers
	
	//Values to be written in Baud Rate Registers U0DLM and U0LL
	/*
	Formula is
	
	Baud_Rate = PCLK*MulVal / [(16*(256*U0DLM+U0DLL)*(MulVal + DivAddVal))]
	
	Example:-
	MulVal = 1;
	DivAddVal = 0;
	Baud_Rate = 9600;
	PCLK = 15MHz
	U0DLM = 0;
	
	Hence,
	U0DLL = 15000000/(9600*16) = 97.65625 = 98
	U0DLL = 98 = 0x62
	*/
	U1DLM = 0x00;
	U1DLL = 0x62;									//Baud Rate of 9600
	U1LCR = 0x03;									//Disable Access to Divisor Latches
}

void UART1_Write(unsigned char value)
{
	/*	
	THRE bit can be extracted by this U0LSR & 0x20
	THRE = 0 means data is present.
	THRE = 1 means register is empty.
	In order to transmit data, we have to wait will the THRE = 1,
	then only we can transmit data.
	*/
	
	while(!(U1LSR&0x20));						//THRE = 0 stay here
	U1THR = value;
}

void UART1_Write_Text(unsigned char * msg)
{
	while(*msg)
	{
		UART1_Write(*msg);
		msg++;
	}
}

unsigned char UART1_Read(void)
{
	/*
	Receiver Data Ready = U0LSR.0 bit
	RDR bit can be extracted by this U0LSR & 0x01
	RDR = 0 means no Data is Received in U0RBR
	RDR = 1 means that Data is present in U0RBR
	*/
	while(!(U1LSR & 0x01));						//RDR = 0 stay here
	return (U1RBR);
}
