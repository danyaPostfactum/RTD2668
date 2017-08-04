/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     vte.h
 * @brief    Internal (package scope) Ttx/C interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Dec. 21, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_VBI_TRANSFER_ENGINE_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_VBI_TRANSFER_ENGINE_INCLUDED__ (1)

/* -- CCL headers -----------------------------------------------------------*/
#include "vte_hal.h"
#include "ttx_pkg.h"
#include <ttx_log.h>    /* Provides: TTX_Warning() */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/
#ifdef __KEIL__  /* KeilC 8051 compiler */
#define VTE_SOFTWARE_EMULATOR           (0)     // Define (1) to use software VTE; (0) to use hardware VTE.
#else
#define VTE_SOFTWARE_EMULATOR           (1)     // Define (1) to use software VTE; (0) to use hardware VTE.
#endif


//-----     CD      -----//
#define VTE_CD_NULL_PTN1                (0x5aa5a55a)

#if !VTE_SOFTWARE_EMULATOR
    /* Same register read/write functions as CCL */
#define VTE_RegRead(addr)               _VTE_REG_READ(addr)
#define VTE_RegWrite(addr,val)          _VTE_REG_WRITE(addr,val)
#endif

    /* VTE Log functions */
#define VTE_Log                         TTX_Log

    /* VTE Assertion functions */
#define VTE_REQUIRE(_test)              TTX_REQUIRE(_test)
#define VTE_ENSURE(_test)               TTX_ENSURE(_test)
#define VTE_ALLEGE(_test)               TTX_ALLEGE(_test)
#define VTE_ASSERT_COMPILE(_test)       TTX_ASSERT_COMPILE(_test)

    typedef enum
    {
        VTE_CD_MODE_H84M0  = VTE_CD_CTRL_MODE_H84M0,
        VTE_CD_MODE_H84M1  = VTE_CD_CTRL_MODE_H84M1,
        VTE_CD_MODE_H2418  = VTE_CD_CTRL_MODE_H2418,
        VTE_CD_MODE_ODDPAR = VTE_CD_CTRL_MODE_ODDPAR,
        VTE_CD_MODE_AIT    = VTE_CD_CTRL_MODE_AIT
    } VTE_CDMode_t;


    /* VTE_open() flags */
    typedef enum
    {
        VTE_OPEN_NOP   = 0x00000000u,
        VTE_OPEN_RESET = 0x00000001u
    } VTE_OpenFlag_t;

    /* -- Global typedef Declarations -------------------------------------------*/
    typedef uint32_t           VTE_Data_t;     /* width of data bus of OSD */

    /* Initialization configuration structure */
    typedef struct
    {
        /* Only Writeable control registers here. */
        _VTE_INT_ST_TYPE   intStatus;
    } VTE_Config;



    /* -- Function Prototypes ---------------------------------------------------*/

    bool_t                  VTE_Open                        (const VTE_OpenFlag_t flags);
    bool_t                  VTE_Close                       (void);
    void                    VTE_Reset                       (void);

    bool_t                  VTE_Decode                      (VTE_CDMode_t mode, uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src, uint16_t len);
    bool_t                  VTE_Test                        (void);




#ifdef __cplusplus
}   /* extern "C" */
#endif



#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_VBI_TRANSFER_ENGINE_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of vte.h                                                              *
\*---------------------------------------------------------------------------*/

