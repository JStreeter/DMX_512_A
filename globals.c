#include <main.h>
#include <stdint.h>
#include <stdio.h>
#include <globals.h>


U32 BBFlags;

U8	A_DMX[513];//THIS IS REQUIRED FOR DMX DO NOT ASK
U8	B_DMX[513];//THIS IS REQUIRED FOR DMX DO NOT ASK

U8 	ShadowDMX[513];//The thing we play with before we transmitt

U8 	IncomingDMX[2];
U16 	Address;
U8 LocalHead;

U16 Incoming_Counter;


const char * StringCheck[] = 
{
    "clear\0",
    "set\0",
    "get\0",
	"on\0",
	"off\0",
	"max\0",
	"poll\0"	
};

U16 MaxSend;

void IntGlobals()
{
	U16 Setup;
	Incoming_Counter = 0;
	A_DMX[0] = '0';
	B_DMX[0] = '0';
	PingPongSemaphore = 0;
	ShadowDMX[0] = 0;
	for(Setup=1;Setup<511;Setup++)
	{
		A_DMX[Setup] = 'A';
		B_DMX[Setup] = 'B';
		ShadowDMX[Setup] = 0;
	}
	A_DMX[511] = '\r';
	B_DMX[511] = '\r';
	ShadowDMX[511] = 0;
	
	A_DMX[512] = '\n';
	B_DMX[512] = '\n';
	ShadowDMX[512] = 0;
	
	MaxSend = 0;
	LocalHead = 0;
	
	return;
}
