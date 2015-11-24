/*----------------------------------------------------------------------------
 * Name:    Serial.h
 * Purpose: Low level serial definitions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef SERIAL_H
#define SERIAL_H
#include "globals.h"
struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
  //Does not look like I need more
};

#define MAXRINGBUFSIZE	1024

extern volatile U8	UartRingBuffer[MAXRINGBUFSIZE]; //Max length of NEMA sentece is 80 and the max STX is 154
extern volatile U16	HIndex, TIndex;			//Internal Indexs
extern volatile U8 	TxReady;

extern void RngAdd(U8 NewData);//Adds to a Global Buffer
extern S16 	RngGet(U16 *LocalTailIndex);//Pulls from the buffer
extern void RngFlush(U16 *LocalTailIndex);//Flush the local Buffer
extern U16 	RngDataUsed(U16 *LocalTailIndexPeek);//DEBUG
extern void UartWrite(U8 *DataToSend, U16 Length);//Blocking write of data pack. Data to send then Length. Raw Data
extern void DEBUGENputc(int ch);

void DMA_Setup_UART1(void);
#endif
