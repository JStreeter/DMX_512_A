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

#endif
