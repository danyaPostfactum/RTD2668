/*=============================================================
 * Copyright (c)      Realtek Semiconductor Corporation, 2004 *
 * All rights reserved.                                       *
 *============================================================*/
/*======================= Description ========================*/
/**
  * @file
 * We define the usual types in this file.
 * We define the usual types in this file such as INT8,uin8,etc.
 * Please include this file in all source file. And declare your
 * variables with this re-defined type.
 * @Author Moya Yu
 * @date  2004/4/7
 * @version 1.0
 */

/*======================= CVS Headers =========================
  $Header$

  $Log$
  Revision 1.1  2005/04/18 02:26:48  tzungder
  Initial revision

  Revision 1.1  2005/03/23 06:05:00  temin
  +: created for RTD2635 project

 *=============================================================*/


#ifndef _RTD_TYPES_H
#define _RTD_TYPES_H
//#include <autoconf.h>
/*
 * Internal names for basic integral types.  Omit the typedef if
 * not possible for a machine/compiler combination.
 */
#ifdef _WIN32 //for platform-acrossing of models, _WIN32: the pre-defined keyword of VC compiler
typedef 	unsigned __int64         UINT64; //!<for unsigned 64 bits integer in VC windows platform
typedef  __int64				INT64;//!<for signed 64 bits integer in VC windows platform
#else
//typedef unsigned long long		UINT64; //!< for unsigned 64 bits integer
//	typedef long long				INT64;     //!< for signed 64 bits integer
#endif
typedef unsigned long			UINT32;   //!< for unsigned integer (32 bits)
typedef long        		    INT32;    //!< for signed integer (32 bits)
typedef unsigned short			UINT16;  //!< for unsigned short (16 bits)
typedef short					INT16;    //!< for signed short (16 bits)

#ifndef  CONFIG_RTD2636E_A
typedef unsigned char			MUST_UINT8;   //!< for unsigned character (8 bits)
typedef char					MUST_INT8;    //!< for character (8 bits)
typedef unsigned char			UINT8;   //!< for unsigned character (8 bits)
typedef char					INT8;    //!< for character (8 bits)
#else
typedef unsigned short			UINT8;   //!< for unsigned character (8 bits)
typedef short					INT8;    //!< for character (8 bits)
typedef unsigned char			MUST_UINT8;   //!< for unsigned character (8 bits)
typedef char					MUST_INT8;    //!< for character (8 bits)
#endif

typedef unsigned int RBus_UInt32;
typedef unsigned short RBus_UInt16;
typedef unsigned char RBus_UInt8;

//typedef int BOOL;  //!< for boolean value
typedef float					FLOAT32;	//!< for float point (32 bits)
typedef double				FLOAT64;	//!< for float point (64 bits)

#ifndef bool
//typedef unsigned char	bool ;	//!< For Boolean declaration
typedef enum {FALSE = 0, TRUE} bool;
#endif

#ifndef BOOL
#define BOOL bool
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef _TRUE
#define _TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef _FALSE
#define _FALSE 0
#endif

#ifndef _ENABLE
#define _ENABLE 1
#endif

#ifndef _DISABLE
#define _DISABLE 0
#endif

#ifndef _ON
#define _ON	1
#endif

#ifndef _OFF
#define _OFF 0
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILED
#define FAILED -1
#endif

#define CLEARBITS(a,b)	((a) &= ~(b))
#define SETBITS(a,b)		((a) |= (b))
#define ISSET(a,b)		(((a) & (b))!=0)
#define ISCLEARED(a,b)	(((a) & (b))==0)

//#define SEC(x) (1000 * x)
#define SEC(x) 		(1000 * x)
#ifndef MAX
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#endif			   /* max */

#ifndef MIN
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
#endif			   /* min */

#define ASSERT_CSP(x) if (!(x)) {printk("\nAssertion fail at file %s, function %s, line number %d:, expression '%s'", __FILE__, __FUNCTION__, __LINE__, #x); while(1);}
#define ASSERT_ISR(x) if (!(x)) {printk("\nAssertion fail at file %s, function %s, line number %d:, expression '%s'", __FILE__, __FUNCTION__, __LINE__, #x); while(1);}


//------------------------------------------
// Definitions of Bits
//------------------------------------------
#define _ZERO   						0x00
#define _BIT0   						0x01UL
#define _BIT1   						0x02UL
#define _BIT2   						0x04UL
#define _BIT3   						0x08UL
#define _BIT4   						0x10UL
#define _BIT5   						0x20UL
#define _BIT6   						0x40UL
#define _BIT7   						0x80UL
#define _BIT8   						0x0100UL
#define _BIT9   						0x0200UL
#define _BIT10  						0x0400UL
#define _BIT11  						0x0800UL
#define _BIT12  						0x1000UL
#define _BIT13  						0x2000UL
#define _BIT14  						0x4000UL
#define _BIT15  						0x8000UL
#define _BIT16   						0x00010000UL
#define _BIT17  						0x00020000UL
#define _BIT18   						0x00040000UL
#define _BIT19   						0x00080000UL
#define _BIT20   						0x00100000UL
#define _BIT21   						0x00200000UL
#define _BIT22   						0x00400000UL
#define _BIT23   						0x00800000UL
#define _BIT24   						0x01000000UL
#define _BIT25   						0x02000000UL
#define _BIT26  						0x04000000UL
#define _BIT27  						0x08000000UL
#define _BIT28  						0x10000000UL
#define _BIT29  						0x20000000UL
#define _BIT30  						0x40000000UL
#define _BIT31  						0x80000000UL


//------------------------------------------
// Definitions of Pages
//------------------------------------------
#define _PAGE0   						0x00
#define _PAGE1   						0x01
#define _PAGE2   						0x02
#define _PAGE3   						0x03
#define _PAGE4   						0x04
#define _PAGE5   						0x05
#define _PAGE6   						0x06
#define _PAGE7   						0x07
#define _PAGE8   						0x08
#define _PAGE9   						0x09

//------------------------------------------
// Definitions of Bank
//------------------------------------------
#define _BANK0   						0x00
#define _BANK1   						0x01
#define _BANK2   						0x02
#define _BANK3   						0x03
#define _BANK4   						0x04
#define _BANK5   						0x05
#define _BANK6   						0x06
#define _BANK7   						0x07
#define _BANK8   						0x08
#define _BANK9   						0x09
#define _BANK10   					0x0a
#define _BANK11 						0x0b
#define _BANK12 						0x0c
#define _BANK13 						0x0d
#define _BANK14 						0x0e
#define _BANK15 						0x0f

#endif
