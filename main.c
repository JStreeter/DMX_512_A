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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
//#include "TM4C123.h"                    // Device header
#include "TM4C123GH6PM.h"                    // Device header
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"
#include "inc/hw_uart.h"
//#include "pin_map.h"
//#include "inc/hw_memmap.h"
#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include <string.h>
//#include <Globals.h>
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

////////////////////////////////////////////////////////////////////////////////
void SystemInit() //THIS RUNS FIRST!!! on BOOT UP!!
{
	
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    //UARTIntEnable();
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Leds Push Buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//UARTPins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//UART0 Mod
		// Configure HW to work with 16 MHz XTAL, PLL enabled, system clock of 40 MHz
    SYSCTL->RCC =SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);

    // Set GPIO ports to use APB (not needed since default configuration -- for clarity)
    // Note UART on port A must use APB
    SYSCTL->GPIOHBCTL = 0;

    // Configure LED and pushbutton pins
	//Leds First
    GPIODirModeSet(GPIOF_BASE,GPIO_DIR_MODE_OUT, GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3);
	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	//Push Buttons
	GPIODirModeSet(GPIOF_BASE,GPIO_DIR_MODE_IN, GPIO_PIN_0 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
///////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
////	GPIODirModeSet(GPIOA_BASE,GPIO_DIR_MODE_HW, GPIO_PIN_0 | GPIO_PIN_1);
////	GPIOPadConfigSet(GPIOA_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPinConfigure(GPIO_PA0_U0RX | GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIOA_BASE,GPIO_PIN_0 | GPIO_PIN_1);

	UARTEnable(UART0_BASE);
	UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);
	
//   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
//    UART0_CTL_R = 0;                                 // turn-off UART0 to allow safe programming
//	UART0_CC_R = UART_CC_CS_SYSCLK;                  // use system clock (40 MHz)
//    UART0_IBRD_R = 21;                               // r = 40 MHz / (Nx115.2kHz), set floor(r)=21, where N=16
//    UART0_FBRD_R = 45;                               // round(fract(r)*64)=45
//    UART0_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN; // configure for 8N1 w/ 16-level FIFO
//    UART0_CTL_R = UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN; // enable TX, RX, and module

	return;
}
// Blocking function that writes a serial character when the UART buffer is not full
void putcUart0(char c)
{
	while (UART0->FR & UART_FR_TXFF);//<<<<<<-------------------------------------------WHILE!!!!
	UART0->DR = c;
}
// Blocking function that writes a string when the UART buffer is not full
void putsUart0(char* str)
{
	int i;
    for (i = 0; i < strlen(str); i++)
	{
	  putcUart0(str[i]);
	 }
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
int main(void)
{
	uint8_t ui8LED = 2;
	volatile unsigned long BLAH = SYSCTL_SYSDIV_1;
	
		// Display greeting
    putsUart0("Serial Example\r\n");
    putsUart0("Press '0' or '1'\r\n");
    putcUart0('a');
	GPIOPinTypeGPIOOutput(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	while(1)
	{
		// Turn on the LED
		
		GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
		// Delay for a bit
		SysCtlDelay(2000000);
		// Cycle through Red, Green and Blue LEDs
		if (ui8LED == 8) {ui8LED = 2;} else {ui8LED = ui8LED*2;}
		
		
	}
}
