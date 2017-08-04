/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_source_pes.h
 * @brief    Digital Video Broadcasting PES stream processor for Teletext.
 * @author   Janus Cheng
 * @date     Feb. 10, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __REALTEK_TELETEXT_SOURCE_DVB_PACKETIZED_ELEMENTARY_STREAM_PROCESSOR_HEADER_INCLUDED__
#define __REALTEK_TELETEXT_SOURCE_DVB_PACKETIZED_ELEMENTARY_STREAM_PROCESSOR_HEADER_INCLUDED__    (1)

#include <ttx_pkg.h>    /* Provides: bool_t and uint8_t */
#include <stdlib.h>     /* Provides: size_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global typedef Declarations -------------------------------------------*/

    /* data_unit_id + data_unit_lengh + data_field
     *   (1 byte)        (1 byte)       (44 bytes)
     */
#define TTX_PES_DATA_PACKAGE_SIZE   (1+1+44)


    typedef enum _PES_VBI_DATA_TYPE
    {
        PES_TT_NON_SUBTITLE = 0x02,
        PES_TT_SUBTITLE     = 0x03,
        PES_TT_INVERTED     = 0xC0,
        PES_VPS             = 0xC3,
        PES_WSS             = 0xC4,
        PES_CLOSED_CAPTION  = 0xC5,
        PES_MONOCHROME      = 0xC6,
        PES_STUFFING        = 0xFF
    } PES_VBI_DATA_TYPE;

    /* -- Public Function Prototypes --------------------------------------------*/

    /* PES stream specific routines */
    bool_t          TTX_SRC_SeekPesHeaderPos    (const uint8_t* pData , const size_t numOfBytes, size_t* pIdx);
    bool_t          TTX_SRC_isPesHeader         (const uint8_t* pData);
    bool_t          TTX_SRC_isPesVbiData        (const uint8_t data_identifier);
    bool_t          TTX_SRC_isPesTtxData        (const uint8_t data_unit_id   );


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_TELETEXT_SOURCE_DVB_PACKETIZED_ELEMENTARY_STREAM_PROCESSOR_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_source_pes.h                                                   *
\*---------------------------------------------------------------------------*/
