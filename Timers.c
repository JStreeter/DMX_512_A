/// @file    Timers.c
/// @author  Jonathan Streeeter
/// @date    Septmber 28, 2015
/// @brief   Timers and fun stuff
/// @details 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	// INCLUDES
////////////////////////////////////////////////////////////////////////////////
#include <main.h>
#include "TM4C123GH6PM.h"                    // Device header
#include "Serial.h"
#include "inc/hw_timer.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/uart.h"
#include "inc/hw_gpio.h"
#include "driverlib/udma.h"
#include <stdio.h>
#include "driverlib/gpio.h"
#include "globals.h"
////////////////////////////////////////////////////////////////////////////////
	// EXTERNALS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	// GLOBALS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	//  CONSTANTS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void TIMER0A_Handler()
{	
	static U8 StateOfLevels = 0;
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);//TIMER TIME OUT
	//110 uS have already Passed.
	//OR
	//14 uS have passed and the data should shart sending
	if(StateOfLevels == 0)
	{
		//Release the TX line
		TIMER0->TAILR = 450; // 12 uSeconds
		TIMER0->CTL |= TIMER_CTL_TAEN | TIMER_CTL_TBEN;
		StateOfLevels = 1;
		if(MaxSend)
		{
			PULLDOWNER = 1; //OPEN DRAIN 0 is IS Float!!!
		}
	}
	else
	{
		if(MaxSend)
		{
			StateOfLevels = 0;
			if(PingPongSemaphore == 0)
			{
				uDMAChannelTransferSet( UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
										A_DMX,
										(void *)(UART1_BASE + UART_O_DR),
										MaxSend + 2);
			}
			else
			{
				uDMAChannelTransferSet( UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
										B_DMX,
										(void *)(UART1_BASE + UART_O_DR),
										MaxSend + 2);//Counting So + 1 for data byte + plus 1 for actual data
			}
			
			LastPingPongSemaphore = PingPongSemaphore;
			uDMAChannelEnable(UDMA_CHANNEL_UART1TX);
			UARTDMAEnable(UART1_BASE, UART_DMA_TX);
			uDMAChannelRequest(UDMA_CHANNEL_UART1TX);
//			BLUE_LED ^= 1;
		}
	}	

	return;
}

void TIMER1A_Handler()// 1/ 40 Seconds
{
	TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);//TIMER TIME OUT
	//Set the Time 0 and drop the TX line
	//Auto Resets
	TIMER0->TAILR = 4950; // 112 uSeconds
	TIMER0->CTL |= TIMER_CTL_TAEN | TIMER_CTL_TBEN;
	Semaphore += 1;
	if(MasterSlave == Master)
	{
		PULLDOWNER = 0; //OPEN DRAIN 1 is DOWN!!!
		DEro = 1;//Turn on the abiltity for the device to transmit
	}
	else
	{
		DEro = 0;//Turn on the abiltity for the device to transmit
	}
	
}
//This is the Keep safe timer for the 
void TIMER2A_Handler()// 1/ 40 Seconds
{
	TimerIntClear(TIMER2_BASE,TIMER_TIMA_TIMEOUT);//TIMER TIME OUT
//	Incoming_Counter = 0;
//	TIMER2->TAILR = 10000; // 112 uSeconds
//	TIMER2->CTL |= TIMER_CTL_TAEN | TIMER_CTL_TBEN;
	RED_LED ^= 1;
}
