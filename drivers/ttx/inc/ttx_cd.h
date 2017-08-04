/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_cd.h
 * @brief    Internal (package scope) shared interfaces for ppr_emu.c and vte_emu.c.
 * @author   Janus Cheng
 * @date     Dec. 29, 2008
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_SOFTWARE_CHANNEL_DECODER_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_SOFTWARE_CHANNEL_DECODER_HEADER_INCLUDED__ (1)

#include <ttx_pkg.h>    /* Provides: bool_t and uint8_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/
    typedef enum TTX_CD_Mode_t
    {
        TTX_CD_HAMMING_8_4_MODE_0,
        TTX_CD_HAMMING_8_4_MODE_1,
        TTX_CD_HAMMING_24_18,
        TTX_CD_ODD_PARITY,
        TTX_CD_AIT
    } TTX_CD_Mode_t;

    /* -- Public Function Prototypes --------------------------------------------*/


    bool_t           TTX_CD_ChannelDecode       (const uint8_t* src_addr, uint8_t* des_addr, const uint32_t packet_len, const TTX_CD_Mode_t mode);


#if TTX_SUPPORT_DVB /* These functions will only be called when reading TTX data from DVB */

    void             TTX_CD_ReverseBitOrder     (      uint8_t* buf     , size_t numOfBytes);
    void             TTX_CD_InvertBits          (      uint8_t* buf     , size_t numOfBytes);

#endif  /* end of #if TTX_SUPPORT_DVB */



#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_SOFTWARE_CHANNEL_DECODER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_cd.h                                                           *
\*---------------------------------------------------------------------------*/
