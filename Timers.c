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
		TIMER0->TAILR = 750; // 12 uSeconds
		TIMER0->CTL |= TIMER_CTL_TAEN | TIMER_CTL_TBEN;
		StateOfLevels = 1;
	}
	else
	{
		if(MaxSend)
		{
//			StateOfLevels = 0;
//			if(PingPongSemaphore == 0)
//			{
//				uDMAChannelTransferSet( UDMA_CHANNEL_UART0TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
//										A_DMX,
//										(void *)(UART0_BASE + UART_O_DR),
//										MaxSend + 1);
//			}
//			else
//			{
//				uDMAChannelTransferSet( UDMA_CHANNEL_UART0TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
//										B_DMX,
//										(void *)(UART0_BASE + UART_O_DR),
//										MaxSend + 1);
//			}
//			
//			LastPingPongSemaphore = PingPongSemaphore;
//			uDMAChannelEnable(UDMA_CHANNEL_UART0TX);
//			UARTDMAEnable(UART0_BASE, UART_DMA_TX);
//			uDMAChannelRequest(UDMA_CHANNEL_UART0TX);
		}
	}	

	return;
}

void TIMER1A_Handler()// 1/ 40 Seconds
{
	TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);//TIMER TIME OUT
	//Set the Time 0 and drop the TX line
	//Auto Resets
	TIMER0->TAILR = 5600; // 112 uSeconds
	TIMER0->CTL |= TIMER_CTL_TAEN | TIMER_CTL_TBEN;
	
	Semaphore = 1;
}
