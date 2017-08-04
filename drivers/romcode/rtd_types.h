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

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
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
#define _BIT0   						0x01
#define _BIT1   						0x02
#define _BIT2   						0x04
#define _BIT3   						0x08
#define _BIT4   						0x10
#define _BIT5   						0x20
#define _BIT6   						0x40
#define _BIT7   						0x80
#define _BIT8   						0x0100
#define _BIT9   						0x0200
#define _BIT10  						0x0400
#define _BIT11  						0x0800
#define _BIT12  						0x1000
#define _BIT13  						0x2000
#define _BIT14  						0x4000
#define _BIT15  						0x8000
#define _BIT16   						0x00010000
#define _BIT17  						0x00020000
#define _BIT18   						0x00040000
#define _BIT19   						0x00080000
#define _BIT20   						0x00100000
#define _BIT21   						0x00200000
#define _BIT22   						0x00400000
#define _BIT23   						0x00800000
#define _BIT24   						0x01000000
#define _BIT25   						0x02000000
#define _BIT26  						0x04000000
#define _BIT27  						0x08000000
#define _BIT28  						0x10000000
#define _BIT29  						0x20000000
#define _BIT30  						0x40000000
#define _BIT31  						0x80000000


#endif
