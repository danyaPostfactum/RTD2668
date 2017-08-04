/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_cd.c
 * @brief    Teletext software channel decoder for PPR_EMU and VTE_EMU.
 * @details  This is a replacement of hardware channel decoder.
 * @author   Janus Cheng
 * @date     Dec. 29, 2008
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/

#include <ttx_cd.h>         /* Provides: functions to be implemented */
#include <ttx_std.h>        /* Provides: TTX_STD_Hamming84Decode(), TTX_STD_Hamming2418Decode(), TTX_STD_OddParityCheck() and TTX_STD_OddParityDecode(). */
#include <string.h>         /* Provides: memcpy() */

#include <ttx_assert.h>

#define VBIVTE_X26_NULL_PTN1                    (0x00200000)//(0x20200000)
#define CHAR_SPACE                              ' '

TTX_DEFINE_THIS_MODULE( ttx_cd.c )

/* -- Prototypes of Private Functions ---------------------------------------*/
static bool_t           TTX_CD_DecodeHamming84Mode0            (uint8_t* des, const uint8_t* src, const uint32_t numOfBytes);
static bool_t           TTX_CD_DecodeHamming84Mode1            (uint8_t* des, const uint8_t* src, const uint8_t  numOfPackets);
static bool_t           TTX_CD_DecodeHamming2418               (uint8_t* des, const uint8_t* src, const uint8_t  numOfPackets);
static bool_t           TTX_CD_DecodeOddParity                 (uint8_t* des, const uint8_t* src, const uint32_t numOfBytes);
static bool_t           TTX_CD_DecodeAit                       (uint8_t* des, const uint8_t* src, const uint8_t  numOfPackets);


/* -- Implementations of Public Functions -----------------------------------*/

#ifndef __KEIL__  /* KeilC 8051 compiler */

/*---------------------------------------------------------------------------*/
bool_t
TTX_CD_ChannelDecode                   (const uint8_t* src_addr, uint8_t* des_addr, const uint32_t packet_len, const TTX_CD_Mode_t mode)
/*****************************************************************************\
 *  @brief   Emulates the VTE CD Hamming 8/4 Mode 0 decode function.
 *  @param   des         buffer to store decoded data
 *  @param   src         data that is Hamming 8/4 encoded
 *  @param   numOfBytes  the number of bytes that src points to.
 *
 *  @return  decoding status
 *  @retval  TRUE if success
 *  @retval  FALSE if failure
 *
 *  @pre     <ul>
 *              <li>src[0] to src[numOfBytes-1] are the encoded data to be decoded.</li>
 *              <li>des[0] to des[numOfBytes-1] has been allocated properly.</li>
 *           </ul>
 *  @post    After this function, the following vairables will be modified:
 *           <ul>
 *              <li>des[0] to des[numOfBytes-1] are the deocded data.</li>
 *           </ul>
\*****************************************************************************/
{
    bool_t ret = FALSE;
    switch ( mode )
    {
    case TTX_CD_HAMMING_8_4_MODE_0:
        ret = TTX_CD_DecodeHamming84Mode0( des_addr, src_addr, packet_len );
        break;
    case TTX_CD_HAMMING_8_4_MODE_1:
        ret = TTX_CD_DecodeHamming84Mode1( des_addr, src_addr, (uint8_t)(packet_len/40) );
        break;
    case TTX_CD_HAMMING_24_18:
        ret = TTX_CD_DecodeHamming2418( des_addr, src_addr, (uint8_t)(packet_len/40) );
        break;
    case TTX_CD_ODD_PARITY:
        ret = TTX_CD_DecodeOddParity( des_addr, src_addr, packet_len );
        break;
    case TTX_CD_AIT:
        ret = TTX_CD_DecodeAit( des_addr, src_addr, (uint8_t)(packet_len/40) );
        break;

    }
    return( ret );
}


#if( TTX_SUPPORT_DVB )

/*---------------------------------------------------------------------------*/
void
TTX_CD_ReverseBitOrder  (uint8_t* buf, size_t numOfBytes)
/*****************************************************************************\
 *  @brief   Reverse bit-order of buf[0] to buf[numOfBytes-1] and then store
 *             the results in buf[0] to buf[numOfBytes-1].
 *  @details This function reverses the bit order. For example: 12345678 -> 87654321.
 *  @note    Mimic TTUtil_BitOrderSwap().
 *  @param   buf         buffer to store decoded data
 *  @param   numOfBytes  the number of bytes that buf points to.
 *  @pre     buf[0] to src[numOfBytes-1] are the encoded data to be decoded.
 *  @post    After this function, the following vairables will be modified:
 *              buf[0] to buf[numOfBytes-1] are the deocded data.
\*****************************************************************************/
{
    uint8_t temp = 0;
    size_t i = 0;
    uint8_t j;

    if (buf == NULL)
        return;

    while (numOfBytes > 0)
    {
        temp = 0;

        for (j=8; j>0; j--)
        {
            temp = temp + ((buf[i]&1)<<(j-1));
            buf[i] = buf[i] >> 1;
        }

        buf[i] = temp;

        i++;
        numOfBytes--;
    }
}

/*---------------------------------------------------------------------------*/
void
TTX_CD_InvertBits       (uint8_t* buf, size_t numOfBytes)
/*****************************************************************************\
 *  @brief   Reverse bit-order of buf[0] to buf[numOfBytes-1] and then store
 *             the results in buf[0] to buf[numOfBytes-1].
 *  @details This function inverts the bits. For example: 10100000 -> 01011111.
 *  @note    Mimic TTUtil_BitInvert().
 *  @param   buf         buffer to store decoded data
 *  @param   numOfBytes  the number of bytes that buf points to.
 *  @pre     buf[0] to src[numOfBytes-1] are the encoded data to be decoded.
 *  @post    After this function, the following vairables will be modified:
 *              buf[0] to buf[numOfBytes-1] are the deocded data.
\*****************************************************************************/
{
    uint8_t temp;
    size_t i = 0;
    uint8_t j;

    if (buf == NULL)
        return;

    while (numOfBytes > 0)
    {
        temp = 0;

        for (j=0; j<8; j++)
        {
            // Bit is 0, we will turn it into 1.
            if ( ( (buf[i]>>j) &1) == 0)
                temp = temp + (1<<j);
            // Bit is 1, we should turn it into 0, but temp is already 0 at that bit.
        }

        buf[i] = temp;
        i++;
        numOfBytes--;
    }
}

#endif  /* end of #if( TTX_SUPPORT_DVB ) */

/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*/
static bool_t
TTX_CD_DecodeHamming84Mode0            (uint8_t* des, const uint8_t* src, const uint32_t numOfBytes)
/*****************************************************************************\
 *  @brief   Emulates the VTE CD Hamming 8/4 Mode 0 decode function.
 *  @param   des         buffer to store decoded data
 *  @param   src         data that is Hamming 8/4 encoded
 *  @param   numOfBytes  the number of bytes that src points to.
 *
 *  @return  decoding status
 *  @retval  TRUE if success
 *  @retval  FALSE if failure
 *
 *  @pre     <ul>
 *              <li>src[0] to src[numOfBytes-1] are the encoded data to be decoded.</li>
 *              <li>des[0] to des[numOfBytes-1] has been allocated properly.</li>
 *           </ul>
 *  @post    After this function, the following vairables will be modified:
 *           <ul>
 *              <li>des[0] to des[numOfBytes-1] are the deocded data.</li>
 *           </ul>
\*****************************************************************************/
{
    if ( des && src )
    {
        uint32_t i;
        bool_t success = TRUE;

        for ( i = 0 ; i < numOfBytes ; i++ )
        {
            if ( ! TTX_STD_Hamming84Decode( des + i, *(src + i) ) )
            {
                success = FALSE;
            }
        }
        return( success );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_CD_DecodeHamming84Mode1           (uint8_t* des, const uint8_t* src, const uint8_t  numOfPackets)
/*****************************************************************************\
 *  @brief   Emulates the VTE CD Hamming 8/4 Mode 1 decode function.
 *  @param   des           buffer to store decoded data
 *  @param   src           data that is Hamming 8/4 encoded
 *  @param   numOfPackets  the number of packets (40 bytes) that src points to.
 *
 *  @return  decoding status
 *  @retval  true if success
 *  @retval  false if failure
 *
 *  @pre     <ul>
 *              <li>src[0] to src[40*numOfPackets] are the encoded data to be decoded.</li>
 *              <li>des[0] to des[40*numOfPackets] has been allocated properly.</li>
 *           </ul>
 *  @post    After this function, the following vairables will be modified:
 *           <ul>
 *              <li>des[0] to des[40*numOfPackets] are the deocded data.</li>
 *           </ul>
\*****************************************************************************/
{
    const uint8_t PKT_SIZE = 40;  /* bytes */

    if ( des && src )
    {
        uint8_t i, j;
        bool_t success = TRUE;

        for ( i=0; i<numOfPackets ; i++ )
        {
            /* byte-0, byte-38 and byte-39 of *des remain the same as that of *src */
            des[PKT_SIZE*i+ 0] = src[PKT_SIZE*i+ 0];  /* byte-0  */
            des[PKT_SIZE*i+38] = src[PKT_SIZE*i+38];  /* byte-38 */
            des[PKT_SIZE*i+39] = src[PKT_SIZE*i+39];  /* byte-39 */

            /* decode byte-1 to byte-37 of *src to *des */
            for ( j = 1 ; j <= 37 ; j++ )
            {
                if ( ! TTX_STD_Hamming84Decode( des + PKT_SIZE*i + j, *(src + PKT_SIZE*i + j) ) )
                {
                    success = FALSE;
                }
            }
        }
        return( success );
    }

    return( FALSE );
}

#ifndef __KEIL__  /* KeilC 8051 compiler */
/*---------------------------------------------------------------------------*/
static bool_t
TTX_CD_DecodeHamming2418               (uint8_t* des, const uint8_t* src, const uint8_t  numOfPackets)
/*****************************************************************************\
 *  @brief   Emulates the VTE CD Hamming 24/18 decode function.
 *  @details Use this function to emulate the VTE CD Hamming 24/18 decode function.
 *  @param   des            points to the destination to store the decoded *src
 *  @param   src            points to the source Hamming 24/18 encoded data
 *  @param   numOfPackets   number of 40-byte (packet) data to be decoded
 *
 *  @return  the decode status
 *  @retval  true  success
 *  @retval  false failure
 *
 *  @ingroup Tim
 *  @see     TTApi_DeInit()
 *  @todo
 *           nothing to do.
\*****************************************************************************/
{
    const uint8_t PKT_SIZE = 40;  /* bytes */

    if ( des && src )
    {
        uint8_t i, j;

        bool_t success = TRUE;

        for ( i=0; i<numOfPackets ; i++ )
        {
            memcpy( des + PKT_SIZE*i + 0, src + PKT_SIZE*i + 0, 1 );  /* byte-0 remain the same, because Hamming 8/4 has been done by PPR */
            //success = Hamming84Decode(des + PKT_SIZE*i + 0, src[PKT_SIZE*i + 0]);

            for ( j=1; j<PKT_SIZE; j+=3 )
            {
                if ( ! TTX_STD_Hamming2418Decode( des + PKT_SIZE*i + j, src + PKT_SIZE*i + j ) )
                {
                    // To emulate the error handling behavior of RTD2638.
                    //   Mark des[] as error by using VBIVTE_X26_NULL_PTN1.
                    {
                        if ( j==1 )
                        {
                            des[PKT_SIZE*i+0] = 0;
                        }
                        des[PKT_SIZE*i+j+0] = (uint8_t)(((VBIVTE_X26_NULL_PTN1)&0x003FFFFF)>>16);
                        des[PKT_SIZE*i+j+1] = (uint8_t)(((VBIVTE_X26_NULL_PTN1)&0x003FFFFF)>> 8);
                        des[PKT_SIZE*i+j+2] = (uint8_t)(((VBIVTE_X26_NULL_PTN1)&0x003FFFFF)>> 0);
                    }

                    success = FALSE;
                }
            }
        }
        return( success );
    }

    return( FALSE );
}

#endif
/*---------------------------------------------------------------------------*/
static bool_t
TTX_CD_DecodeOddParity                 (uint8_t* des, const uint8_t* src, const uint32_t numOfBytes)
/*****************************************************************************\
 *  @brief   Emulates the VTE CD odd-parity decode function.
 *  @details Use this function to emulate the VTE CD odd-parity decode function.
 *  @param   des            des[0] to des[numOfBytes-1] destination to store the decoded *src
 *  @param   src            src[0] to src[numOfBytes-1] are the odd-parity encoded data
 *  @param   numOfBytes     number of bytes to be decoded
 *
 *  @return  the decode status
 *  @retval  true  success
 *  @retval  false failure
 *
 *  @ingroup Tim
 *  @see     TTApi_DeInit()
 *  @todo
 *           nothing to do.
\*****************************************************************************/
{
    if ( des && src )
    {
        uint32_t i;
        bool_t success = TRUE;

        for ( i = 0 ; i < numOfBytes ; i++ )
        {
            if ( TTX_STD_OddParityCheck(*(src+i) ) )
            {
                *(des + i) = TTX_STD_OddParityDecode( *(src+i) );
            }
            else
            {
                *(des + i) = CHAR_SPACE;
                success = FALSE;
            }
        }

        return( success );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_CD_DecodeAit           (uint8_t* des, const uint8_t* src, const uint8_t  numOfPackets)
/*****************************************************************************\
 *  @brief   Emulates the VTE CD Additional Information Table special decode data.
 *  @details Use this function to emulate the VTE CD Additional Information Table special decode data.
 *  @param   des            points to the destination to store the decoded *src
 *  @param   src            points to the source AIT encoded data
 *  @param   numOfPackets   number of 40-byte (packet) data to be decoded
 *
 *  @return  the decode status
 *  @retval  true  success
 *  @retval  false failure
 *
 *  @ingroup Tim
 *  @see     TTApi_DeInit()
 *  @todo
 *           nothing to do.
\*****************************************************************************/
{
    const uint8_t PKT_SIZE = 40;  /* bytes */
    bool_t success = TRUE;

    if ( des && src )
    {
        uint8_t i, j;

        for ( i=0; i<numOfPackets ; i++ )
        {
            /* byte 0 to 7 are hamming 8/4 encoded. */
            for (j=0; j<8; j++)
            {
                if ( TTX_STD_Hamming84Decode( des + i*PKT_SIZE + j, *(src + i*PKT_SIZE + j) ) != TRUE )
                {
                    success = FALSE;
                }
            }

            /* byte 8 to 19 are odd parity encoded. */
            for (j=8; j<20; j++)
            {
                if ( TTX_STD_OddParityCheck( *(src + i*PKT_SIZE + j) ) )
                {
                    *(des + i*PKT_SIZE + j) = TTX_STD_OddParityDecode( *(src + i*PKT_SIZE + j) );
                    success = TRUE;
                }
                else
                {
                    success = FALSE;
                }
            }

            /* byte 20 to 27 are hamming 8/4 encoded. */
            for (j=20; j<28; j++)
            {
                if ( TTX_STD_Hamming84Decode( des + i*PKT_SIZE + j, *(src + i*PKT_SIZE + j) ) != TRUE )
                {
                    success = FALSE;
                }
            }

            /* byte 28 to 39 are odd parity encoded. */
            for (j=28; j<40; j++)
            {
                if ( TTX_STD_OddParityCheck( *(src + i*PKT_SIZE + j) ) )
                {
                    *(des + i*PKT_SIZE + j) = TTX_STD_OddParityDecode( *(src + i*PKT_SIZE + j) );
                    success = TRUE;
                }
                else
                {
                    success = FALSE;
                }
            }

        }
        return( success );
    }

    return( FALSE );

}

#endif  /* end of #ifndef __KEIL__ */

/*---------------------------------------------------------------------------*\
 * end of ttx_cd.c                                                           *
\*---------------------------------------------------------------------------*/
