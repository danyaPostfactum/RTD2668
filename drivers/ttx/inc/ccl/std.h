/*-------------------------------------------------------------------------*\
 *               File: std.h
 *                Ver: 0.1
 *             Author: Chi-Chun Cheng
 * Functions Provided: Provides the "C Chip Library" Standard header.
\*-------------------------------------------------------------------------*/

#ifndef	__C_CHIP_LIB_STANDARD_HEADER_INCLUDED__
#define	__C_CHIP_LIB_STANDARD_HEADER_INCLUDED__

/* Standard headers -------------------------------------------------------*/
#include <ttx_port.h>   /* Provides: uint8_t, uint16_t... */

/* CCL headers ------------------------------------------------------------*/
#include "std_hal.h"

/* Scope macros -----------------------------------------------------------*/
#ifdef __cplusplus
extern "C" namespace CCL
{
#endif

/* Macro Definitions ------------------------------------------------------*/

/* a few standard constants */
#ifndef TRUE
#define TRUE   (1)
#endif
#ifndef FALSE
#define FALSE  (0)
#endif


/* Invalid Data:
 *   In many instances, it is better to use CCL_INV as opposed to NULL
 *   because NULL may actually be a valid data.
 */
#define CCL_INV    (uint8_t)(-1)

/* Invalid Pointer:
 *   In many instances, it is better to use CCL_PTR_INV as opposed to NULL
 *   because NULL may actually be a valid pointer
 */
#define CCL_PTR_INV    ((void*)(-1))


#ifndef CCL_UNREFERENCED_PARAMETER
#define CCL_UNREFERENCED_PARAMETER(P)    ((P)=(P))
#endif

#ifndef REG32
#define REG32(addr) (*(volatile uint32_t*)(addr))
#define REG16(addr) (*(volatile uint16_t*)(addr))
#define REG8(addr)  (*(volatile  uint8_t*)(addr))
#endif


#ifdef __cplusplus
}	/* extern "C" namespace CCL */
#endif

#endif	/* !__C_CHIP_LIB_STANDARD_HEADER_INCLUDED__ */


/*-------------------------------------------------------------------------*\
 * end of std.h                                                            *
\*-------------------------------------------------------------------------*/


