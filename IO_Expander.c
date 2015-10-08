#include <main.h>
//#include <stdint.h>
//#include <stdio.h>
#include "TM4C123GH6PM.h"                    // Device header
//#include "Serial.h"
//#include "inc/hw_ints.h"
//#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
//#include "inc/hw_types.h"
//#include "inc/hw_flash.h"
//#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"

//#include "driverlib/cpu.h"
//#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
//#include "driverlib/gpio.h"
//#include "driverlib/uart.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/timer.h"
#include "driverlib/ssi.h"
#include "driverlib/udma.h"
#include <string.h>

 void SSI3_Handler()
 {
 
 }
 
 void Spi_Blk(U8 *BuffIn,U8 *BuffOut,U16 Length, U8 Device)
 {
	SSIDMAEnable(SSI3_BASE, SSI_DMA_RX | SSI_DMA_TX);




	SSIDMADisable(SSI3_BASE,SSI_DMA_RX | SSI_DMA_TX);
	return;
 }
 
 void SpiSetup()
 {
	SSIClockSourceSet(SSI3_BASE, SSI_CLOCK_SYSTEM);
	SSIConfigSetExpClk(SSI3_BASE, SysCtlClockGet(),	SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, SysCtlClockGet() / 16, 8);
	SSIAdvModeSet(SSI3_BASE, SSI_ADV_MODE_LEGACY);
	
	
	SSIEnable(SSI3_BASE);
	return;
 }
 
 void DMA()
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
 
