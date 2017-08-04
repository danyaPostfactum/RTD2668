/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     slr.h
 * @brief    Vertical Blanking Interval (VBI) Slicer driver.
 * @author   Janus Cheng
 * @date     Apr. 17, 2009
 * @version  0.1
 * @ingroup  vbi
\*****************************************************************************/

/**
 * @addtogroup vbi
 * @{
 */
#ifndef __ENHANCED_TELETEXT_LIBRARY_VERTICAL_BLANKING_INTERVAL_SLICER_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_LIBRARY_VERTICAL_BLANKING_INTERVAL_SLICER_HEADER_INCLUDED__ (1)

#define TTX_SPARROW

/* -- CCL headers -----------------------------------------------------------*/
#include "slr_hal.h"

/* -- TTX headers -----------------------------------------------------------*/
#include <ttx_pkg.h>    /* Provides: uint32_t */
#include <ttx_log.h>    /* Provides: TTX_Warning() */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/

    /* Same register read/write functions as CCL */
#define SLR_RegRead(addr)               _SLR_REG_READ(addr)
#define SLR_RegWrite(addr,val)          _SLR_REG_WRITE(addr,val)

    /* SLR Log functions */
#define SLR_Trace                       TTX_Trace
#define SLR_Warning                     TTX_Warning
#define SLR_Error                       TTX_Error


    /* SLR Assertion functions */
#define SLR_REQUIRE(_test)              TTX_REQUIRE(_test)
#define SLR_ENSURE(_test)               TTX_ENSURE(_test)
#define SLR_ALLEGE(_test)               TTX_ALLEGE(_test)
#define SLR_ASSERT_COMPILE(_test)       TTX_ASSERT_COMPILE(_test)

    /* SLR_open() flags */
    typedef enum
    {
        SLR_OPEN_NOP   = 0x00000000u,
        SLR_OPEN_RESET = 0x00000001u
    } SLR_OpenFlag_t;

    /* -- Global typedef Declarations -------------------------------------------*/
    typedef uint16_t           SLR_Address_t;
    typedef uint32_t           SLR_Data_t;     /* width of data bus of slicer */




    /* -- Function Prototypes ---------------------------------------------------*/

    bool_t                  SLR_Open                      (const SLR_OpenFlag_t flags);
    bool_t                  SLR_Close                     (void);
    void                    SLR_Reset                     (void);


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__ENHANCED_TELETEXT_LIBRARY_VERTICAL_BLANKING_INTERVAL_SLICER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of slr.h                                                              *
\*---------------------------------------------------------------------------*/
