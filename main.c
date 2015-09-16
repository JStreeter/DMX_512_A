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
#include <stdio.h>
#include <stdint.h>
#include "TM4C123.h"                    // Device header
#include "RTE_Components.h"             // Component selection
#include "Serial.h"

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
//	uint8_t ui8LED = 2;
//	SystemInit();
//	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
//	while(1)
//	{
//		// Turn on the LED
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
//		// Delay for a bit
//		SysCtlDelay(2000000);
//		// Cycle through Red, Green and Blue LEDs
//		if (ui8LED == 8) {ui8LED = 2;} else {ui8LED = ui8LED*2;}
//	}
}

// Initialize Hardware
void initHw()
{
	// Configure HW to work with 16 MHz XTAL, PLL enabled, system clock of 40 MHz
//    SYSCTL_RCC_R = SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);
//SYSCTL = RCC_XTAL_16MHZ
    // Set GPIO ports to use APB (not needed since default configuration -- for clarity)
    // Note UART on port A must use APB
//    SYSCTL_GPIOHBCTL_R = 0;

//    // Enable GPIO port A and F peripherals
//    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOF;

//    // Configure LED and pushbutton pins
//    GPIO_PORTF_DIR_R = 0x0A;  // bits 1 and 3 are outputs, other pins are inputs
//    GPIO_PORTF_DR2R_R = 0x0A; // set drive strength to 2mA (not needed since default configuration -- for clarity)
//    GPIO_PORTF_DEN_R = 0x1A;  // enable LEDs and pushbuttons
//    GPIO_PORTF_PUR_R = 0x10;  // enable internal pull-up for push button

//    // Configure UART0 pins
//	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;         // turn-on UART0, leave other uarts in same status
//    GPIO_PORTA_DEN_R |= 3;                           // default, added for clarity
//	GPIO_PORTA_AFSEL_R |= 3;                         // default, added for clarity
//    GPIO_PORTA_PCTL_R = GPIO_PCTL_PA1_U0TX | GPIO_PCTL_PA0_U0RX;

   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
//    UART0_CTL_R = 0;                                 // turn-off UART0 to allow safe programming
//	UART0_CC_R = UART_CC_CS_SYSCLK;                  // use system clock (40 MHz)
//    UART0_IBRD_R = 21;                               // r = 40 MHz / (Nx115.2kHz), set floor(r)=21, where N=16
//    UART0_FBRD_R = 45;                               // round(fract(r)*64)=45
//    UART0_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN; // configure for 8N1 w/ 16-level FIFO
//    UART0_CTL_R = UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN; // enable TX, RX, and module
}
