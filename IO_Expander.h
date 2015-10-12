#ifndef H_IO_EXPANDER
#define H_IO_EXPANDER 1
#include "globals.h"

#define ExIO(x,y)	Spi_Blk(x,x,y,0)

#define IO_Ex_Write 0x4000
#define IO_Ex_Read 	0x4100

/////////////////////// BANK == 1 /////////////////////////ALL PortA then All Port B
#define IO_Ex_1_IODIRA 		0x0000
#define IO_Ex_1_IPOLA 		0x0001
#define IO_Ex_1_GPINTENA 	0x0002
#define IO_Ex_1_DEFVALA 	0x0003
#define IO_Ex_1_INTCONA 	0x0004
#define IO_Ex_1_IOCON 		0x0005
#define IO_Ex_1_GPPUA 		0x0006
#define IO_Ex_1_INTFA 		0x0007
#define IO_Ex_1_INTCAPA 	0x0008
#define IO_Ex_1_GPIOA 		0x0009
#define IO_Ex_1_OLATA 		0x000A
#define IO_Ex_1_IODIRB 		0x0010
#define IO_Ex_1_IPOLB 		0x0011
#define IO_Ex_1_GPINTENB 	0x0012
#define IO_Ex_1_DEFVALB 	0x0013
#define IO_Ex_1_INTCONB 	0x0014
#define IO_Ex_1_IOCON_b 	0x0015
#define IO_Ex_1_GPPUB 		0x0016
#define IO_Ex_1_INTFB 		0x0017
#define IO_Ex_1_INTCAPB 	0x0018
#define IO_Ex_1_GPIOB 		0x0019
#define IO_Ex_1_OLATB 		0x001A

/////////////////////// BANK == 0 /////////////////////////PortA_PortB, PORTA_PORTB

#define IO_Ex_0_IODIRA 		0x0000
#define IO_Ex_0_IODIRB 		0x0001
#define IO_Ex_0_IPOLA 		0x0002
#define IO_Ex_0_IPOLB 		0x0003
#define IO_Ex_0_GPINTENA 	0x0004
#define IO_Ex_0_GPINTENB 	0x0005
#define IO_Ex_0_DEFVALA 	0x0006
#define IO_Ex_0_DEFVALB 	0x0007
#define IO_Ex_0_INTCONA 	0x0008
#define IO_Ex_0_INTCONB 	0x0009
#define IO_Ex_0_IOCON 		0x000A
#define IO_Ex_0_IOCON_b 	0x000B
#define IO_Ex_0_GPPUA 		0x000C
#define IO_Ex_0_GPPUB 		0x000D
#define IO_Ex_0_INTFA 		0x000E
#define IO_Ex_0_INTFB 		0x000F
#define IO_Ex_0_INTCAPA 	0x0010
#define IO_Ex_0_INTCAPB 	0x0011
#define IO_Ex_0_GPIOA 		0x0012
#define IO_Ex_0_GPIOB 		0x0013
#define IO_Ex_0_OLATA 		0x0014
#define IO_Ex_0_OLATB 		0x0015

extern void Spi_Blk(U16 *BuffOut,U16 *BuffIn,U16 Length, U8 Device);
extern void SpiSetup(void);
extern void DMA_Setup(void);
extern U16 ReadAddessEXIO(void);
extern void WriteOutIOEX(U16 output);

#endif
