#include <main.h>
#include <stdint.h>
#include <stdio.h>
#include <globals.h>


U32 BBFlags;

U8	A_DMX[513];//THIS IS REQUIRED FOR DMX DO NOT ASK
U8	B_DMX[513];//THIS IS REQUIRED FOR DMX DO NOT ASK

U8 LocalHead;
const char * StringCheck[] = {
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
	
	A_DMX[0] = '0';
	B_DMX[0] = '0';
	PingPongSemaphore = 0;
	for(Setup=1;Setup<511;Setup++)
	{
		A_DMX[Setup] = 'A';
		B_DMX[Setup] = 'B';
	}
	A_DMX[511] = '\r';
	B_DMX[511] = '\r';
	
	A_DMX[512] = '\n';
	B_DMX[512] = '\n';
	
	MaxSend = 512;
	LocalHead = 0;
	
	return;
}
