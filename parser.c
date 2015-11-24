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
U8 ParseInput(U8 Raw[],U16 *Rv, U8 Types);

void parseCommand(U8 ch)
{
	static U8 Lbuff[MaxLocal]={0};

	switch(ch)
	{
		case(0x08)://Delete
			if(LocalHead != 0)
			{
				Lbuff[--LocalHead] = 0;
			}
			else
			{
				Lbuff[LocalHead] = 0;
				printf("\r \r");
			}
		break;
		case(0x0A):// \n For someone special
		case(0x0D):// \r
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

"show" -----> Show all the dat in all the addresss
After each command is received from the PC, the controller shall respond with “Ready” or “Error”. 
*/
static const char HELLO3[][20] = 
{
	"clear",
	"set",
	"get",
	"on",
	"off",
	"max",
	"poll",
	"show",
	"master",
	"slave"
};
void getCommand(U8 Pick[] ,U8 *maxSize)
{
	U8 i;
	U16 j,Input,Par[4];
	static U16 SaveM = 0,GoGOFlag = 0;
	
	char* Value;
//	printf("\r\n^-----\r\n");

	i = 0;
	do
	{
		Value = strstr((char*)Pick,HELLO3[i]);
	}while(Value == 0 && i++<7);
	
	if(Value != 0)
	{
		switch(i)
		{
			case(0)://CLEAR
				printf("Clear\r\n");
				for(j=0;j<514;j++)
				{
					ShadowDMX[j]=0;
				}
				if(LastPingPongSemaphore)
				{
					memcpy(B_DMX,ShadowDMX,513);
				}
				else
				{
					memcpy(A_DMX,ShadowDMX,513);
				}
				LastPingPongSemaphore ^= 1;
				break;
			case(1)://SET
				Par[0] = 0;
				Par[1] = 0;
				Input = ParseInput(Pick+3,Par, 2);
				if(Input > 1)
				{
					printf("\r\n");
					if(Par[0] >512)
						Par[0] = 512;
					if(Par[1] >512)
						Par[1] = 512;
						
					printf("Address %d == Data %d\r\n",Par[0],(U8)Par[1]);
					ShadowDMX[Par[0]+1] = (U8)Par[1];
					
					if(PingPongSemaphore) //If it is set it is On B
					{
						printf("\r\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n");						
						memcpy(A_DMX,ShadowDMX,513);
						printf("\r\n          %02X\r\n",B_DMX[0]);
						for(j=1;j<513;j++)
						{
							printf("%3d ",A_DMX[j]);
							if((j) %16 == 0)
								printf("\r\n");
						}	
					}
					else
					{
						printf("\r\nBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\r\n");

						memcpy(B_DMX,ShadowDMX,513);
						printf("\r\n          %02X\r\n",B_DMX[0]);
						for(j=1;j<513;j++)
						{
							printf("%3d ",B_DMX[j]);
							if((j) %16 == 0)
								printf("\r\n");
						}
					}
					PingPongSemaphore ^= 1;
						
				}
				else
				{
					printf("\r\nType ERROR\r\n");
				}
				//////////////////////////////////////////////ShadowDMXz
				break;
			case(2)://GET
				Par[0] = 0;
				Par[1] = 0;
				Input = ParseInput(Pick+3,Par, 1);
				if(Input != 0)
				{
					if(Par[0] >511)
						Par[0] = 511;
					printf("\r\n");
					printf("Address %d == %d\r\n",Par[0],ShadowDMX[Par[0]+1]);
				}
				else
				{
					printf("Type ERROR\r\n");
				}
				break;
			case(3)://ON
				MaxSend = SaveM;
				GoGOFlag = 1;
				printf("ON sending %d address\r\n", MaxSend);
				break;
			case(4)://OFF
				MaxSend = 0;
				GoGOFlag = 0;
				printf("Off\r\n");
				break;
			case(5)://MAX
				Par[0] = 0;
				Par[1] = 0;
				ParseInput(Pick+3,Par, 1);
				if(Par[0] > 512)
					Par[0] = 512;
				printf("Max Address Sent %d\r\n",Par[0]);
				//MaxSend = Par[0];
				SaveM = Par[0];
				if(GoGOFlag)
				{
					MaxSend = SaveM;
				}
				break;
			case(6)://POLL
				printf("EMERGANCY!!! FIRE!!! CLOSE DOWN SYSTEM AND RUN!!!\r\n");
				break;
			case(7)://Show
				for(j=1;j<513;j++)
				{
					printf("%3d ",ShadowDMX[j]);
					if((j) %16 == 0)
						printf("\r\n");
				}
				
				if(PingPongSemaphore == 0)
					printf("Current Buffer A\r\n");
				else
					printf("Current Buffer B\r\n");
				for(j=1;j<513;j++)
				{
					
					if(PingPongSemaphore == 0)
					{	
						printf("%3d ",A_DMX[j]);
					}
					else
					{
						printf("%3d ",B_DMX[j]);
					}
					if((j) %16 == 0)
						printf("\r\n");
				}
				printf("\r\n");
				break;
			case(8)://master
				
				break;
			case(9)://slave
			
				break;
			default://HACF
				printf("Halt and Catch Fire\r\n");
				break;
		}
		printf("Ready");
	}
	else
	{
		printf("ERROR\r\n");
	}	
	
	*maxSize = 0;
	
	return;
}

U8 ParseInput(U8 Raw[],U16 *Rv, U8 Types)
{
	U16 Counter = 0;
	U8 i,j,k;
	
	j = 0;//Return vaule
	k = 0;//Decaeded counter
	do
	{
		i = 0;
		while(Raw[Counter] == ' ' || Raw[Counter] ==  '\t')
			Counter++;
		if( Raw[Counter] != '\0' && Raw[Counter] !=  ',' && Raw[Counter] >= '0' && Raw[Counter] <= '9')// && !isalpha(Raw[Counter]) )
		{
			if(j == 0)
			{
				j++;
			}
			if(k>2)
				return(0);
//			printf("j %d k %d B %d ",j,k,Rv[j-1]);
			Rv[j-1] = Rv[j-1] * 10;
//			printf("D %d ",Rv[j-1]);
			Rv[j-1] += Raw[Counter] - '0';
//			printf("A %d\r\n",Rv[j-1]);

			Counter++;
			k++;
			i = 1;
		}
		
		if(Raw[Counter] ==  ',')
		{
			Counter++;
			i = 1;
			j++;//Next type
			k=0;
			Rv[j] = 0;
		}
	}while(i == 1 && j <= Types);

//	printf("%d %d\r\n",Rv[0],Rv[1]);//DEBUG
	return(j);
}
