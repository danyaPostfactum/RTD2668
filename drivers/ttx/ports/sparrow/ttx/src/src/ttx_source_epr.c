/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_source_epr.c
 * @brief    TARA EPR stream processor for TTX_SRC module.
 * @author   Janus Cheng
 * @date     Feb. 10, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ttx_source_epr.h> /* Provides: functions to be implemented */
#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */

#include <stdlib.h>         /* Provides: NULL */


TTX_DEFINE_THIS_MODULE( ttx_source_epr.c )



/* -- Prototypes of Private Functions ---------------------------------------*/





/* -- Implementations of Public Functions -----------------------------------*/



/*---------------------------------------------------------------------------*/
bool_t
TTX_SRC_SeekTtxEprLinePos   (const uint8_t* pData , const size_t numOfBytes, size_t* pIdx)
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

    const size_t headerLen = 2+2+1; // bytes: LineCtrl (2 bytes) + ClockRunIn (2 bytes) + FramingCode (1 byte)

    // Teletext EprLine header is headerLen bytes.
    if ( numOfBytes < headerLen )
        return( TRUE );    // impossible for pData[0] to pData[numOfBytes-1] contain a Teletext EprLine header.

    while ( count <= numOfBytes-headerLen )
    {
        if ( TTX_SRC_isTtxEprLine( pData+count ) )   // found
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
TTX_SRC_isTtxEprLine (const uint8_t* pEprLine)
/*****************************************************************************\
 *  @brief    This function checks if the input buffer is PES header.
 *  @note     Mimic xIsPesHeader().
\*****************************************************************************/
{
    // If this is a control line
    if ( (pEprLine[0] == 0)&&!(pEprLine[1] &0x07 ) )
    {
        return( FALSE );
    }

    // If this is a TT packet
    if (   (pEprLine[2] == 0x55)
            &&(pEprLine[3] == 0x55)
            &&(pEprLine[4] == 0x27) )
    {
        return( TRUE );
    }

    return( FALSE );
}









/* -- Implementations of Private Functions ----------------------------------*/










/*---------------------------------------------------------------------------*\
 * end of ttx_source_epr.c                                                   *
\*---------------------------------------------------------------------------*/

