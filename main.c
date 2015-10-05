/// @file    main.c
/// @author  Jonathan Streeeter
/// @date    Septmber 14, 2015
/// @brief   Main code to control DMX master
/// @details 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	// INCLUDES
////////////////////////////////////////////////////////////////////////////////
#include <main.h>
#include <stdint.h>
#include <stdio.h>
#include "TM4C123GH6PM.h"                    // Device header
#include "Serial.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"

#include "driverlib/interrupt.h"
#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
	// EXTERNALS
////////////////////////////////////////////////////////////////////////////////
volatile U8 Semaphore = 0;
FILE __stdout;
////////////////////////////////////////////////////////////////////////////////
	// GLOBALS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	//  CONSTANTS
////////////////////////////////////////////////////////////////////////////////
#define PULSETEST      (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
 
////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void SystemInit() //THIS RUNS FIRST!!! on BOOT UP!!
{
	//Main set
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	
	//Perhial Clock enable
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//UART0Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//UART1Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//Leds Push Buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Leds Push Buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//A0,A1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//B0,B1
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//Timer 0

    // Set GPIO ports to use APB (not needed since default configuration -- for clarity)
    // Note UART on port A must use APB
    SYSCTL->GPIOHBCTL = 0;

    // Configure LED and pushbutton pins
	GPIOPadConfigSet(GPIOE_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOE_BASE,GPIO_PIN_3,GPIO_DIR_MODE_OUT);
	
	//Leds First
    GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3,GPIO_DIR_MODE_OUT);
	
	//Push Buttons
	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_DIR_MODE_IN);

///////////////Debug/////////////////////////////////////////////////////////////////////////////////////	
	GPIOPinTypeUART(GPIOA_BASE,GPIO_PIN_0 | GPIO_PIN_1);
	//
	// Enable UART1 functionality on GPIO Port B pins 0 and 1.
	//
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinConfigure(GPIO_PA0_U0RX);

   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
	UARTEnable(UART0_BASE);
	UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), DebugBaud, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);
	UARTFIFODisable(UART0_BASE);
	//UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8,UART_FIFO_RX1_8);
//END/////////////Debug//////////////////////////////////////////////////////////////////////////////////
	
///////////////DMX-512-A/////////////////////////////////////////////////////////////////////////////////////	
	GPIOPinTypeUART(GPIOB_BASE,GPIO_PIN_0 | GPIO_PIN_1);
	//
	// Enable UART1 functionality on GPIO Port B pins 0 and 1.
	//
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);

   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
	UARTEnable(UART1_BASE);
	UARTClockSourceSet(UART1_BASE,UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), DMXBAUD, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_TWO);
//END/////////////DMX-512-A//////////////////////////////////////////////////////////////////////////////////

//TIMER//
	TimerConfigure(TIMER0_BASE,TIMER_CFG_ONE_SHOT);
	TimerClockSourceSet(TIMER0_BASE,TIMER_CLOCK_SYSTEM);
	TimerLoadSet(TIMER0_BASE,TIMER_A,TimeDebug1);//API says to use the Timer A if full width
	//IntRegister(INT_TIMER0A, UART0Handler);
//	IntRegister(INT_UART0, UART0Handler);
//	TimerIntRegister(TIMER0_BASE,TIMER_BOTH,TIMER0A_Handler);
//END TIMER//
	HIndex = 0;
	TIndex = 0;	
	
	//HWREGBITW(GPIOE_BASE+GPIO_O_DATA,3) = 1;
	return;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//       @@@@      @@@@      @@@@          @@@@@@@@      @@@@      @@@@       //
//       @@@@@@  @@@@@@    @@@@@@@@          @@@@        @@@@@@    @@@@       //
//       @@@@@@@@@@@@@@  @@@@    @@@@        @@@@        @@@@@@@@  @@@@       //
//       @@@@@@@@@@@@@@  @@@@    @@@@        @@@@        @@@@  @@@@@@@@       //
//       @@@@  @@  @@@@  @@@@@@@@@@@@        @@@@        @@@@    @@@@@@       //
//       @@@@      @@@@  @@@@    @@@@        @@@@        @@@@      @@@@       //
//       @@@@      @@@@  @@@@    @@@@      @@@@@@@@      @@@@      @@@@       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// 100uS low 14uS High  -------BREAK___________________MAB--PACKET0+Data1+Data2
//Minimum Packet is 24 data packets(Add padding)
////////////////////////////////////////////////////////////////////////////////
#define GREEN_LED    (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 3*4)))
int main(void)
{
    U32 lfsr = 0xACE1u;/* Any nonzero start state will work. */
    U32	Time;
	S16 TempCh;
	U16 RxBufpt;
	unsigned bit;
		// Display greeting
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1, 0xFF);//Write to the pins
	printf("\r\nHello World\r\n");	
	Semaphore = 0;
	UARTIntEnable(UART0_BASE,UART_INT_RX);
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	
	IntEnable(INT_UART0);
	IntEnable(INT_TIMER0A);
	
	IntMasterEnable();

	Time = SysCtlClockGet();
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
	printf("The Clock is set to %d\r\n",Time);
	//printf("The tick counter then is %f\r\n",1.0);
	RngFlush(&RxBufpt);
	while(1)
	{
		// Turn on the LED
	
		if(	Semaphore != 0)
		{	
			TimerEnable(TIMER0_BASE, TIMER_BOTH);
			Semaphore = 0;

			GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, lfsr);//Write to the pins
//			0x40025000 + 0x00000000 , 1*4
//			 (*((volatile uint32_t *)((((uint32_t)(0x40025000) & 0xF0000000) | 0x02000000 | (((uint32_t)(0x40025000) & 0x000FFFFF) << 5) | ((b) << 2))))) =1 ;
//#define GREEN_LED    (*((volatile uint32_t *)(0x42 0000 00 + (0x400253FC-0x40000000)*32 + 3*4)))
//			4273 A012
//			424A 0018
			HWREGBITW(GPIOF_BASE + GPIO_O_DATA + 0x3FC,1) = lfsr & 1;
			HWREGBITW(GPIOF_BASE + GPIO_O_DATA + 0x3FC,2) = (lfsr>>1) & 1;;
			HWREGBITW(GPIOF_BASE + GPIO_O_DATA + 0x3FC,4) = (lfsr>>2) & 1;
			HWREGBITW(GPIOE_BASE + GPIO_O_DATA + 0x3FC,8) = (lfsr>>3) & 1;

//			GPIOE->DATA	^= 0xFF;
			if(Time)
			{
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA,0) = 1;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA,1) = 1;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA,2) = 1;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA,4) = 1;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA,8) = 1;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA,16) = 1;
			}
			else
			{
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA ,0) = 0;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA ,1) = 0;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA ,2) = 0;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA ,4) = 0;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA ,8) = 0;
				HWREGBITW(GPIOE_BASE + GPIO_O_DATA ,16) = 0;
			}
			Time ^= 0xFF;
//			GREEN_LED = 1;
			/* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
			//x^32,x^22,x^2,x^1
			bit  = ((lfsr >> 0) ^ (lfsr >> 10) ^ (lfsr >> 30) ) & 1;
			
			lfsr =  (lfsr >> 1) | (bit << 15);
		}
		
		TempCh = RngGet(&RxBufpt);
	
		if(TempCh != EOF)
		{
			if((U8)TempCh == '\r')
			{
				printf("\r\n");
			}
			else
			{
				printf("%c",(U8)TempCh);
			}
		}
	}
	//Should never end up here
}
