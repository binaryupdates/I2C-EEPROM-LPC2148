/**********************************************************************************************
 File          : Timer.c
 Date          : Jan 2013
 Target      	 : LPC2148
 Description   : Functions related to the Timer
***********************************************************************************************/

#include <LPC214X.H>
#include "Type.h"
#include "Timer.h"

/**************************************************
Timer0 is used for delay in micro sec
Timer1 is used for time out delay

Timer is initialized to simply count at a specified
frequency(Configuration.h).
Count can be read from register TC. 
****************************************************/


/*
Initialises the timers
*/
void TIMER_Init (void)
{
	// Power ON timer peripheral 
	PCONP	|= 0x00000006;

	// TPC: Timer Prescaler counter
	// the counter is incremented once every TPC+1 cycles of PCLK

	T0PR	= PRESCALER0;
	//T1PR	= PRESCALER1;	

	// TCR: Timer  Control Register
	// 2: reset counters (both timer and prescaler)
	// 1: enable counting

	T0TCR=2;
	//T1TCR=2;

}

/*
waits for next tick in timer
*/
void TIMER_WaitForNextTick (void)
{
	uint32 start=T0TC;
	
	while (T0TC==start) {}	// wait until timer counter changes, then leave
}
