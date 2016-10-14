/**********************************************************************************************
 File          : UART.h
 Date          : Jan 2013
 Target      	 : LPC2148
 Description   : Functions related to the UART)		  
***********************************************************************************************/
#ifndef UART_H
#define UART_H

void UART0_Init(void);
void UART0_Write(unsigned char value);
void UART0_Write_Text(char *msg);
unsigned char UART0_Read(void);

void UART1_Init(void);
void UART1_Write(unsigned char value);
void UART1_Write_Text(unsigned char *msg);
unsigned char UART1_Read(void);

#endif /* UART0_H */
