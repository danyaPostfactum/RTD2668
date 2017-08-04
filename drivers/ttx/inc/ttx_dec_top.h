/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_decode_top.h
 * @brief    Teletext Table of Pages software decoder routines.
 * @author   Janus Cheng
 * @date     Jan. 23, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_TABLE_OF_PAGES_SERVICE_SOFTWARE_DECODER_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_TABLE_OF_PAGES_SERVICE_SOFTWARE_DECODER_HEADER_INCLUDED__    (1)

#include <ttx_std.h>	/* Provides: TTX_STD_Page_AddrWithSubCode_t */
#include <ttx_obj.h>	/* Provides: TTX_Handle */
#include <ttx_dec.h>	/* Provides: TTX_LopParam_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/

    /* -- Public Function Prototypes --------------------------------------------*/

    void TTX_TOP_Init (void);
    void TTX_TOP_Hide (void);
    bool_t TTX_TOP_Decode (
        const TTX_STD_Page_AddrWithSubCode_t* pShowingPage,
        TTX_TOP_Service_t* pTop, uint8_t* pbSubtitleAvailable,
        TTX_STD_Page_Addr_t* pSubtitlePage, const TTX_LopParam_t* pParam);
    bool_t TTX_FLOF_Decode (
        const TTX_STD_Page_AddrWithSubCode_t* pShowingPage,
        TTX_FLOF_Service_t* pFlof, uint8_t idx);

#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_TABLE_OF_PAGES_SERVICE_SOFTWARE_DECODER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_dec_top.h                                                      *
\*---------------------------------------------------------------------------*/
