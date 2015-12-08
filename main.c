/// @file    main.c
/// @author  Jonathan Streeeter
/// @date    Septmber 14, 2015
/// @brief   Main code to control DMX master
/// @details 
////////////////////////////////////////////////////////////////////////////////
/*
C4 U1Rx
C5 U1TX
C6 DEro
C7 FAULT
D6 Override
*/
////////////////////////////////////////////////////////////////////////////////
	// INCLUDES
////////////////////////////////////////////////////////////////////////////////
#include <main.h>
#include <stdint.h>
#include <stdio.h>
#include "TM4C123GH6PM.h"                    // Device header
#include "Serial.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ssi.h"
#include "inc/hw_uart.h"

#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/ssi.h"
#include "driverlib/udma.h"
#include <string.h>

#include "IO_Expander.h"
#include "Paser.h"
////////////////////////////////////////////////////////////////////////////////
	// EXTERNALS
////////////////////////////////////////////////////////////////////////////////
volatile U8 Semaphore = 0;
FILE __stdout;
////////////////////////////////////////////////////////////////////////////////
	// GLOBALS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	//  CONSTANTS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void SystemInit() //THIS RUNS FIRST!!! on BOOT UP!!
{
	//Main set
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN | SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_64);
	HWREG(SYSCTL_RCC) |= SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_64;
	
	//Perhial Clock enable
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//UART0Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//UART1Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//UART1Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//UART1Pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//Leds Push Buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//Leds Push Buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//A0,A1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//B0,B1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);//E5
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA); //Serial DMA
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); //Servo
	
	SysCtlGPIOAHBDisable(SYSCTL_PERIPH_GPIOF);	//un needed
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//Timer 0//Short time
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);//Timer 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);//Timer 2

    // Set GPIO ports to use APB (not needed since default configuration -- for clarity)
    // Note UART on port A must use APB
    SYSCTL->GPIOHBCTL = 0;

	GPIOPinWrite(GPIOE_BASE, GPIO_PIN_1, 0);//Write to the pins
	
    // Configure LED and pushbutton pins
	GPIOPadConfigSet(GPIOE_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOE_BASE,GPIO_PIN_3,GPIO_DIR_MODE_OUT);
	
	//CCS
	GPIOPadConfigSet(GPIOD_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOD_BASE,GPIO_PIN_1,GPIO_DIR_MODE_OUT);
	
	//Pulldowner
	GPIOPadConfigSet(GPIOD_BASE,GPIO_PIN_6,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_OD);
	GPIODirModeSet(GPIOD_BASE,GPIO_PIN_6,GPIO_DIR_MODE_OUT);
	
	//Reset Line to the IO Expander
	GPIOPadConfigSet(GPIOD_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOD_BASE,GPIO_PIN_2,GPIO_DIR_MODE_OUT);
	
	//Leds First
//    GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
//	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3,GPIO_DIR_MODE_OUT);
	
    GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_1 |  GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_1 |  GPIO_PIN_3,GPIO_DIR_MODE_OUT);
	//Leds First Blue PWM
	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_2,GPIO_DIR_MODE_HW);
		//I/O
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	
	
	//Push Buttons
	GPIOPadConfigSet(GPIOF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIODirModeSet(GPIOF_BASE,GPIO_PIN_0 | GPIO_PIN_4,GPIO_DIR_MODE_IN);
	
	GPIOPadConfigSet(GPIOE_BASE,GPIO_PIN_1 ,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOE_BASE,GPIO_PIN_1 ,GPIO_DIR_MODE_OUT);

///////////////Debug/////////////////////////////////////////////////////////////////////////////////////	
	GPIOPinTypeUART(GPIOA_BASE,GPIO_PIN_0 | GPIO_PIN_1);
	//
	// Enable UART1 functionality on GPIO Port B pins 0 and 1.
	//
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinConfigure(GPIO_PA0_U0RX);

   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
	UARTEnable(UART0_BASE);
	UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);
/*DEBUG*/	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), DebugBaud, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);
	//UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), DMXBAUD, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);	
	UARTFIFODisable(UART0_BASE);
	
//END/////////////Debug//////////////////////////////////////////////////////////////////////////////////

///////////////Debug TX ONLY!/////////////////////////////////////////////////////////////////////////////////////	
	GPIOPinTypeUART(GPIOE_BASE,GPIO_PIN_5);
	GPIOPinConfigure(GPIO_PE5_U5TX);

   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
	UARTEnable(UART5_BASE);
	UARTClockSourceSet(UART5_BASE,UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), DebugBaud, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_TWO);
	UARTFIFODisable(UART5_BASE);
	
//END/////////////Debug//////////////////////////////////////////////////////////////////////////////////
	
///////////////DMX-512-A/////////////////////////////////////////////////////////////////////////////////////	

	GPIOPinTypeUART(GPIOC_BASE,GPIO_PIN_5 | GPIO_PIN_4);
	
   	// Configure UART0 to 115200 baud, 8N1 format (must be 3 clocks from clock enable and config writes)
	UARTEnable(UART1_BASE);
	UARTClockSourceSet(UART1_BASE,UART_CLOCK_SYSTEM);
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), DMXBAUD, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_TWO);
	UARTFIFODisable(UART1_BASE);
	
	GPIOPinConfigure(GPIO_PC5_U1TX);
	GPIOPinConfigure(GPIO_PC4_U1RX);

	//TX/rx PIN
	GPIOPadConfigSet(GPIOC_BASE,GPIO_PIN_6 ,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);
	GPIODirModeSet(GPIOC_BASE,GPIO_PIN_6 ,GPIO_DIR_MODE_OUT);
	
	//Fault Pin
	GPIOPadConfigSet(GPIOC_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIODirModeSet(GPIOC_BASE,GPIO_PIN_7,GPIO_DIR_MODE_IN);
//END/////////////DMX-512-A//////////////////////////////////////////////////////////////////////////////////

//TIMER//
/////////TIMER 0/////////
	TimerConfigure(TIMER0_BASE,TIMER_CFG_ONE_SHOT);
	TimerClockSourceSet(TIMER0_BASE,TIMER_CLOCK_SYSTEM);
	TimerLoadSet(TIMER0_BASE,TIMER_A,TimeDebug1);//API says to use the Timer A if full width
///////END TIMER 0///////
/////////TIMER 1 40 Hertz Timer/////////
	TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC);
	TimerClockSourceSet(TIMER1_BASE,TIMER_CLOCK_SYSTEM);
	TimerLoadSet(TIMER1_BASE,TIMER_A,TimeDebug1);//API says to use the Timer A if full width
///////END TIMER 1///////
///////////TIMER 2/////////
	TimerConfigure(TIMER2_BASE,TIMER_CFG_ONE_SHOT);
	TimerClockSourceSet(TIMER2_BASE,TIMER_CLOCK_SYSTEM);
	TimerLoadSet(TIMER2_BASE,TIMER_A,TimeDebug1);//API says to use the Timer A if full width
/////////END TIMER 2///////
//END TIMER//
	IntPrioritySet(INT_UART0, 0x05);
	IntPrioritySet(INT_SSI3, 0x10);
	
	HIndex 		= 0;
	TIndex 		= 0;	
	RED_LED 	= 0;
	BLUE_LED 	= 0;
	GREEN_LED 	= 0;
	return;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//       @@@@      @@@@      @@@@          @@@@@@@@      @@@@      @@@@       //
//       @@@@@@  @@@@@@    @@@@@@@@          @@@@        @@@@@@    @@@@       //
//       @@@@@@@@@@@@@@  @@@@    @@@@        @@@@        @@@@@@@@  @@@@       //
//       @@@@@@@@@@@@@@  @@@@    @@@@        @@@@        @@@@  @@@@@@@@       //
//       @@@@  @@  @@@@  @@@@@@@@@@@@        @@@@        @@@@    @@@@@@       //
//       @@@@      @@@@  @@@@    @@@@        @@@@        @@@@      @@@@       //
//       @@@@      @@@@  @@@@    @@@@      @@@@@@@@      @@@@      @@@@       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// 100uS low 14uS High  -------BREAK___________________MAB--PACKET0+Data1+Data2
//Minimum Packet is 24 data packets(Add padding)
////////////////////////////////////////////////////////////////////////////////


int main(void)
{
//    U32 lfsr = 0xAAAAu;/* Any nonzero start state will work. */
	volatile S16 TempCh;
	U16 In,oldIn,RxBufpt;
	U16 Buffer[5];
	U32 PWM;
	volatile U32 BaseTime = TimeDebug1;
//	unsigned bit;
//	U8 Ms = 1;
	U8 x;
	
	IO_RESET = 0;
	
	UARTIntEnable(UART0_BASE,UART_INT_RX);				//Uart Intterrupt for USer
	UARTIntEnable(UART1_BASE,UART_INT_RX | UART_INT_BE);//Incoming DMX PLUS frame error to reset the counter
	//UARTIntEnable(UART5_BASE,UART_INT_TX);			//Extra uart for testing
	IntGlobals();										// Set all of the varibles to intail settings
	
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);		//Short timer for the Break and MAB
	TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);		// 40 Hertz timer
	TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);	//??

	IntEnable(INT_UART0);								//DEBUG PORT
	IntEnable(INT_UART1);								//DMX PORT
	IntEnable(INT_UART5);								//DEBUG TX ONLY!!! NOT FOR TRUE USE
	
	IntEnable(INT_TIMER0A);								//The Break Before Make timer
	IntEnable(INT_TIMER1A);								// 40 hertz
	IntEnable(INT_TIMER2A);								// 20ms

	uDMAChannelEnable(UDMA_CHANNEL_UART1TX);			//DMA enable for the DMX transmit
	UARTDMAEnable(UART1_BASE, UART_DMA_TX);				//GOGO GADGET DMA
	
	IntMasterEnable();									//Interrupts are now a GO!	
		
	TimerEnable(TIMER0_BASE, TIMER_BOTH);				//Trigger a time
	
	RngFlush(&RxBufpt);									//Just too make sure there is nothing in the buffer
	
	Semaphore = 0;										//Setup
	
	TIMER1->TAILR = 1250000;//1 / 40 					//Time set to 40 hertz
	TIMER1->CTL |= TIMER_CTL_TAEN | TIMER_CTL_TBEN;		//GO!!!
	
	
	
	x = 0;
	IO_RESET = 1;
	DEro = 0;
	SpiSetup();											//Spi setup
	SSIIntEnable(SSI3_BASE,SSI_RXFF);					//Interrupt enable
	
	Semaphore = 0;
	
	GREEN_LED = 1;										//Green to start
	while(x < 20) 										//Wait for a little bit after everything is setup
	{
		if(Semaphore >= 1)
		{
			Semaphore = 0;
			x++;
		}
	}
	//Proof that it at least made it this far
	GREEN_LED = 0;										//Green is off

	printf("\r\nHello World\r\n");						//Proof that the Serial User works
	
	BLUE_LED = 1;//Blue is on							// Waiting for ther user to press a button

	while(GPIOPinRead(GPIOF_BASE, GPIO_PIN_4));			//Wait of user
	BLUE_LED = 0; 										// Blue is now off

	DMA_Setup_UART1();									//Vroom Vroom!!!
														
	WriteOutIOEX(0);									//First Write to the spi so that it setup for a read	
	printf("Ready\r\n");								//Set to go 
	
	MasterSlave = Master;								//We start off by being master
	
	In =  ReadAddessEXIO();								//WHAT IS THE ADDRESS!!!!
	oldIn = In;											//Setup
	Address = In;										//Setup
	
	printf("In = %02X %s\r\n",In&0x1FF,MasterSlave==Master?"Master":"Slave");
	
	MaxSend = 4;										//DEBUG!!!
	
	PWM_Setup();
	HWREG(SYSCTL_RCC) |= SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_64;
	PWM = 0;
	while(1)
	{			
		{	// This section reads the I/O expander
			Buffer[0] = IO_Ex_Read | IO_Ex_0_GPIOA;			
			Buffer[1] = 0x0000;
			ExIO(Buffer,2);
			
			In =  ReadAddessEXIO();
		}
		
		if(oldIn != In)			//If there has been a change then
		{
			printf("In = %02X %s\r\n",In&0x1FF,MasterSlave==Master?"Master":"Slave");
			printf("%d \r\n",PWMClockGet(PWM1_BASE));
			Address = In;		//UPDATE THE ADDRESS!!!!
			oldIn = In;			//Prevent spam
		}

		{	//DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG 
			if(!GPIOPinRead(GPIOF_BASE, GPIO_PIN_4))
			{
				Incoming_Counter= 0;
				DEBUGENputc(0x00);
				x++;
				DEBUGENputc(x-1);
				DEBUGENputc(x);
				DEBUGENputc(x+1);
				RED_LED ^= 1;
				while(!GPIOPinRead(GPIOF_BASE, GPIO_PIN_4));
			}
		}	//DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG 

		{	//Keep Alive light
			if(	Semaphore >= 16)	
			{	
				
				//BLUE_LED ^= 1;
				Semaphore = 0;
			//	printf("%d\r\n",PWM);
				//390 	//1 ms
				//1,562 // 2 ms
				PWM += (1952-390) / 128;
				if(PWM >= 1952) PWM = 390;
			
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, PWM);
			}
		}
		
		if(RXREADY)	//DMX has GOT SOMETHING!!!!
		{
			printf("I GOT DATA!!!\r\n %02X %02X\r\n",IncomingDMX[0],IncomingDMX[1]);
			WriteOutIOEX(IncomingDMX[1] & 0x7F);
			RXREADY = 0;
		}

		{	//User input
			TempCh = RngGet(&RxBufpt);
			if(TempCh != EOF)
			{
				parseCommand((U8)TempCh);
			}
		}
	}
	//Should never end up here
}

void masscopy()
{

	return;
}
