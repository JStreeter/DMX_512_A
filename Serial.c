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
#include <string.h>


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
 void UART1_Handler()//DMX
{
	volatile U8 Foo;
	if (UART1->RIS & UART_RIS_RXRIS)				//Got a Byte of Data?//RX IF
	{
		
		UARTIntClear(UART1_BASE,UART_INT_RX);			//Clear Flag
		//RngAdd(UART1->DR);		//Yes this is all it does	//A
		Foo = UART1->DR;
		//Set A gloabal flag that bits have been inserted.
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

/* FILE is typedef�d in stdio.h. */

int fputc(int ch, FILE *f) 
{
	/* Your implementation of fputc(). */
	while (UART0->FR & UART_FR_TXFF);//<<<<<<-------------------------------------------WHILE!!!!
	UART0->DR = ch;
  return ch;
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
	//uDMAChannelAttributeDisable(UDMA_CHANNEL_SW, UDMA_ATTR_ALL);
	uDMAChannelAttributeDisable(UDMA_CHANNEL_UART0TX, UDMA_ATTR_ALL);
	
////	//
////	// Now set up the characteristics of the transfer for 8-bit data size, with
////	// source and destination increments in bytes, and a byte-wise buffer copy.
////	// A bus arbitration size of 8 is used.
////	//
////	uDMAChannelControlSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT,
////		UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_8 | UDMA_ARB_8);
	 uDMAChannelControlSet(  UDMA_CHANNEL_UART0TX    |
                                UDMA_PRI_SELECT,
                                UDMA_SIZE_8             |
                                UDMA_SRC_INC_8          |
                                UDMA_DST_INC_NONE       |
                                UDMA_NEXT_USEBURST              );
		

////	// Finally, the channel must be enabled. Because this is a software-
////	// initiated transfer, a request must also be made. The request starts the
////	// transfer.
////	//
	UARTIntEnable(UART0_BASE,UART_INT_DMATX);

//        // DMA channel must be enabled first, or an interrupt will occur immediately
	uDMAChannelEnable(UDMA_CHANNEL_UART0TX);
	UARTDMAEnable(UART0_BASE, UART_DMA_TX);
}

//U8 GetTokens(unsigned char *sentence, U8 **tok)
//{
//	U8	RetVal,
//		idx,
//		len;

//	len = strlen((char*)sentence);
//	RetVal = 0;
//	if (len > 2)
//	{
//		tok[0] = sentence + 2;
//		RetVal = 1;
//		for (idx = 2; idx < len; idx++)
//		{
//			if (sentence[idx] == ',' || sentence[idx] == '*')	// If we find a delimiter, end this token string
//			{						
//																// and point to the beginning of the next one,
//																// even if this points to another delimiter or the NULL terminator!!
//				tok[RetVal++] = &sentence[idx + 1];
//				sentence[idx] = 0;	// This delimiter is now NULL terminator for the previous token.
//			}

//			// in the unlikely case that there is no data between the final delimiter
//			// and the NULL terminator, the final entry will point to the NULL terminator
//			// thus yielding a legal, empty string.
//		}
//	}

//	tok[RetVal] = NULL;				// Mark the end.
//	return RetVal;
//}

void InitSWTransfer(void)
{
//	unsigned int uIdx;
//	
//	for(uIdx = 0; uIdx < 1024; uIdx++)
//	{
//		g_ulSrcBuf[uIdx] = uIdx;
//	}
//	
//	uDMAChannelAttributeDisable(UDMA_CHANNEL_UART0TX, UDMA_ATTR_ALL);
//	uDMAChannelControlSet(  UDMA_CHANNEL_UART0TX    |
//                                UDMA_PRI_SELECT,
//                                UDMA_SIZE_8             |
//                                UDMA_SRC_INC_8          |
//                                UDMA_DST_INC_NONE       |
//                                UDMA_ARB_1              );
//		
//	UARTIntEnable(UART0_BASE,UART_INT_DMATX);
//	uDMAChannelEnable(UDMA_CHANNEL_SSI1RX);
//	SSIDMAEnable(SSI3_BASE, UART_DMA_RX);
//	
//	uDMAChannelEnable(UDMA_CHANNEL_SSI1RX);
//	SSIDMAEnable(SSI3_BASE, UART_DMA_RX);
//	
//	IntEnable(INT_UDMA);
//	uDMAChannelAttributeDisable(UDMA_CHANNEL_SW, UDMA_ATTR_ALL);
//	uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_32 | UDMA_SRC_INC_32 | UDMA_DST_INC_32 | UDMA_ARB_8);
//	uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, g_ulSrcBuf, g_ulDstBuf, MEM_BUFFER_SIZE);
//	uDMAChannelEnable(UDMA_CHANNEL_SW);
//	uDMAChannelRequest(UDMA_CHANNEL_SW);
}
