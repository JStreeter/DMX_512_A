#include <main.h>
#include <stdint.h>
#include <stdio.h>
#include <globals.h>


U32 BBFlags;

U8	PingDMX[513];//THIS IS REQUIRED FOR DMX DO NOT ASK
U8	PongDMX[513];//THIS IS REQUIRED FOR DMX DO NOT ASK

void IntGlobals()
{
	U16 Setup;
	
	PingDMX[0] = '0';
	PongDMX[0] = '0';
	PingPongSemaphore = 0;
	for(Setup=1;Setup<511;Setup++)
	{
		PingDMX[Setup] = 'A';
		PongDMX[Setup] = 'B';
	}
	PingDMX[511] = '\r';
	PongDMX[511] = '\r';
	
	PingDMX[512] = '\n';
	PongDMX[512] = '\n';
	return;
}
