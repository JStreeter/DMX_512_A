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

#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
	// EXTERNALS
////////////////////////////////////////////////////////////////////////////////
FILE __stdout;
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
	
//	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
//    //UARTIntEnable();
//	
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Leds Push Buttons
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//UARTPins
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//UART0 Mod
//		// Configure HW to work with 16 MHz XTAL, PLL enabled, system clock of 40 MHz
//    SYSCTL->RCC =SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);

//    // Set GPIO ports to use APB (not needed since default configuration -- for clarity)
//    // Note UART on port A must use APB
//    SYSCTL->GPIOHBCTL = 0;

//    // Configure LED and pushbutton pins
//	//Leds First
//    GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
//	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3,GPIO_DIR_MODE_OUT);
//	
//	//Push Buttons
//	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
//	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_DIR_MODE_IN);
//	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//	GPIOPinTypeUART(GPIOB_BASE,GPIO_PIN_0 | GPIO_PIN_1);
//	//
//	// Enable UART1 functionality on GPIO Port B pins 0 and 1.
//	//
//	GPIOPinConfigure(GPIO_PB0_U1RX);
//	GPIOPinConfigure(GPIO_PB1_U1TX);

////   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
//	UARTEnable(UART1_BASE);
//	UARTClockSourceSet(UART1_BASE,UART_CLOCK_SYSTEM);
//	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);

	return;
}
// Blocking function that writes a serial character when the UART buffer is not full
//void putcUart1(char c)
//{
//	while (UART1->FR & UART_FR_TXFF);//<<<<<<-------------------------------------------WHILE!!!!
//	UART1->DR = c;
//}
// Blocking function that writes a string when the UART buffer is not full
void putsUart1(char* str)
{
	int i;
    for (i = 0; i < strlen(str); i++)
	{
	  fputc(str[i],&__stdout);
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
    uint32_t lfsr = 0xACE1u;/* Any nonzero start state will work. */
    unsigned bit;
		// Display greeting
	GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1, 0xFF);//Write to the pins
	printf("Hello World\r\n");
//    putsUart1("Serial Example\r\n");
//    putsUart1("Press '0' or '1'\r\n");
//    putcUart1('a');
//	UARTIntEnable(UART1_BASE,UART_INT_RX);
//	IntEnable(INT_UART1);
//	IntMasterEnable();
	//IntMasterEnable
	while(1)
	{
		putsUart1("Serial Example\r\n");
		// Turn on the LED
		GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, lfsr);//Write to the pins
		// Delay for a bit
		SysCtlDelay(1500000);
		/* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
		//x^32,x^22,x^2,x^1
        bit  = ((lfsr >> 0) ^ (lfsr >> 10) ^ (lfsr >> 30) ) & 1;
		
        lfsr =  (lfsr >> 1) | (bit << 15);
		printf("%ul\r\n",lfsr);
	}
}
