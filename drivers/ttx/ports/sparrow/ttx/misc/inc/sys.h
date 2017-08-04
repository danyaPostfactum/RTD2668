/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     sys.h
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
#ifndef __C_CHIP_LIB_REALTEK_RTD_2668_SYSTEM_HARDWARE_LIBRARY_LAYER_HEADER_INCLUDED__
#define __C_CHIP_LIB_REALTEK_RTD_2668_SYSTEM_HARDWARE_LIBRARY_LAYER_HEADER_INCLUDED__

/* -- CCL headers -----------------------------------------------------------*/
#include "sys_hal.h"
#include <ttx_pkg.h>    /* Provides: uint32_t */
#include <ttx_log.h>    /* Provides: TTX_Warning() */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/

    /* Same register read/write functions as CCL */
#define SYS_RegRead(addr)            _SYS_REG_READ(addr)
#define SYS_RegWrite(addr,val)       _SYS_REG_WRITE(addr,val)

    /* VD Log functions */
#define SYS_Log                      TTX_Log

    /* VD Assertion functions */
#define SYS_REQUIRE(_test)           TTX_REQUIRE(_test)
#define SYS_ENSURE(_test)            TTX_ENSURE(_test)
#define SYS_ALLEGE(_test)            TTX_ALLEGE(_test)


    /* SYS_open() flags */
    typedef enum
    {
        SYS_OPEN_NOP   = 0x00000000u,
        SYS_OPEN_RESET = 0x00000001u
    } SYS_OpenFlag_t;

    /* -- Global typedef Declarations -------------------------------------------*/



    /* -- Function Prototypes ---------------------------------------------------*/
    bool_t                  SYS_Open                          (const SYS_OpenFlag_t flags);
    bool_t                  SYS_Close                         (void);

    /* Performs SYS tests */
    bool_t                  SYS_Test                          (void);

    /* Copy data from SPI flash to DMEM or registers */
    void                    SYS_SDMA_Copy                     (void TTX_MEM*         addrTo  , \
            uint8_t               bankFrom, \
            const void TTX_ROM*   addrFrom, \
            uint16_t              numOfBytes);


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__C_CHIP_LIB_REALTEK_RTD_2668_SYSTEM_HARDWARE_LIBRARY_LAYER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * End of sys.h                                                              *
\*---------------------------------------------------------------------------*/
