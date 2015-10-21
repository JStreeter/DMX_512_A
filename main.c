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
#include "inc/hw_ssi.h"
#include "inc/hw_uart.h"

#include "driverlib/interrupt.h"
#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/ssi.h"
#include "driverlib/udma.h"
#include <string.h>

#include "IO_Expander.h"
////////////////////////////////////////////////////////////////////////////////
	// EXTERNALS
////////////////////////////////////////////////////////////////////////////////
volatile U8 Semaphore = 0;
FILE __stdout;
////////////////////////////////////////////////////////////////////////////////
	// GLOBALS
////////////////////////////////////////////////////////////////////////////////
//C4-C7
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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//UART1Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//UART1Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//Leds Push Buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Leds Push Buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//A0,A1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//B0,B1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOF);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//Timer 0

    // Set GPIO ports to use APB (not needed since default configuration -- for clarity)
    // Note UART on port A must use APB
    SYSCTL->GPIOHBCTL = 0;

    // Configure LED and pushbutton pins
	GPIOPadConfigSet(GPIOE_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOE_BASE,GPIO_PIN_3,GPIO_DIR_MODE_OUT);
	
	//CCS
	GPIOPadConfigSet(GPIOD_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOD_BASE,GPIO_PIN_1,GPIO_DIR_MODE_OUT);
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
	GPIOPinTypeUART(GPIOC_BASE,GPIO_PIN_5 | GPIO_PIN_4);
	//
	// Enable UART1 functionality on GPIO Port B pins 0 and 1.
	//
	
   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
	UARTEnable(UART1_BASE);
	UARTClockSourceSet(UART1_BASE,UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), DMXBAUD, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_TWO);
	UARTFIFODisable(UART1_BASE);
	
	GPIOPinConfigure(GPIO_PC5_U1TX);
	GPIOPinConfigure(GPIO_PC4_U1RX);

//END/////////////DMX-512-A//////////////////////////////////////////////////////////////////////////////////

//TIMER//
	TimerConfigure(TIMER0_BASE,TIMER_CFG_ONE_SHOT);
	TimerClockSourceSet(TIMER0_BASE,TIMER_CLOCK_SYSTEM);
	TimerLoadSet(TIMER0_BASE,TIMER_A,TimeDebug1);//API says to use the Timer A if full width
//	IntRegister(INT_TIMER0A, UART0Handler);
//	IntRegister(INT_UART0, UART0Handler);
//	TimerIntRegister(TIMER0_BASE,TIMER_BOTH,TIMER0A_Handler);
//END TIMER//
	IntPrioritySet(INT_UART0, 0x05);
	IntPrioritySet(INT_SSI3, 0x10);
	SpiSetup();
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
    U32 lfsr = 0xAAAAu;/* Any nonzero start state will work. */
    U32	Time;
	U16	In,Old;
	volatile S16 TempCh;
	U16 RxBufpt;
	volatile U32 BaseTime = TimeDebug1;
	unsigned bit;
	U8 Rand;
	U8 BLAH[256];
	U8 Short[16],RunOnce;
	U16 x;
	
	RunOnce = 0;
		// Display greeting
//	
	UARTIntEnable(UART0_BASE,UART_INT_RX);
//	UARTIntEnable(UART1_BASE,UART_INT_DMATX);

	SSIIntEnable(SSI3_BASE,SSI_RXFF);
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);	

	IntEnable(INT_UART0);
	IntEnable(INT_TIMER0A);
	
	IntMasterEnable();
	
	Time = SysCtlClockGet();
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
	
	//printf("The tick counter then is %f\r\n",1.0);
	RngFlush(&RxBufpt);
	
	TimerLoadSet(TIMER0_BASE,TIMER_A,TimeDebug1<<3);//API says to use the Timer A if full width
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
	Semaphore = 0;
	
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_3, 0xFF);//Write to the pins
	while(Semaphore == 0);
	TimerLoadSet(TIMER0_BASE,TIMER_A,TimeDebug1);//API says to use the Timer A if full width
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_3, 0x00);//Write to the pins

	printf("\r\nHello World\r\n");	
	printf("The Clock is set to %d\r\n",Time);
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_2, 0xFF);//Write to the pins

	printf("In = %03X \r\n",ReadAddessEXIO());
	
	while(GPIOPinRead(GPIOF_BASE, GPIO_PIN_4));//Wait of user
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_2, 0x00);//Write to the pins
	Old = 0;
	DMA_Setup_UART1();
//	for(x=0;x<256;x++)
//	{
//			BLAH[x] = 'b';
//	}
	Rand = sprintf((char*)&BLAH[0],"This is the exact message that I am printing.\r\n\0");
	printf("%s",BLAH);
	while(1)
	{	
		
//		if(UART1->RIS & UART_RIS_TXRIS)
//		{
//			 UART1->DR  = Rand++;
//		}
		
		In =  ReadAddessEXIO();
		if(Old != In)
		{
			printf("In = %03X \r\n",In);
			Old = In;
		}
		
		if(GPIOPinRead(GPIOF_BASE, GPIO_PIN_4) && RunOnce == 0)
		{
			RunOnce = 1;
			uDMAChannelTransferSet( UDMA_CHANNEL_UART0TX | UDMA_PRI_SELECT,
										UDMA_MODE_BASIC,
										BLAH,
										(void *)(UART0_BASE + UART_O_DR),
										strlen((char*)&BLAH[0]));
			uDMAChannelEnable(UDMA_CHANNEL_UART0TX);
			UARTDMAEnable(UART0_BASE, UART_DMA_TX);
			uDMAChannelRequest(UDMA_CHANNEL_UART0TX);
		}
		
//		if()
//		{
//		
//		}
		
		WriteOutIOEX(lfsr | In);
		
		if(	Semaphore != 0)
		{	
			TimerEnable(TIMER0_BASE, TIMER_BOTH);
			Semaphore = 0;
			bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
			lfsr =  (lfsr >> 1) | (bit << 15);
		}
//		
		TempCh = RngGet(&RxBufpt);
		if(TempCh != EOF)
		{
			printf("%c",(char)TempCh);
		}
	}
	//Should never end up here
}
