#include <main.h>
#include <stdio.h>
#include "TM4C123GH6PM.h"                    // Device header
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "inc/hw_gpio.h"

#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/udma.h"
#include <string.h>

#include "IO_Expander.h"
 void SSI3_Handler()
 {
	
	
	return;
 }
 
 void UDMA_Handler()
 {
	
	
	return;
 }
 void Spi_Blk(U16 *BuffOut,U16 *BuffIn,U16 Length, U8 Device)
 {
	U16 index = 0;
	U32 temp;
//	SSIDMAEnable(SSI3_BASE, SSI_DMA_RX | SSI_DMA_TX);
//	GPIOPinWrite(GPIOD_BASE, GPIO_PIN_1, 0x00);//Write to the pins
	GPIOPinWrite(GPIOD_BASE, GPIO_PIN_1, 0xFF);//Write to the pins
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	do
	{
		SSIDataPut(SSI3_BASE,(U32)BuffOut[index]);
		while(SSIBusy(SSI3_BASE));
		//BuffIn[index] = (U8)HWREG(SSI3_BASE + SSI_O_DR);
		SSIDataGet(SSI3_BASE,&temp);
		BuffIn[index] = (U16)temp;
	}while(index++ < Length);
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	__NOP;
	GPIOPinWrite(GPIOD_BASE, GPIO_PIN_1, 0x00);//Write to the pins
//	SSIDMADisable(SSI3_BASE,SSI_DMA_RX | SSI_DMA_TX);
//	GPIOPinWrite(GPIOD_BASE, GPIO_PIN_1, 0xFF);//Write to the pins
	return;
 }
 
 void SpiSetup()
 {
	U16 Buffer[3];
    // Enable the SSI0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
 
    // The SSI0 peripheral is on Port A and pins 2,3,4 and 5.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
 
	GPIOPadConfigSet(GPIOD_BASE,GPIO_PIN_0,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOD_BASE,GPIO_PIN_0,GPIO_DIR_MODE_HW);
	
	GPIOPadConfigSet(GPIOD_BASE,GPIO_PIN_2,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOD_BASE,GPIO_PIN_2,GPIO_DIR_MODE_HW);
	
	GPIOPadConfigSet(GPIOD_BASE,GPIO_PIN_3,GPIO_STRENGTH_8MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOD_BASE,GPIO_PIN_3,GPIO_DIR_MODE_HW);
 
    GPIOPinConfigure(GPIO_PD0_SSI3CLK);
    GPIOPinConfigure(GPIO_PD2_SSI3RX);//MISO
    GPIOPinConfigure(GPIO_PD3_SSI3TX);//MOSI
 
    // Configures the pins for use by the SSI, takes 2 parameters
    GPIOPinTypeSSI(GPIOD_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_0);

	SSIClockSourceSet(SSI3_BASE, SSI_CLOCK_SYSTEM);
	SSIConfigSetExpClk(SSI3_BASE, SysCtlClockGet(),	SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 10000, 16);
//	SSIAdvModeSet(SSI3_BASE, SSI_ADV_MODE_LEGACY);
	
	
	SSIEnable(SSI3_BASE);
while(1)
{
	Buffer[0] = IO_Ex_Write | IO_Ex_0_IOCON;									//WRITE
	Buffer[1] = 0x0404;	
	ExIO(Buffer,2);

	Buffer[0] = IO_Ex_Write | IO_Ex_0_GPPUA;									//WRITE
	Buffer[1] = 0x01FF;
	ExIO(Buffer,2);
	
	Buffer[0] = IO_Ex_Write | IO_Ex_0_IODIRA;									//WRITE
	Buffer[1] = 0x01FF;
	ExIO(Buffer,2);
	
	Buffer[0] = IO_Ex_Write | IO_Ex_0_OLATA;// IO_Ex_0_IODIRA;		
	Buffer[1] = ~(0x01FF) ;
	ExIO(Buffer,2);

//////////////////////////
	Buffer[0] = IO_Ex_Read | IO_Ex_0_IOCON;									//WRITE
	Buffer[1] = 0;	
	ExIO(Buffer,2);
	printf("IO_Ex_0_IOCON  %04X-> %04X\r\n",IO_Ex_Read | IO_Ex_0_IOCON,Buffer[1]);
//////////////////////////

//////////////////////////
	Buffer[0] = IO_Ex_Read | IO_Ex_0_GPPUA;									//WRITE
	Buffer[1] = 0;	
	ExIO(Buffer,2);
	printf("IO_Ex_0_GPPUA  %04X-> %04X\r\n",IO_Ex_Read | IO_Ex_0_GPPUA,Buffer[1]);
//////////////////////////

//////////////////////////
	Buffer[0] = IO_Ex_Read | IO_Ex_0_IODIRA;									//WRITE
	Buffer[1] = 0;	
	ExIO(Buffer,2);
	printf("IO_Ex_0_IODIRA %04X-> %04X\r\n",IO_Ex_Read | IO_Ex_0_IODIRA,Buffer[1]);
//////////////////////////

//////////////////////////
	Buffer[0] = IO_Ex_Read | IO_Ex_0_OLATA;									//WRITE
	Buffer[1] = 0;	
	ExIO(Buffer,2);
	printf("IO_Ex_0_OLATA  %04X-> %04X\r\n",IO_Ex_Read | IO_Ex_0_OLATA,Buffer[1]);
//////////////////////////	
}
	return;
 }
 
 void DMA_Setup()
 {
//	//
//// The application must allocate the channel control table. This one is a
//// full table for all modes and channels.
//// NOTE: This table must be 1024-byte aligned.
//////
//	uint8_t pui8DMAControlTable[1024];
//	//
//	uint8_t pui8SourceBuffer[256];
//	uint8_t pui8DestBuffer[256];
//	//
//	// Enable the uDMA controller.
//	//
//	uDMAEnable();
//	//
//	// Set the base for the channel control table.
//	//
//	uDMAControlBaseSet(&pui8DMAControlTable[0]);
//	//
//	// No attributes must be set for a software-based transfer. The attributes
//	// are cleared by default, but are explicitly cleared here, in case they
//	// were set elsewhere.
//	//
//	uDMAChannelAttributeDisable(UDMA_CHANNEL_SW, UDMA_CONFIG_ALL);
//	//
//	// Now set up the characteristics of the transfer for 8-bit data size, with
//	// source and destination increments in bytes, and a byte-wise buffer copy.
//	// A bus arbitration size of 8 is used.
//	//
//	uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT,
//	UDMA_SIZE_8 | UDMA_SRC_INC_8 |
//	UDMA_DST_INC_8 | UDMA_ARB_8);
//	//
//	// The transfer buffers and transfer size are now configured. The transfer
//	// uses AUTO mode, which means that the transfer automatically runs to
//	// completion after the first request.
//	//
//	uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT,
//	UDMA_MODE_AUTO, pui8SourceBuffer, pui8DestBuffer,
//	sizeof(pui8DestBuffer));
//	//
//	// Finally, the channel must be enabled. Because this is a software-
//	// initiated transfer, a request must also be made. The request starts the
//	// transfer.
//	//
//	uDMAChannelEnable(UDMA_CHANNEL_SW);
//	uDMAChannelRequest(UDMA_CHANNEL_SW);
 }
 
 //817 - 223 - 3344 David Dodson
 U16 ReadAddessEXIO()
 {
	U16 Buffer[3];
	U8 RV = 0;
	Buffer[0] = IO_Ex_Read | IO_Ex_0_GPIOA;
	Buffer[1] = 0x0000;
	ExIO(Buffer,2);
	RV = (Buffer[1] >> 8) & 0x01FF;
	return RV;
 }
 
 void WriteOutIOEX(U16 output)
 {
	U16 Buffer[3];	
	Buffer[0] = IO_Ex_Write | IO_Ex_0_OLATA;// IO_Ex_0_IODIRA;		
	Buffer[1] =  ((~output) & 0x007F) | 0xFF80;
	ExIO(Buffer,2);
	return;
 }
