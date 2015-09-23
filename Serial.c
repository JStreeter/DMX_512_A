/// @file    Serial.c
/// @author  Jonathan Streeeter
/// @date    Septmber 23, 2015
/// @brief   Main code to control DMX master
/// @details 
////////////////////////////////////////////////////////////////////////////////

#include "TM4C123.h"                    // Device header
#include "Serial.h"
#include <stdbool.h>
#include "driverlib/uart.h"
#include "driverlib/gpio.h"

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
      PA0 U0_RX, PA1 U0_TX, 115200 @ 16MHz, 8 N 1
 *----------------------------------------------------------------------------*/

//  UART0->CTL  =   0;                             /* disable UART              */
//  UART0->IM   =   0;                             /* no interrupts             */


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
 void UART1_Handler()
 {
	static unsigned char Foo;
	UARTIntClear(UART1_BASE,UART_INT_RX);
	while(1)GPIOPinWrite(GPIOF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0xFF);//Write to the pins
	Foo = UART1->DR;
	UART1->DR = Foo;
 }
 
//int SER_PutChar (int c) {

//  while ((UART0->FR & (1ul << 7)) == 0x00);
//  UART0->DR = (c & 0xFF);

//  return (c);
//}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
//int SER_GetChar (void) {

//  while ((UART0->FR & (1ul << 4)) == 0x00);
//  return (UART0->DR & 0xFF);
//}
