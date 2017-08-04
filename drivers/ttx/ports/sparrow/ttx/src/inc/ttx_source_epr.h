/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_source_epr.h
 * @brief    TARA EPR stream processor for Teletext.
 * @author   Janus Cheng
 * @date     Feb. 10, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_SOURCE_TARA_EPR_STREAM_PROCESSOR_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_SOURCE_TARA_EPR_STREAM_PROCESSOR_HEADER_INCLUDED__    (1)

#include <ttx_pkg.h>    /* Provides: bool_t and uint8_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/

    /* LineControl +  LineData + LineInfo
     *   (2 bytes)   (49 bytes)  (13 bytes)
     */
#define TTX_EPR_LINE_SIZE           (2+49+13)



    /* -- Public Function Prototypes --------------------------------------------*/

    /* PES stream specific routines */
    bool_t          TTX_SRC_SeekTtxEprLinePos    (const uint8_t* pData , const size_t numOfBytes, size_t* pIdx);
    bool_t          TTX_SRC_isTtxEprLine         (const uint8_t* pData);


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_SOURCE_TARA_EPR_STREAM_PROCESSOR_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_source_epr.h                                                   *
\*---------------------------------------------------------------------------*/
