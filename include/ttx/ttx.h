/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx.h
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
#ifndef __REALTEK_ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_INDEPENDENT_PUBLIC_INTERFACE_HEADER_INCLUDED__
#define __REALTEK_ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_INDEPENDENT_PUBLIC_INTERFACE_HEADER_INCLUDED__

#include <ttx_pkg.h>
#include <ttx_std.h>        /* Provides: TTX_STD_Page_AddrWithSubCode_t */
#include <ttx_evt.h>        /* Provides: TTX_EVTS_TYPES */
#include <ttx_obj.h>        /* Provides: TTX_Handle and TTX_FLOF_KEY_NUM */

#ifdef __cplusplus
extern "C"
{
#endif

    /*--- TTX Configurations ----------------------------------------------------*/

    /*--- TTX Constants ---------------------------------------------------------*/



    typedef union TTX_CmdArg_t
    {
        /* for TTX_CMD_GO_TO_PAGE,
         *     TTX_CMD_SET_SUBTITLE_PAGE_NUM,
         *     TTX_CMD_SET_PAGE_NUM_INDICATOR
         */
        struct
        {
            uint8_t magNum;
            uint8_t pageNumTens;
            uint8_t pageNumUnits;
        } pageNum;

        /* for
         *     TTX_CMD_GO_TO_SUBPAGE
         */
        TTX_STD_Page_AddrWithSubCode_t pageAddr;

    } TTX_CmdArg_t;

    typedef enum TTX_CallbackEvt_t
    {
        TTX_CALLBACK_ON_ENTERED_NORMAL,
        TTX_CALLBACK_ON_ENTERED_SUBTITLE,
        TTX_CALLBACK_ON_STOPPED
    } TTX_CallbackEvt_t;

    typedef union TTX_CallbackEvtArgs_t
    {
        int dummy;
    } TTX_CallbackEvtArgs_t;

    /** Initialze TTX library. Must be called before any other TTX function calls. */
    void                          TTX_Open                    (void);

    /** Clean up TTX library. Must be called if you no longer need TTX library. */
    void                              TTX_Close                   ();

    void                                TTX_Reset                   ();

    void                                TTX_Main                    (void);

    bool_t 				   TTX_SetInitPage (const TTX_STD_Page_AddrWithSubCode_t* pageInit);
    bool_t 				   TTX_GetInitPage (TTX_STD_Page_AddrWithSubCode_t* pageInit);

    bool_t                              TTX_Control                 ( TTX_Cmd_t cmd);
    bool_t                              TTX_Control1                 ( TTX_Cmd_t cmd, TTX_CmdArg_t* pArg);

    void                                TTX_Tick                    ();

    bool_t                              TTX_WaitForCompletion       ();

    void                                TTX_Callback                (const TTX_CallbackEvt_t evt, TTX_CallbackEvtArgs_t* pArg);

    bool_t                              TTX_GetStatus               (const  TTX_Status_t* pStatus);

    /** Perform built-in test of TTX library. It returns TRUE if test OK; FALSE otherwise. */
    bool_t                              TTX_Test                    ();



    void                                TTX_TestSoftware            (void); // test programming errors


    /** Dump contents of memory. */
    void                                TTX_DumpMem                 (const void* addr, uint32_t numOfBytes);

    /*****************************************************************************/
    /** @brief obtain the current TTX version number string
    *
    * @return version of the TTX as a constant 6-character string of the form
    * x.y.zz, where x is a 1-digit major version number, y is a 1-digit minor
    * version number, and zz is a 2-digit release number.
    */
    char const TTX_ROM * TTX_ROM_VAR    TTX_GetVersion              (void);


    extern TTX_Handle xdata m_hTtx;

#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !REALTEK_ENHANCED_TELETEXT_DECODER_LIBRARY_PLATFORM_INDEPENDENT_PUBLIC_INTERFACE_HEADER_INCLUDED */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx.h                                                              *
\*---------------------------------------------------------------------------*/
