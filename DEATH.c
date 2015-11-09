#include <main.h>
#include <stdio.h>
#include "TM4C123GH6PM.h"                    // Device header
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "inc/hw_gpio.h"

#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/udma.h"
#include <string.h>
#include "globals.h"
#include "IO_Expander.h"

void DEATH()
{
	U8 Buffer[128];
	
	GPIOPinWrite(GPIOD_BASE, GPIO_PIN_6, 0xFF);//Write to the pins
	GPIOPinWrite(GPIOB_BASE, GPIO_PIN_1, 0);//Write to the pins
	
	printf("I am about send it\r\n");
	Buffer[0] = 0xA4;
	ExIO_8(Buffer,1);

//	Buffer[0] = 0x40;
//	Buffer[1] = 0xA1;
//	Buffer[2] = 0xC0;
//	Buffer[3] = 0xA6;
//	Buffer[4] = 0xA3;
//	Buffer[5] = 0x2F;
//	Buffer[6] = 0xF8;
//	Buffer[7] = 0x00;
//	Buffer[8] = 0x27;
//	Buffer[9] = 0x81;
//	Buffer[10] = 0x04;
//	Buffer[11] = 0xAC;
//	Buffer[12] = 0x00;


	Buffer[0] = 0xA1;
	Buffer[1] = 0xC0;
	Buffer[2] = 0xA6;
	Buffer[3] = 0xA2;
	Buffer[4] = 0x2F;
	Buffer[5] = 0xF8;
	Buffer[6] = 0x00;
	Buffer[7] = 0x27;
	Buffer[8] = 0x81;
	Buffer[9] = 0x16;
	Buffer[10] = 0xAC;
	Buffer[11] = 0x00;
	Buffer[12] = 0x00;
	ExIO_8(Buffer,13);
	
	Buffer[0] = 0xA7;
	ExIO_8(Buffer,1);
	
	printf("I sent it\r\n");
	
	Buffer[0] = 0xB0;
	Buffer[1] = 0x10;
	Buffer[2] = 0x00;
	ExIO_8(Buffer,3);
	printf("I sent it\r\n");
	GPIOPinWrite(GPIOB_BASE, GPIO_PIN_1, 0xFF);//Write to the pins
	printf("I sent it\r\n");

	Buffer[0] = 0xFF;
	Buffer[1] = 0xFF;
	Buffer[2] = 0xFF;
	Buffer[3] = 0xFF;
	Buffer[4] = 0xFF;
	Buffer[5] = 0xFF;
	Buffer[6] = 0xFF;
	Buffer[7] = 0xFF;
	Buffer[8] = 0xFF;
	Buffer[9] = 0xFF;
	ExIO_8(Buffer,10);
	
	Buffer[0] = 0xA4;
	ExIO_8(Buffer,1);
	while(1);
	return;
}
