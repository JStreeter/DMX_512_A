/// @file    Serial.c
/// @author  Jonathan Streeeter
/// @date    Septmber 23, 2015
/// @brief   Main code to control DMX master
/// @details 
////////////////////////////////////////////////////////////////////////////////

#include "TM4C123.h"                    // Device header
#include "Serial.h"
#include <stdbool.h>
#include "inc/hw_uart.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include <stdio.h>
#include "globals.h"
#include "driverlib/udma.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include <string.h>
#include "inc/hw_timer.h"

#define MEM_BUFFER_SIZE 1024
unsigned char 			pui8DMAControlTable[1024]__attribute__((aligned (1024*8)));//THIS IS REQUIRED FOR DMA DO NOT ASK


volatile U8		UartRingBuffer[MAXRINGBUFSIZE]; //Max length of NEMA sentece is 80 and the max STX is 154
volatile U16	HIndex, TIndex;			//Internal Indexs
volatile U8 	TxReady;
uint8_t 		pui8DMAControlTable[1024];//THIS IS REQUIRED FOR DMA DO NOT ASK. DO NOT CHANGE. DO NOT MOVE

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
 /// @fn      void Uart0_ISR()
/// @author  Jonathan Streeter
/// @date    1/21/2014
/// @brief   UART interrupt
/// @details Only used for the UART ISR.
/// @return  void  ISR
////////////////////////////////////////////////////////////////////////////////
void UART5_Handler()//DMX
{
	
	return;
}


void UART1_Handler()//DMX
{
	volatile U8 	Foo;
	volatile U32 	IntterruptReg;
	volatile U32 	Tryit;
	U8 				check;
	
	IntterruptReg = UART1->RIS;
	Tryit = UART1->DR;
	check = 1;
	if(Tryit & 0x0F00) //CHECK ON THE FRAMMING ERROR!
	{
		UARTIntClear(UART1_BASE,UART_INT_BE);			//Clear Flag
		UARTIntClear(UART1_BASE,UART_RIS_FERIS);
		
		Incoming_Counter = 0;
		Tryit = 0xFFFFFFFF;
		return;
	}
	
	if (IntterruptReg & UART_RIS_RXRIS)				//Got a Byte of Data?//RX IF
	{
		UARTIntClear(UART1_BASE,UART_INT_RX);			//Clear Flag

		RedBlink = true;
		RedBlinkDeath = false;
		
		do
		{
			if(check)
			{
				Foo = (U8)Tryit;
				check = 0;
			}
			else
			{
				Foo = (U8) UART1->DR;
			}

			if(!ThePollTrigger)
			{
				if(Incoming_Counter == 0)
				{
					IncomingDMX[0] = Foo;//COMMAND
					if(IncomingDMX[0] == 0xF0)
					{
						//This triggers the break mode.
						ThePollTrigger = true;
					}
				}
				
				if(Incoming_Counter == Address + 1 )
				{
					IncomingDMX[1] 	= Foo;//DATA!!!
				}
				else if(Incoming_Counter == Address + 2)//Servo
				{
					IncomingDMX[2] 		= Foo;//DATA!!!
					RXREADY 			= true;
				}
			}
			else if(ThePollTrigger && (Incoming_Counter == Address + 1 ) )
			{
				if(Foo != 0x01)
				{
					ThePollTrigger = false;
				}
			}
			Incoming_Counter++;
			
			
			if(ThePollTrigger && Incoming_Counter == 512)
			{
				//TRIGGERBREAK 100us then
				TIMER0->TAILR 		= 4950; 	// 112 uSeconds
				TIMER0->CTL 		|= TIMER_CTL_TAEN | TIMER_CTL_TBEN;//GOGO!
				DEro 				= 1;	//The TX is on 100us to go!
				PULLDOWNER			= 0;	//BREAK!!!
			}
			
			if(Incoming_Counter >= 520)
			{
				
				Incoming_Counter 	= 0;
			}
			
		}while(!HWREG(UART1_BASE + UART_O_FR) & UART_FR_RXFE);
	}
	
	return;
}

 void UART0_Handler()//DEBUG
{
	 uint32_t status;
	if (UART0->RIS & UART_RIS_RXRIS)				//Got a Byte of Data?//RX IF
	{
		UARTIntClear(UART0_BASE,UART_INT_RX);			//Clear Flag
		RngAdd(UART0->DR & 0xFF);		//Yes this is all it does	//A
	}
    // If RX channel no longer enabled, receive finished
    if (!uDMAChannelIsEnabled(UDMA_CHANNEL_UART0RX))
    {

    }
    // Else if TX channel no longer enabled, transmit finished
    else if (! uDMAChannelIsEnabled(UDMA_CHANNEL_UART0TX))
    {
        UARTDMADisable(UART0_BASE, UART_DMA_TX);
    }

    status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, status);

	return;
}
///////////////////////////////////////////////////////// 

/* FILE is typedef’d in stdio.h. */

int fputc(int ch, FILE *f) 
{
	/* Your implementation of fputc(). */
	while (UART0->FR & UART_FR_TXFF);//<<<<<<-------------------------------------------WHILE!!!!
	UART0->DR = ch;
  return ch;
}

void DEBUGENputc(int ch) 
{
	/* Your implementation of fputc(). */
	while (UART5->FR & UART_FR_TXFF);//<<<<<<-------------------------------------------WHILE!!!!
	UART5->DR = ch;
  return;
}

int ferror(FILE *f)
{
  /* Your implementation of ferror(). */
  return 0;
}

////////////////////////////////////////////////////////

/// @fn      void RngAdd(U8 NewData)
/// @author  Jonathan Streeter
/// @date    1/20/2014
/// @brief   Adds to the Buffer and moves the Head and Tail
/// @details Adds a new data byte from the UART interrupt to the Ring Buffer and
///				moves the Head and Tail buffer. This is the ONLY function that
///				moves the Head Pointer
///
/// @param   NewData  Data to be inserted into the RngBuffer
/// @return  void
////////////////////////////////////////////////////////////////////////////////
void RngAdd(U8 NewData)
{
	UartRingBuffer[HIndex] = NewData;			//Shove data in
	HIndex = (HIndex + 1) % MAXRINGBUFSIZE;		//Advance and check overflow
	if (HIndex == TIndex)						//Check if they have looped back on eachother
		TIndex = (TIndex + 1) % MAXRINGBUFSIZE; //If so andvance the TP
	return;
}

/// @fn      S16 RngGet(U8 *LocalTailIndex)
/// @author  Jonathan Streeter
/// @date    1/21/2014
/// @brief   Adds to the Buffer and moves the Head and Tail
/// @details Get the next char from the ring buffer using the Local Tail Index that you pass it.
///				If the return is > 0xFF then the Tail is at the Head. The Tail index is moved to
///				the next char
/// @param   LocalTailIndex
/// @return  char that it pulled. EOF if the Head matches the tail
////////////////////////////////////////////////////////////////////////////////
S16 RngGet(U16 *LocalTailIndex)
{
	S16 RTV = EOF;
	//printf("\r\n------> %d <------\r\n",*LocalTailIndex);
	if (*LocalTailIndex != HIndex)				//Not Matching
	{
		RTV = UartRingBuffer[*LocalTailIndex];	//Grab data
		*LocalTailIndex = (*LocalTailIndex + 1) % MAXRINGBUFSIZE;	//increament and check for overflow
	}
	
	return (RTV);	//Will be 0xFFFF if no new char was aquired
}

/// @fn      void RngFlush(U8 *LocalTailIndex)
/// @author  Jonathan Streeter
/// @date    1/21/2014
/// @brief   Flushes Local Index
/// @details Matches the Local Tail Index to the Head Index. Effectivly Flushes buffer
///
/// @param   LocalTailIndex
/// @return  void
////////////////////////////////////////////////////////////////////////////////
void RngFlush(U16 *LocalTailIndex)
{
	*LocalTailIndex = HIndex;
	return;
}

/// @fn      U8 RngDataUsed(U8* LocalTailIndexPeek)
/// @author  Jonathan Streeter
/// @date    1/21/2014
/// @brief   Using local TailIndex return howmany bytes ahead the Head is.
/// @details Using local TailIndex return howmany bytes ahead the Head is.
///
/// @param    LocalTailIndexPeek  The index that is passed in
/// @return   Number of bytes left in Buffer
////////////////////////////////////////////////////////////////////////////////
U16 RngDataUsed(U16 *LocalTailIndexPeek)
{
	U16 DataLeft = 0;
	U16 TempHeadIndex = HIndex;

	if (TempHeadIndex < *LocalTailIndexPeek) TempHeadIndex += MAXRINGBUFSIZE;

	DataLeft = (U16) (TempHeadIndex -*LocalTailIndexPeek);

	return (DataLeft);
}

void UartWrite(U8 *DataToSend, U16 Length)
{
	U16 Counter;

	for (Counter = 0; Counter < Length; Counter++)
	{
		fputc(DataToSend[Counter],&__stdout);
	}
}

 void DMA_Setup_UART1()
 {
	//
	/*The application must allocate the channel control table. This one is a
	full table for all modes and channels.
	NOTE: This table must be 1024-byte aligned.*/
	//
	uDMAEnable();
////	//
////	// Set the base for the channel control table.
////	//
	uDMAControlBaseSet(&pui8DMAControlTable[0]);
////	//
////	// No attributes must be set for a software-based transfer. The attributes
////	// are cleared by default, but are explicitly cleared here, in case they
////	// were set elsewhere.
////	//
	uDMAChannelAttributeDisable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_ALL);
	
////	//
////	// Now set up the characteristics of the transfer for 8-bit data size, with
////	// source and destination increments in bytes, and a byte-wise buffer copy.
////	// A bus arbitration size of 8 is used.
	 uDMAChannelControlSet(  UDMA_CHANNEL_UART1TX	    |
                                UDMA_PRI_SELECT,
                                UDMA_SIZE_8             |
                                UDMA_SRC_INC_8          |
                                UDMA_DST_INC_NONE       |
                                UDMA_NEXT_USEBURST              );
		

////	// Finally, the channel must be enabled. Because this is a software-
////	// initiated transfer, a request must also be made. The request starts the
////	// transfer.
////	//
	UARTIntEnable(UART1_BASE,UART_INT_DMATX);

//        // DMA channel must be enabled first, or an interrupt will occur immediately
	uDMAChannelEnable(UDMA_CHANNEL_UART1TX);
	UARTDMAEnable(UART1_BASE, UART_DMA_TX);
}

