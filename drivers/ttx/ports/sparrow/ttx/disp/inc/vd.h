/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     vd.h
 * @brief    Internal (package scope) Ttx/C interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Dec. 21, 2007
 * @version  0.1
 * @ingroup  osd
\*****************************************************************************/

/**
 * @addtogroup vd
 * @{
 */
#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_VIDEO_DECODER_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_VIDEO_DECODER_HEADER_INCLUDED__

/* -- CCL headers -----------------------------------------------------------*/
#include "vd_hal.h"
#include <ttx_pkg.h>    /* Provides: uint32_t */
#include <ttx_log.h>    /* Provides: TTX_Warning() */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/

    /* Same register read/write functions as CCL */
#define VD_RegRead(addr)            _VD_REG_READ(addr)
#define VD_RegWrite(addr,val)       _VD_REG_WRITE(addr,val)

    /* VD Log functions */
#define VD_Log                      TTX_Log

    /* VD Assertion functions */
#define VD_REQUIRE(_test)           TTX_REQUIRE(_test)
#define VD_ENSURE(_test)            TTX_ENSURE(_test)
#define VD_ALLEGE(_test)            TTX_ALLEGE(_test)


    /* VD_open() flags */
    typedef enum
    {
        VD_OPEN_NOP   = 0x00000000u,
        VD_OPEN_RESET = 0x00000001u
    } VD_OpenFlag_t;

    /* -- Global typedef Declarations -------------------------------------------*/
    typedef uint16_t           VD_Address_t;
    typedef uint32_t           VD_Data_t;     /* width of data bus of OSD */



    /* -- Function Prototypes ---------------------------------------------------*/

    bool_t                  VD_Open                             (const VD_OpenFlag_t flags);
    bool_t                  VD_Close                            (void);
    void                    VD_Reset                            (void);
    void                    VD_TestPwrOnDefaults                (void);
    bool_t                  VD_Test                             (void);


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_VIDEO_DECODER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * End of vd.h                                                               *
\*---------------------------------------------------------------------------*/
