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


#define SWREG(x) (*((volatile uint32_t *)(x)))
#define SWREGBITW(x, b) SWREG(((uint32_t)(x) & 0x20000000) | 0x02000000 |                     \
							(((uint32_t)(x) & 0x000FFFFF) << 5) | ((b) << 2))

#define PULSETEST      	(*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
#define CS      		(*((volatile uint32_t *)(0x42000000 + (0x400073FC-0x40000000)*32 + 1*4)))
extern U8	PingDMX[513];
extern U8	PongDMX[513];
extern U32 	BBFlags;
#define PingPongSemaphore	SWREGBITW(&BBFlags, 0)
#define LastPingPongSemaphore	SWREGBITW(&BBFlags, 1)
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

extern void IntGlobals(void);
#endif
