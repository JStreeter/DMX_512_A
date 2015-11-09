//#include "TM4C123.h"                    // Device header
//#include "inc/hw_uart.h"
//#include "driverlib/uart.h"
//#include "driverlib/gpio.h"
//#include "driverlib/ssi.h"
//#include "driverlib/udma.h"
//#include "driverlib/interrupt.h"
//#include "inc/hw_ints.h"
//#include "inc/hw_ssi.h"

#include <stdbool.h>
#include <stdio.h>
#include "globals.h"
#include "Paser.h"
#include <string.h>
#include <ctype.h>

#define MaxLocal	80


void getCommand(U8 Pick[] ,U8 *);

void parseCommand(U8 ch)
{
	static U8 Lbuff[MaxLocal]={0};

	switch(ch)
	{
		case(0x08)://Delete
			Lbuff[--LocalHead] = 0;
		break;
		case(0x0A)://New line DO IT
		case(0x0D):
			getCommand(Lbuff,&LocalHead);
			printf("\r\n");
		break;
		default:
			if(LocalHead < MaxLocal-1)
			{
				Lbuff[LocalHead] = ch;
				LocalHead++;
				Lbuff[LocalHead] = 0;
			}
		break;
	}
	
	
	printf("%c[2K",0x1B);
	if(LocalHead != 0)
	{
		printf("%s\r",Lbuff);
	}
	return;
}
/*
“clear” ----> Wipe all 512 addressed data(512)
“set A, V”7 -> is sent, where A is the DMX address and V is the value, the controller should update the
				value being transmitted to address A to a value of V.
“get A” ----> Return the vaule at that address
“on” -------> FULL 512 SENT AS FAST AS POSSIBLE
"off" ------> All STOP
"max M" ----> Send up to Address M
"poll" -----> Poll for Other devices

After each command is received from the PC, the controller shall respond with “Ready” or “Error”. 
*/
static const char HELLO3[][20] = 
{
	"clear",
	"set",
	"get",
	"on",
	"off",
	"max M",
	"poll"
};
void getCommand(U8 Pick[] ,U8 *maxSize)
{
	U8 i;
	char* Value;
	printf("\r\n^-----\r\n");

	i = 0;
	do
	{
		Value = strstr((char*)Pick,HELLO3[i]);
	}while(Value == 0 && i++<7);
	
	if(Value != 0)
	{
		printf("%s ||| %s\r\n",Value,Pick);
		switch(i)
		{
			case(0)://CLEAR
				
				break;
			case(1)://SET
			
				break;
			case(2)://GET
			
				break;
			case(3)://ON
			
				break;
			case(4)://OFF
			
				break;
			case(5)://MAX
			
				break;
			case(6)://POLL
			
				break;
				
			default://HACF
				
				break;
		}
	}
	else
	{
		printf("ERROR\r\n");
	}
	*maxSize = 0;
	return;
}
