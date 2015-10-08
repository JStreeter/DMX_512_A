#ifndef H_IO_EXPANDER
#define H_IO_EXPANDER 1
#include "globals.h"

#define ExIO(x,y)	Spi_Blk(x,x,y,0)

#define IO_Ex_Write 0x40
#define IO_Ex_Read 	0x41

/////////////////////// BANK == 1 /////////////////////////ALL PortA then All Port B
#define IO_Ex_1_IODIRA 		0x00
#define IO_Ex_1_IPOLA 		0x01
#define IO_Ex_1_GPINTENA 	0x02
#define IO_Ex_1_DEFVALA 	0x03
#define IO_Ex_1_INTCONA 	0x04
#define IO_Ex_1_IOCON 		0x05
#define IO_Ex_1_GPPUA 		0x06
#define IO_Ex_1_INTFA 		0x07
#define IO_Ex_1_INTCAPA 	0x08
#define IO_Ex_1_GPIOA 		0x09
#define IO_Ex_1_OLATA 		0x0A
#define IO_Ex_1_IODIRB 		0x10
#define IO_Ex_1_IPOLB 		0x11
#define IO_Ex_1_GPINTENB 	0x12
#define IO_Ex_1_DEFVALB 	0x13
#define IO_Ex_1_INTCONB 	0x14
#define IO_Ex_1_IOCON_b 	0x15
#define IO_Ex_1_GPPUB 		0x16
#define IO_Ex_1_INTFB 		0x17
#define IO_Ex_1_INTCAPB 	0x18
#define IO_Ex_1_GPIOB 		0x19
#define IO_Ex_1_OLATB 		0x1A

/////////////////////// BANK == 0 /////////////////////////PortA_PortB, PORTA_PORTB

#define IO_Ex_0_IODIRA 		0x00
#define IO_Ex_0_IODIRB 		0x01
#define IO_Ex_0_IPOLA 		0x02
#define IO_Ex_0_IPOLB 		0x03
#define IO_Ex_0_GPINTENA 	0x04
#define IO_Ex_0_GPINTENB 	0x05
#define IO_Ex_0_DEFVALA 	0x06
#define IO_Ex_0_DEFVALB 	0x07
#define IO_Ex_0_INTCONA 	0x08
#define IO_Ex_0_INTCONB 	0x09
#define IO_Ex_0_IOCON 		0x0A
#define IO_Ex_0_IOCON_b 	0x0B
#define IO_Ex_0_GPPUA 		0x0C
#define IO_Ex_0_GPPUB 		0x0D
#define IO_Ex_0_INTFA 		0x0E
#define IO_Ex_0_INTFB 		0x0F
#define IO_Ex_0_INTCAPA 	0x10
#define IO_Ex_0_INTCAPB 	0x11
#define IO_Ex_0_GPIOA 		0x12
#define IO_Ex_0_GPIOB 		0x13
#define IO_Ex_0_OLATA 		0x14
#define IO_Ex_0_OLATB 		0x15






extern void Spi_Blk(U8 *BuffOut,U8 *BuffIn,U16 Length, U8 Device);
extern void SpiSetup(void);
extern void DMA_Setup(void);

#endif
