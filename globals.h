/* 
 * File:   globals.h
 * Author: Jonathan Streeter
 *
 * Created on 9/28/15, 11:58 AM
 */

#ifndef CHIPSETUP_H
#define	CHIPSETUP_H

typedef signed char     S8;		//Standard Naming
typedef unsigned char   U8;     //Standard Naming
typedef short           S16;    //Standard Naming
typedef unsigned short  U16;    //Standard Naming
typedef int             S32;    //Standard Naming
typedef unsigned int    U32;    //Standard Naming
typedef long long       S64;    //Standard Naming
typedef unsigned long long U64; //Standard Naming


typedef enum
{
	Master,
	Slave
}_masterslave;
#define SWREG(x) (*((volatile uint32_t *)(x)))
#define SWREGBITW(x, b) SWREG(((uint32_t)(x) & 0x20000000) | 0x02000000 |                     \
							(((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2))

//#define PULSETEST      		(*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
#define CS      			(*((volatile uint32_t *)(0x42000000 + (0x400073FC-0x40000000)*32 + 1*4)))
#define RED_LED      		(*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
//#define BLUE_LED      		(*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 2*4)))
#warning BLUE LED IS RUINED!!!
#define BLUE_LED      		(*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 8*4)))
#define GREEN_LED   		(*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 3*4)))
#define DEro	   			(*((volatile uint32_t *)(0x42000000 + ((GPIOC_BASE + 0x3FC) -0x40000000)*32 + 6*4)))
#define PULLDOWNER 			(*((volatile uint32_t *)(0x42000000 + ((GPIOD_BASE + 0x3FC) -0x40000000)*32 + 6*4)))
#define IO_RESET	   		(*((volatile uint32_t *)(0x42000000 + ((GPIOE_BASE + 0x3FC) -0x40000000)*32 + 1*4)))

extern U8	A_DMX[513];
extern U8	B_DMX[513];
extern U8 	ShadowDMX[513];
extern U16 	Incoming_Counter;
extern U8 	IncomingDMX[6];
extern U16 	Address;
extern U32 	BBFlags;
extern U16 	MaxSend;

#define PingPongSemaphore		SWREGBITW(&BBFlags, 0)
#define LastPingPongSemaphore	SWREGBITW(&BBFlags, 1)
#define Master_Slave			SWREGBITW(&BBFlags, 2)
#define RXREADY					SWREGBITW(&BBFlags, 3)
#define ThePollTrigger			SWREGBITW(&BBFlags, 4)
#define BREAK					SWREGBITW(&BBFlags, 5)
#define OneShotTX				SWREGBITW(&BBFlags, 6)
#define OneShotRX				SWREGBITW(&BBFlags, 7)

typedef enum
{
	Null,						//Empty
	Standard,
} _DMX_Message_Type;

struct _DMX_Message;
struct _DMX_Message_Flags;

typedef struct _sQueque_Link QLink;
typedef struct _DMX_Message DmxMessage;
typedef struct _DMX_Message_Flags DmxFlags;

extern _masterslave MasterSlave;

struct _DMX_Message_Flags
{
	U16 RFU : 16;				//
};

struct _DMX_Message
{
	_DMX_Message_Type	Type;
	DmxFlags			MessageFlags;
	U32					RFU;
};

struct _sQueque_Link
{
	DmxMessage	Message;
	QLink		*Link;
};

extern const char * StringCheck[];

extern void IntGlobals(void);
extern U8 LocalHead;
#endif
