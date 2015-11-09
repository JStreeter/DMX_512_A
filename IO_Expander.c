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
#include "globals.h"
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

	CS = 0;//OPEN DRAIN
	do
	{
		SSIDataPut(SSI3_BASE,(U32)BuffOut[index]);
		while(SSIBusy(SSI3_BASE));
		//BuffIn[index] = (U8)HWREG(SSI3_BASE + SSI_O_DR);
		SSIDataGet(SSI3_BASE,&temp);
		BuffIn[index] = (U16)temp;
	}while(index++ < Length);
	CS = 1;
	
	return;
 }
 
void Spi_Blk_8(U8 *BuffOut,U8 *BuffIn,U16 Length, U8 Device)
{
	U16 index = 0;
	U32 temp;

	CS = 0;//
	do
	{
		SSIDataPut(SSI3_BASE,(U32)BuffOut[index]);
		while(SSIBusy(SSI3_BASE));
		//BuffIn[index] = (U8)HWREG(SSI3_BASE + SSI_O_DR);
		SSIDataGet(SSI3_BASE,&temp);
		BuffIn[index] = (U8)temp;
	}while(index++ < Length);
	CS = 1;

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
	SSIConfigSetExpClk(SSI3_BASE, SysCtlClockGet(),	SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 10000, 8);
	SSIAdvModeSet(SSI3_BASE, SSI_ADV_MODE_READ_WRITE);
		
	SSIEnable(SSI3_BASE);

//	Buffer[0] = IO_Ex_Write | IO_Ex_0_IOCON;									//WRITE
//	Buffer[1] = 0x0404;	
//	ExIO(Buffer,2);

//	Buffer[0] = IO_Ex_Write | IO_Ex_0_GPPUA;									//WRITE
//	Buffer[1] = (0xFF << 8) + 0x80;//All of A plus the highest bit of B;
//	ExIO(Buffer,2);
//	
//	Buffer[0] = IO_Ex_Write | IO_Ex_0_IODIRA;									//WRITE
//	Buffer[1] = (0xFF << 8) + 0x80;//All of A plus the highest bit of B
//	ExIO(Buffer,2);
//	
//	Buffer[0] = IO_Ex_Write | IO_Ex_0_OLATA;// IO_Ex_0_IODIRA;		
//	Buffer[1] = 0x007F ;
//	ExIO(Buffer,2);

	return;
 }
 
 U16 ReadAddessEXIO()
 {
	U16 Buffer[3];
	U16 RV = 0;
	Buffer[0] = IO_Ex_Read | IO_Ex_0_GPIOA;
	Buffer[1] = 0x0000;
	ExIO(Buffer,2);
	
	RV = ((Buffer[1] >> 8) & 0x01FF) | ((Buffer[1] & 0x0080)<<1) ;
	
	//printf("Raw %04X | %04X\r",Buffer[1],RV);
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
