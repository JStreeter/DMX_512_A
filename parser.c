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
#include "Paser.h"
#include "globals.h"
#include <string.h>
#include <ctype.h>
#include "TM4C123GH6PM.h"                    // Device header


#define MaxLocal	80


void getCommand(U8 Pick[] ,U8 *);
U16 ParseInput(U8 Raw[], U16* Derp);

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
	U8 i, *Copy;
	U16 j,Input,Herp;
	static U16 SaveM;
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

		if(PingPongSemaphore)
		{
			Copy = B_DMX;
		}
		else
		{
			Copy = A_DMX;
		}

		switch(i)
		{
			case(0)://CLEAR
				if(PingPongSemaphore)
				{
					Copy = B_DMX;
				}
				else
				{
					Copy = A_DMX;
				}
				
				for(j=0;j<512;j++)Copy[j]=0;
				PingPongSemaphore ^= 1;
				break;
			case(1)://SET
				j = ParseInput(Pick, &Herp);
				Input = ParseInput(Pick + Herp, &Herp);
				Copy[j] = Input;
				PingPongSemaphore ^= 1;
				break;
			case(2)://GET
				j = ParseInput(Pick, &Herp);
				printf("Address == %d\r\n",Copy[j]);
				break;
			case(3)://ON
				MaxSend = SaveM;
				break;
			case(4)://OFF
				SaveM = MaxSend;
				break;
			case(5)://MAX
				Input = ParseInput(Pick + Herp, &Herp);
				if(Input > 512)
					Input = 512;
				MaxSend = Input;
				break;
			case(6)://POLL
				printf("EMERGANCY!!! FIRE!!! CLOSE DOWN SYSTEM AND RUN!!!\r\n");
				break;
				
			default://HACF
				printf("FUCK IF I KNOW\r\n");
				break;
		}
		printf("Ready\r\n");
	}
	else
	{
		printf("ERROR\r\n");
	}
	*maxSize = 0;
	return;
}

U16 ParseInput(U8 Raw[], U16* Derp)
{
	U16 Counter = 0;
	U16 Storage = 0;
	U8 i;
	
	printf("Parser\r\n");
	
	do
	{
		i = 0;
		if( Raw[Counter] != '\0' && Raw[Counter] !=  ',' && !isalpha(Raw[Counter]) )
		{
			Storage = Storage * ( 10 * Counter );
			Storage += Raw[Counter];
			i = 1;
		}
	}while(i == 1);
	*Derp = Counter;
	
	return(Storage);
}
