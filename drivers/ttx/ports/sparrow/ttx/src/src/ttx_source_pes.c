/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_source_pes.c
 * @brief    DVB stream processor for TTX_SRC module.
 * @author   Janus Cheng
 * @date     Feb. 10, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ttx_source_pes.h> /* Provides: functions to be implemented */
#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */

#include <stdlib.h>         /* Provides: NULL */


TTX_DEFINE_THIS_MODULE( ttx_source_pes.c )



/* -- Prototypes of Private Functions ---------------------------------------*/





/* -- Implementations of Public Functions -----------------------------------*/



/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_SeekPesHeaderPos   (const uint8_t* pData , const size_t numOfBytes, size_t* pIdx)
/*****************************************************************************\
 *  @brief    This function trys to set the location of the PES header
 *              from input stream.
 *  @note     xSeekPesHeaderPos().
 *  @return   Stop status of current seek operation.
 *  @retval   TRUE  if there's NO need to call this function again.
 *  @retval   FALSE if it requires to call this function again.
 *  @param    pIdx  The current seek position.
 *                    If it found a PES header, pData[*pIdx] is the starting position of PES header.
 *                    Else, *pIdx will be set to the end of *pData[].
\*****************************************************************************/
{
    size_t count = 0;

    // PES header is 4 bytes.
    if ( numOfBytes < 4 )
        return( TRUE );    // impossible for pData[0] to pData[numOfBytes-1] contain a PES header.

    while ( count <= numOfBytes-4 )
    {
        if ( TTX_SRC_isPesHeader( pData+count ) )   // found
        {
            if ( pIdx != NULL )
            {
                *pIdx = count;
            }
            return( FALSE );
        }

        count++;
    }

    if ( pIdx != NULL )
    {
        *pIdx = count - 1;
    }
    return( FALSE );    // not found
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_isPesHeader (const uint8_t* pData)
/*****************************************************************************\
 *  @brief    This function checks if the input buffer is PES header.
 *  @note     Mimic xIsPesHeader().
\*****************************************************************************/
{
    // pData[0]~pData[2] is packet_start_code_prefix
    if ( (pData[0] == 0x00) &&
            (pData[1] == 0x00) &&
            (pData[2] == 0x01) )
    {
        const uint8_t private_stream_1 = 0xBD;

        // pData[3] is stream_id
        if ( pData[3] == private_stream_1 )
        {
            return( TRUE );
        }
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_isPesVbiData    (const uint8_t data_identifier)
/*****************************************************************************\
 *  @brief    This function checks if the input DTV data identifier is VBI data.
 *  @note     DTV only.
 *  @note     Mimic xIsVbiData().
\*****************************************************************************/
{
    // VBI data has id between 0x10~0x1F and 0x99~0x9B
    if ( (data_identifier >= 0x10) && (data_identifier <= 0x1F) )
        return( TRUE );

    if ( (data_identifier >= 0x99) && (data_identifier <= 0x9B) )
        return( TRUE );

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_isPesTtxData    (const uint8_t data_unit_id)
/*****************************************************************************\
 *  @brief    This function checks if the input DTV data unit Id is teletext data.
 *  @note     DTV only.
 *  @note     Mimic xIsTeletextData().
\*****************************************************************************/
{
    if (   (data_unit_id == PES_TT_NON_SUBTITLE ) \
            ||(data_unit_id == PES_TT_SUBTITLE     ) \
            ||(data_unit_id == PES_TT_INVERTED     ) )
    {
        return( TRUE );
    }

    return( FALSE );
}








/* -- Implementations of Private Functions ----------------------------------*/










/*---------------------------------------------------------------------------*\
 * end of ttx_source_pes.c                                                   *
\*---------------------------------------------------------------------------*/

