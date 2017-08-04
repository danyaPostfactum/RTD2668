/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_dec.c
 * @brief    Teletext software decoder.
 * @author   Janus Cheng
 * @date     Jan. 23, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_dec.h>    /* Provides: functions to be implemented */
#include <ttx_log.h>    /* Provides: TTX_Log(LOGGER_VERBOSE, ) */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */
#include <ttx_buff.h>   /* Provides: TTX_BUF_DecodeNormalPage() */
#include <ttx_disp.h>   /* Provides: TTX_DISP_ToOsdAttrMem() and TTX_DISP_SUBPAGE_MAX_NUM */
#include <ttx_std.h>    /* Provides: TTX_STD_Test() */

#include <string.h>     /* Provides: memcpy() */


TTX_DEFINE_THIS_MODULE( ttx_dec.c )

#define PKT_BUF_SIZE_BYTES  (40)  // size of field buffer containing descrambeld packet data (including packet address)

TTX_ASSERT_COMPILE( PKT_BUF_SIZE_BYTES >= TTX_STD_PACKET_PAYLOAD_SIZE_BYTES );  // ERROR: Insufficient PKT_BUF_SIZE_BYTES.


TTX_X26Info_t xdata ttx_x26_info; // X/26 decoded data




/* -- Prototypes of Private Functions ---------------------------------------*/
static bool_t TTX_InitLopParam (
    TTX_LopParam_t* pParam, TTX_STD_MagazineNum_t* pMag,
    const uint8_t TTX_MEM* src, const bool_t bApplyDISP);

static bool_t TTX_PacketParse (
    TTX_STD_Packet_t* pPkt, const uint8_t src[PKT_BUF_SIZE_BYTES] );
// Parse src[0]~src[41] to TTX_STD_Packet_t

static bool_t TTX_PacketX0Parse (
    TTX_STD_PacketX0_t* pPkt, const uint8_t src[PKT_BUF_SIZE_BYTES] );
// Parse src[0]~src[41] to TTX_STD_PacketX0_t

static bool_t TTX_DecodeLine (
    TTX_DISP_Attr_t dest[TTX_DISP_MAX_ROW_LEN], TTX_STD_Packet_t* pPkt,
    const TTX_LopParam_t* pParam, uint8_t* pIgnoreLine, const bool_t rolling );



/* -- Implementations of Public Functions -----------------------------------*/


/*---------------------------------------------------------------------------*/
bool_t TTX_DecodePage (uint8_t idx, TTX_STD_Page_CtrlBits_t ctrlBits, TTX_LopParam_t *pParam)
/*****************************************************************************\
 * @brief		Decode a page specified by idx to background attribute memory.
 * @param	<idx>		page index
 * @param	<ctrlBits>	control bits from X/0
 * @param	<pParam>	parameters of LOP (LOP means "level one page",
 *						but now this structure contains whole pages parameters.)
 * @post		TTX_DEFAULT_CD_PAGE_ADDRESS will be modified.
 * @see		OSDEMU_DecodeRow0().
\*****************************************************************************/
{
    const uint8_t startRow = 0;
    const uint8_t endRow = 23;

    uint8_t row, ignoreLine;
    uint8_t i;
    //TTX_LopParam_t param; // move to TTX_EVT_onDecodingBufferedPage()
    TTX_STD_Packet_t pkt;
    TTX_DISP_Attr_t decodedDispAttr[TTX_DISP_MAX_ROW_LEN];  /* stores decoded display attribute of an entire row. */
    uint8_t TTX_MEM* src = TTX_BUF_GetMemByIdx(idx);

    //TTX_REQUIRE( startRow <= endRow );

    // Channel decode src page to "cd mem"
    TTX_BUF_DecodeNormalPage( idx ); // decode X/0 ~ X/23 and X/26/0 ~ X/26/14

#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
    // @todo Port level 2.5 functions here.
#endif  /* end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25) */

    /* Initialize param from information provided by src[] and startRow */
    TTX_InitLopParam( pParam, &(pkt.addr.magNum), src, TRUE );  // Each packets within the same page has the same magazine-number.

    ignoreLine = 0xFF;  // do not ignore any lines

#if 0
    if ( (startRow == 0) && (endRow == TTX_DISP_LAST_ROW) )
    {
        TTX_DISP_ZeroAttrMem(); // Zero background attribute memory
    }
#endif

    for ( row = startRow; row <= endRow; row++ )
    {
        flow_timer_Reset_WatchDog();

        if ( (ctrlBits.bInhibitDisplay) && (row>=1) )   // inhibit display: clear X/1 ~ X/23
        {
            memset( decodedDispAttr, 0, sizeof(decodedDispAttr) );
            TTX_DISP_CopyToAttrMem( decodedDispAttr, row );
            continue;
        }

        // Set Default Value of Attribute
        if ( ignoreLine == 0xFF )
        {
            TTX_REQUIRE( sizeof(decodedDispAttr) == TTX_DISP_MAX_ROW_LEN * sizeof(TTX_DISP_Attr_t) );
            memset(decodedDispAttr, 0, sizeof(decodedDispAttr) );
            for ( i=0; i<TTX_DISP_MAX_ROW_LEN; i++ )
            {
                decodedDispAttr[i].fgColorIdx  = TTX_COLOR_WHITE;
                decodedDispAttr[i].charIdx     = 0x10;
                decodedDispAttr[i].charset     = pParam->DEFAULT_G0_CHARSET;
            }
        }

        pkt.addr.pktNum = row;  // packet number equals row number

        // Get decoded packet from "cd mem" and then parse it.
        TTX_PacketParse( &pkt, TTX_BUF_GetDecodedPacket( row ) );

        // Decode the pkt to decodedDispAttr[].
        TTX_DecodeLine( decodedDispAttr, &pkt, pParam, &ignoreLine, FALSE );

        /* Put attributes directly regardless of TTX_DecodeLine()
        {
            int kkk;
            TTX_DISP_Attr_t attr;
            attr.charIdx  = 'A';
            attr.fgColorIdx = TTX_COLOR_BLUE;
            attr.bgColorIdx = TTX_COLOR_GREEN;
            attr.box      = FALSE;
            attr.graphic  = FALSE;
            attr.conceal  = FALSE;
            attr.charset  = param.DEFAULT_G0_CHARSET;
            attr.flashMode = TTX_FLASH_STEADY;
            attr.size     = TTX_STD_SIZE_CFG_NORMAL;
            //attr.ul       = FALSE;
            attr.dmIdx     = 0;
            attr.mosaics   = TTX_MOSAICS_CON;

            for( kkk=0; kkk<TTX_DISP_MAX_ROW_LEN; kkk++ )
            {
                decodedDispAttr[kkk] = attr;
                attr.charIdx++;
            }

        //    TTX_DISP_CopyToAttrMem( attrMem, decodedDispAttr, 1 );
        }*/

        // Copy decoded decodedDispAttr[] to row-th row of attrMem.
        TTX_DISP_CopyToAttrMem( decodedDispAttr, row );
    }


    return( TRUE );
}


/*---------------------------------------------------------------------------*/
bool_t TTX_DecodeX24 (uint8_t idx, const TTX_LopParam_t* pParam)
/*****************************************************************************\
 * @brief		Decode FLOF in this page.
 * @param	<idx>		page index
 * @post		TTX_DEFAULT_CD_PAGE_ADDRESS will be modified.
 * @see		OSDEMU_DecodeRow0().
 * @note		Channel decode is the key point of this function
 *			because we don't need to decode X/26 when we decode X/24.
\*****************************************************************************/
{
    uint8_t i;
    uint8_t ignoreLine = 0xFF; // don't ignore X/24
    //TTX_LopParam_t param; // from TTX_DecodePage()
    TTX_STD_Packet_t pkt;
    TTX_DISP_Attr_t decodedDispAttr[TTX_DISP_MAX_ROW_LEN];  // stores decoded display attribute of an entire row.
    uint8_t TTX_MEM* src = TTX_BUF_GetMemByIdx(idx);

    // Channel decode src page to "cd mem"
    TTX_BUF_DecodeX24( idx );

#if 0 // from TTX_DecodePage()
    /* Initialize param from information provided by src[] and startRow */
    TTX_InitLopParam( &param, &(pkt.addr.magNum), src, 24 );  // Each packets within the same page has the same magazine-number.
#endif

    // Set Default Value of Attribute
    TTX_REQUIRE( sizeof(decodedDispAttr) == TTX_DISP_MAX_ROW_LEN * sizeof(TTX_DISP_Attr_t) );
    memset(decodedDispAttr, 0, sizeof(decodedDispAttr));
    for ( i=0; i<TTX_DISP_MAX_ROW_LEN; i++ )
    {
        decodedDispAttr[i].fgColorIdx  = TTX_COLOR_WHITE;
        decodedDispAttr[i].charIdx     = 0x10;
        decodedDispAttr[i].charset     = pParam->DEFAULT_G0_CHARSET;
    }

    pkt.addr.pktNum = 24;  // packet number equals row number

    // Get decoded packet from "cd mem" and then parse it.
    TTX_PacketParse( &pkt, TTX_BUF_GetDecodedPacket( pkt.addr.pktNum ) );

    // Decode the pkt to decodedDispAttr[].
    TTX_DecodeLine( decodedDispAttr, &pkt, pParam, &ignoreLine, FALSE );

    // Copy decoded decodedDispAttr[] to row-th row of attrMem.
    TTX_DISP_CopyToAttrMem( decodedDispAttr, pkt.addr.pktNum );

    return( TRUE );
}



/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*/
static bool_t TTX_InitLopParam	(TTX_LopParam_t* pParam, TTX_STD_MagazineNum_t* pMag,
                                const uint8_t TTX_MEM* src, const bool_t bApplyDISP)
/*****************************************************************************\
 *  @brief   Initialize a TTX_LopParam_t for decoding.
 *  @param   *pParam will the initialized param.
 *  @param   *pMag is the magazine number.
 *  @param   src[0]~[39]  is the channel decoded data of rowNum.
 *  @post    *pParam will be updated.
 *  @return  the initialization result.
 *  @see     OSDEMU_InitLopParam()
\*****************************************************************************/
{
#if 1
    TTX_STD_PacketX0_t pktX0;
    TTX_STD_CharSet_t DEFAULT_G0_CHARSET, SECOND_G0_CHARSET, G2_CHARSET;

    if ( (pParam == NULL) || (pMag == NULL) || (src == NULL) )
        return( FALSE );

    // Parse src[] and update packet X/0
    TTX_PacketX0Parse( &pktX0, src );

    if ( bApplyDISP )
    {
        if ( pktX0.ctrlBits.bSuppressHeader )
        {
            TTX_DISP_EnableTimecode( FALSE );   // Don't display timecode
        }
        else
        {
            TTX_DISP_EnableTimecode( TRUE );    // Display timecode
        }
    }

    // Decide character set.
    //     Here, we set default character sets by national option.
    //     Character sets could be modified by X/28/0 Format 1 later.

    TTX_STD_GetDefaultG0G2FromNatlOpt(
        (pktX0.ctrlBits.natlOpt & 0x7), &DEFAULT_G0_CHARSET, &G2_CHARSET );
    if ( DEFAULT_G0_CHARSET == TTX_CHARSET_RESERVED )
        DEFAULT_G0_CHARSET = TTX_CHARSET_G0_LATIN;
    if ( G2_CHARSET == TTX_CHARSET_RESERVED )
        G2_CHARSET = TTX_CHARSET_G2_LATIN;

    TTX_STD_GetSecondG0( &SECOND_G0_CHARSET );
    if ( SECOND_G0_CHARSET == TTX_CHARSET_RESERVED )
        SECOND_G0_CHARSET = TTX_CHARSET_G0_LATIN;

    pParam->DEFAULT_G0_CHARSET = (DEFAULT_G0_CHARSET & 0x1F); // default G0
    pParam->SECOND_G0_CHARSET = (SECOND_G0_CHARSET & 0x1F); // second G0
    pParam->G1_CHARSET = (TTX_CHARSET_G1_MOSAICS & 0x1F); // G1
    pParam->G2_CHARSET = (G2_CHARSET & 0x1F); // G2
    pParam->C12C13C14 = (pktX0.ctrlBits.natlOpt & 0x7); // national option

    // L1.5 or L2.5
    pParam->Alpha_Black_Enable = 0x1; // enable Alpha black
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
    pParam->Double_Width_Enable = 0x1; // enable double width
    pParam->Double_Size_Enable = 0x1; // enable double size
#else
    pParam->Double_Width_Enable = 0x0; // disable double width
    pParam->Double_Size_Enable = 0x0; // disable double size (replaced by double height)
#endif
    pParam->ESC_Enable = 0x1; // enable ESC
    pParam->MOSAICS_BLACK_Enable = 0x1; // enable mosaics black
    pParam->NewBackground_Enable = 0x1; // set new background enable

    // enable Subtitle or Newflash
    if ( pktX0.ctrlBits.bNewsFlash || pktX0.ctrlBits.bSubtitle )
    {
        pParam->Newsflash_Subtitle = 0x1;
        if ( bApplyDISP )
            TTX_DISP_EnableBox( TRUE );
    }
    else
    {
        pParam->Newsflash_Subtitle = 0x0;
        if ( bApplyDISP )
            TTX_DISP_EnableBox( FALSE );
    }

    // color related settings
    pParam->Default_BGC = TTX_COLOR_BLACK; // set background color as black
    pParam->FG_CLUT = 0x0; // default color look-up table
    pParam->BG_CLUT = 0x0; // default color look-up table
    pParam->REMAP_CLUT = 0x0; // default color look-up table

    return( TRUE );
#else
    //uint8_t             Mode;
    TTX_STD_PacketX0_t  pktX0;
    uint8_t             c12c13c14, default_G0G2_Char_Dcode, second_G0_Char_Dcode;
    TTX_STD_CharSet_t   DEFAULT_G0_CHARSET,SECOND_G0_CHARSET;
    uint32_t            word_data = 0;

    // set Micronas like(0) / Philips like(1) for DS/DH/DW
    word_data |= TTX_SIZE_ATTR_MODE_PHILIPS << 1;

    // set X/0 & X/24 box enable
    word_data |= UINT32_C(0x01) << 29;

    // set X/24 enable
    word_data |= UINT32_C(0x00) << 28; //timhuang@20061103: for auto update

    // set new background enable
    word_data |= UINT32_C(0x01) << 26;

    // set esc enable
    word_data |= UINT32_C(0x01) << 25;

    // set Alpha black enable
    word_data |= UINT32_C(0x01) << 24;

    // set double width
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
    word_data |= UINT32_C(0x01) << 23;
#else
    word_data |= UINT32_C(0x00) << 23;
#endif

    // set double size
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
    word_data |= UINT32_C(0x01) << 22;
#else
    word_data |= UINT32_C(0x00) << 22;
#endif

    // set mosaic black
    word_data |= UINT32_C(0x01) << 21;

    // set G1
    word_data|= ((uint32_t)TTX_CHARSET_G1_MOSAICS) << 13;

    if ( rowNum == 0 )
    {
        // Parse src[] and update packet X/0
        TTX_PacketX0Parse( &pktX0, src );

        if ( pktX0.ctrlBits.bSuppressHeader )
        {
            word_data|= UINT32_C(0x01) << 30;
            TTX_DISP_EnableTimecode( FALSE );   // Don't display timecode
        }
        else
        {
            TTX_DISP_EnableTimecode( TRUE );    // Display timecode
        }

        // set subtitle/newflash enable
        if ( pktX0.ctrlBits.bNewsFlash || pktX0.ctrlBits.bSubtitle )
        {
            word_data|= UINT32_C(0x01) << 27;
            TTX_DISP_EnableBox( TRUE );
        }
        else
        {
            TTX_DISP_EnableBox( FALSE );
        }

        //  Set VBI inhibit enable
        if ( pktX0.ctrlBits.bInhibitDisplay )
            word_data |= UINT32_C(1)<<12;

        c12c13c14 = pktX0.ctrlBits.natlOpt;
        default_G0G2_Char_Dcode = second_G0_Char_Dcode = 0;

        TTX_STD_GetCharSetFromDefaultG0G2( default_G0G2_Char_Dcode, c12c13c14, &DEFAULT_G0_CHARSET );
        if ( DEFAULT_G0_CHARSET == TTX_CHARSET_RESERVED )
            DEFAULT_G0_CHARSET = TTX_CHARSET_G0_LATIN;

        TTX_STD_GetCharSetFromDefaultG0G2( second_G0_Char_Dcode, c12c13c14, &SECOND_G0_CHARSET );
        if ( SECOND_G0_CHARSET == TTX_CHARSET_RESERVED )
            SECOND_G0_CHARSET = TTX_CHARSET_G0_LATIN;

        // set national option
        word_data |= ((uint32_t)c12c13c14) << 18;

        // set default G0
        word_data |= ((uint32_t)DEFAULT_G0_CHARSET) << 3;

        // set second G0
        word_data |= ((uint32_t)SECOND_G0_CHARSET) << 8;

        if ( pMag != NULL )
        {
            *pMag = pktX0.addr.magNum;
        }
    }


    // set transfer operation mode
    //Mode = 0;//WITH_SDRM_MODE;
    //word_data|= ((uint32_t)Mode) << 1;


    if ( pParam != NULL )
    {
        // decide char set
        pParam->DEFAULT_G0_CHARSET = (word_data>>3)&0x1F;
        pParam->SECOND_G0_CHARSET  = (word_data>>8)&0x1F;
        pParam->G1_CHARSET         = (word_data>>13)&0x1F;
        pParam->C12C13C14          = (word_data>>18)&0x7;

        // L1.5 or L2.5
        pParam->Alpha_Black_Enable   = (word_data>>24)&0x1;
        pParam->Double_Width_Enable  = (word_data>>23)&0x1;
        pParam->Double_Size_Enable   = (word_data>>22)&0x1;
        pParam->ESC_Enable           = (word_data>>25)&0x1;
        pParam->MOSAICS_BLACK_Enable = (word_data>>21)&0x1;
        pParam->NewBackground_Enable = (word_data>>26)&0x1;
        pParam->SizeAttrMode         = (word_data>>31)&0x1;


        // Sub-title or Newflash
        pParam->Newsflash_Subtitle = (word_data>>27)&0x1;

        // X24 Process
        //pParam->X24_Enable         = (word_data>>29)&0x1;

        // Mode Select
        //pParam->Mode = Mode;    // 0: with DRAM Mode

        // Color LUT
        pParam->FG_CLUT     = (0>>5)&0x3;
        pParam->BG_CLUT     = (0>>3)&0x3;
        pParam->REMAP_CLUT  = (0>>7)&0x3;
        pParam->Default_BGC = TTX_COLOR_BLACK;

        return( TRUE );
    }

    return( FALSE );
#endif
}
/*---------------------------------------------------------------------------*/
static bool_t TTX_PacketParse (
    TTX_STD_Packet_t* pPkt, const uint8_t src[PKT_BUF_SIZE_BYTES] )
/*****************************************************************************\
 *  @brief   Parse src[0]~src[PKT_BUF_SIZE_BYTES-1] to TTX_STD_Packet_t.
 *  @param   src[0]~src[PKT_BUF_SIZE_BYTES-1] contains descrambled raw packet data.
 *  @post    Only pPkt->dataBytes[] will be updated.
 *  @return  the parse result.
 *  @retval  TTX_TRUE  success
\*****************************************************************************/
{
    if ( pPkt != NULL )
    {
        // Note: pPkt->addr.magNum and pPkt->addr.pktNum is contained in field buffer
        //          offset, and cannot be updated from src[].

        TTX_REQUIRE( PKT_BUF_SIZE_BYTES >= TTX_STD_PACKET_PAYLOAD_SIZE_BYTES );
        memcpy( pPkt->dataBytes, &(src[0]), TTX_STD_PACKET_PAYLOAD_SIZE_BYTES );

        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t TTX_PacketX0Parse (
    TTX_STD_PacketX0_t* pPkt, const uint8_t src[PKT_BUF_SIZE_BYTES] )
/*****************************************************************************\
 *  @brief   Parse src[0]~src[PKT_BUF_SIZE_BYTES-1] to TTX_STD_PacketX0_t.
 *  @param   src[0]~src[PKT_BUF_SIZE_BYTES-1] is the X/0 field buffer and
 *             contains descrambled packet data.
 *  @return  the parse result.
 *  @retval  TTX_TRUE  success
\*****************************************************************************/
{
    if ( pPkt != NULL )
    {
        pPkt->addr.magNum = src[0] & 0x07;
        pPkt->addr.pktNum = src[1] & 0x1F;

        TTX_ENSURE( pPkt->addr.pktNum == 0 );   // ERROR: packet number of X/0 is not 0.

        pPkt->pageAddr.magNum               = pPkt->addr.magNum;
        pPkt->pageAddr.pageNum.units        =  src[3]       & 0x0F;
        pPkt->pageAddr.pageNum.tens         = (src[3] >> 4) & 0x0F;

        pPkt->pageAddr.subCode.s1           =  src[5]       & 0x0F;
        pPkt->pageAddr.subCode.s2           = (src[5] >> 4) & 0x07;
        pPkt->pageAddr.subCode.s3           =  src[4]       & 0x0F;
        pPkt->pageAddr.subCode.s4           = (src[4] >> 4) & 0x03;

        pPkt->ctrlBits.bErasePage           = (src[6] >> 6) & 0x01;    /* C4  */
        pPkt->ctrlBits.bNewsFlash           = (src[6] >> 5) & 0x01;    /* C5  */
        pPkt->ctrlBits.bSubtitle            = (src[6] >> 4) & 0x01;    /* C6  */
        pPkt->ctrlBits.bSuppressHeader      = (src[6] >> 3) & 0x01;    /* C7  */
        pPkt->ctrlBits.bUpdate              = (src[6] >> 2) & 0x01;    /* C8  */
        pPkt->ctrlBits.bInterruptedSequence = (src[6] >> 1) & 0x01;    /* C9  */
        pPkt->ctrlBits.bInhibitDisplay      =  src[6]       & 0x01;    /* C10 */
        pPkt->ctrlBits.bSerialMode          = (src[7] >> 3) & 0x01;    /* C11 */
        pPkt->ctrlBits.natlOpt              =  src[7]       & 0x07;    /* C12, C13, C14 */

        TTX_REQUIRE( PKT_BUF_SIZE_BYTES >= (TTX_STD_PACKET_X0_PAYLOAD_SIZE_BYTES + 8) );
        memcpy( pPkt->dataBytes, &(src[8]), TTX_STD_PACKET_X0_PAYLOAD_SIZE_BYTES );

        return( TRUE );
    }

    return( FALSE );
}


/*---------------------------------------------------------------------------*/
static bool_t TTX_DecodeLine (
    TTX_DISP_Attr_t DisAttrMem[TTX_DISP_MAX_ROW_LEN], TTX_STD_Packet_t* pPkt,
    const TTX_LopParam_t* param, uint8_t* pIgnoreLine, const bool_t rolling)
/*****************************************************************************\
 *  @brief   The de-initialization routine.
 *  @param   src[] is the field buffer of a whole page.
 *             src[  0]~src[ 39] is the   descrambled X/0 with PPR format.
 *             src[ 40]~src[ 79] is the undescrambled X/1 data.
 *             src[ 80]~src[119] is the undescrambled X/2 data.
 *             ....
 *           dest[] will be decoded display attribute memory.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Init()
\*****************************************************************************/
{
    TTX_STD_Color_t BGC_now, BGC_next; // current/next background color
    TTX_STD_Color_t FGC_now, FGC_next; // current/next foreground color
    uint8_t FLASH_now, FLASH_next; // current/next flash
    TTX_STD_SizeCfg_t SIZECFG_now, SIZECFG_next; // current/next size function for double width, double height, or double size
    uint8_t Graphic_now, Graphic_next;
    uint8_t CONCEAL_now, CONCEAL_next; // Add: gellmann@20050615@ver0.6.43 // current/next conceal function
    uint8_t MOSAICSTYPE_now; // mosaics type: continuous or separated (TTX_STD_MosaicsValues_t)
    uint8_t G0_at_G1_flag;
    uint8_t CHARSET_now, CHARSET_next; // current/next character set
    uint8_t G0G2_toggle; // toggle first and second G0
    TTX_STD_CharIndex_t HOLD_MOSAICS_now, HOLD_MOSAICS_next, HELD_MOSAICS_CHAR_now, HELD_MOSAICS_CHAR_next;
    TTX_STD_ColorTable_t BGCLUT_now, FGCLUT_now;
    uint8_t BOXAREA_now,BOXAREA_next;
    //uint8_t Strectch_Char_Pos; // Remove: gellmann@20050610@ver0.6.42

    uint8_t Strectch_col; // Add: gellmann@20050610@ver0.6.42
    bool_t Strectch_enable_now, Strectch_enable_next;
    TTX_STD_CharIndex_t CINDEX_now; // character index

    const uint8_t row = pPkt->addr.pktNum; // row
    uint8_t col; // column

    TTX_STD_DiacMark_t dm_idx; // diacritical mark index
    uint8_t x26info_num; // counter for X/26
    uint8_t CHARSET_x26; // character set specified by X/26, 0: use default, TTX_CHARSET_G2_LATIN: use G2, TTX_CHARSET_G3_MOSAICS: use G3

    //Strectch_Char_Pos = 0xff; // Remove: gellmann@20050610@ver0.6.42
    uint8_t* pkt = pPkt->dataBytes;

    uint8_t X0_skip_col = 8;

    if ( rolling == TRUE ) // sometimes, rolling header need to change color
        X0_skip_col = 7; // reserve 1 byte to display color


    BGC_now = BGC_next = param->Default_BGC; //TTX_COLOR_BLACK; // default background color is usually black
    FGC_now = FGC_next = TTX_COLOR_WHITE; // default foreground color sets as white
    FGCLUT_now = param->FG_CLUT; // foreground color look-up table
    BGCLUT_now = param->BG_CLUT; // background color look-up table
    FLASH_now = FLASH_next = TTX_FLASH_STEADY; // steady --> no flash
    CONCEAL_now = CONCEAL_next = TTX_CONCEAL_OFF; // no conceal
    MOSAICSTYPE_now = TTX_MOSAICS_CON; // continuous
    G0_at_G1_flag = 0; //0:Default 1:G0_at_G1
    CHARSET_now = CHARSET_next = param->DEFAULT_G0_CHARSET; // default G0 character set
    G0G2_toggle = 1; // 1:Default 0: Second
    SIZECFG_now = SIZECFG_next = TTX_STD_SIZE_CFG_NORMAL; // normal size
    HOLD_MOSAICS_now = HOLD_MOSAICS_next = TTX_SPATT_RELEASE_MOSAICS;
    HELD_MOSAICS_CHAR_now = HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
    Graphic_now = Graphic_next = TTX_GRAPHIC_OFF;

    //Strectch_Char_Pos = 0xff; // Remove: gellmann@20050610@ver0.6.42

    Strectch_enable_now = Strectch_enable_next = FALSE; // Add: gellmann@20050610@ver0.6.42
    Strectch_col = 0x00; // 0: even col 1: odd col      // Add: gellmann@20050610@ver0.6.42

    BOXAREA_now = BOXAREA_next = TTX_BOX_END;

    for ( col=0 ; col<TTX_STD_PACKET_PAYLOAD_SIZE_BYTES ; ++col )
    {
        if ( (row==0) && (col<X0_skip_col) ) // page number and control bits
            continue;

        CINDEX_now = TTX_CHAR_SPACE; // space
        dm_idx = TTX_DIAC_MARK_DISABLE; // no diacritical mark
        CHARSET_x26 = 0; // use default

        if ( *pIgnoreLine != row )
        { // not ignore line
            //TTX_Log(LOGGER_VERBOSE, "pkt[%d] = %x\n",(uint16_t)col,(uint32_t)pkt[col]);
            switch ( pkt[col] )
            {
            case TTX_SPATT_ALPHA_BLACK: // Set-After
                if ( param->Alpha_Black_Enable ) // L2.5
                {
                    FGC_next = TTX_COLOR_BLACK;
                    Graphic_next = TTX_GRAPHIC_OFF;
                    CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                    //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                    CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                    HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                    //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                    CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                }
                break;
            case TTX_SPATT_ALPHA_RED: // Set-After
                FGC_next = TTX_COLOR_RED;
                Graphic_next = TTX_GRAPHIC_OFF;
                CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_ALPHA_GREEN: // Set-After
                FGC_next = TTX_COLOR_GREEN;
                Graphic_next = TTX_GRAPHIC_OFF;
                CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_ALPHA_YELLOW: // Set-After
                FGC_next = TTX_COLOR_YELLOW;
                Graphic_next = TTX_GRAPHIC_OFF;
                CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_ALPHA_BLUE: // Set-After
                FGC_next = TTX_COLOR_BLUE;
                Graphic_next = TTX_GRAPHIC_OFF;
                CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_ALPHA_MAGENTA: // Set-After
                FGC_next = TTX_COLOR_MAGENTA;
                Graphic_next = TTX_GRAPHIC_OFF;
                CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_ALPHA_CYAN: // Set-After
                FGC_next = TTX_COLOR_CYAN;
                Graphic_next = TTX_GRAPHIC_OFF;
                CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_ALPHA_WHITE: // Set-After
                FGC_next = TTX_COLOR_WHITE;
                Graphic_next = TTX_GRAPHIC_OFF;
                CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_FLASH: // Set-After
                FLASH_next = TTX_FLASH_NORMAL;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_STEADY: // Set-At
                FLASH_now = FLASH_next = TTX_FLASH_STEADY;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_END_BOX: // Set-After
                BOXAREA_next = TTX_BOX_END;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_START_BOX: // Set-After
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                if ( col <= 38 && pkt[col+1] == TTX_SPATT_START_BOX ) // Double Transmission Detect
                {
                    BOXAREA_next = TTX_BOX_START;
                }
                break;
            case TTX_SPATT_NORMAL_SIZE: // Set-At
                if (Strectch_enable_now && ((col%2) == Strectch_col))
                {
                    SIZECFG_next = TTX_STD_SIZE_CFG_NORMAL;
                    Strectch_enable_next = FALSE;
                }
                else
                {
                    SIZECFG_now = SIZECFG_next = TTX_STD_SIZE_CFG_NORMAL;
                    Strectch_enable_now = Strectch_enable_next = FALSE;
                }

                if ( SIZECFG_now != TTX_STD_SIZE_CFG_NORMAL )
                {
                    HELD_MOSAICS_CHAR_now = HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE; // Held Mosaics Character Reset
                    //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                }

                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                //SIZECFG_now =
                //SIZECFG_next = TTX_STD_SIZE_CFG_NORMAL;
                //Strectch_Char_Pos = 0xff;
                break;
            case TTX_SPATT_DOUBLE_HEIGHT: // Set-After
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                if (row == 23 || row == 24)
                    break; // DOUBLE HEIGHT should "NOT" insert in row 23 or row 24
                //Strectch_enable_now = Strectch_enable_next = FALSE;
                Strectch_enable_next = FALSE;
                SIZECFG_next = TTX_STD_SIZE_CFG_DH_UPPER;
                *pIgnoreLine = row + 1;

                if (	SIZECFG_now != TTX_STD_SIZE_CFG_DH_UPPER &&
                        SIZECFG_now != TTX_STD_SIZE_CFG_DH_LOWER )  // Held Mosaics Character Reset
                {
                    HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                    //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                }

                // add by timhuang@20060313
                if (	SIZECFG_now == TTX_STD_SIZE_CFG_DW_LEFT ||
                        SIZECFG_now == TTX_STD_SIZE_CFG_DS_L_UPPER )
                {
                    SIZECFG_now = TTX_STD_SIZE_CFG_NORMAL;
                }
                break;
            case TTX_SPATT_DOUBLE_WIDTH: // Set-After
                if (param->Double_Width_Enable)// L2.5
                {
                    if (col == 38 || col == 39) break; // DOUBLE WIDTH should "NOT" insert in col 38 or col 39
                    SIZECFG_next = TTX_STD_SIZE_CFG_DW_LEFT;
                    //Strectch_Char_Pos = col + 2; // Remove: gellmann@20050610@ver0.6.42

                    if (!Strectch_enable_now)
                        Strectch_col = (col%2) ? 0x01: 0x00;

                    Strectch_enable_next = TRUE;

                    if (	SIZECFG_now != TTX_STD_SIZE_CFG_DW_LEFT &&
                            SIZECFG_now != TTX_STD_SIZE_CFG_DW_RIGHT )  // Held Mosaics Character Reset
                    {
                        HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                        //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                    }

                    // add by timhuang@20060313
                    if (	SIZECFG_now == TTX_STD_SIZE_CFG_DW_LEFT ||
                            SIZECFG_now == TTX_STD_SIZE_CFG_DS_L_UPPER )
                    {
                        SIZECFG_now = TTX_STD_SIZE_CFG_NORMAL;
                    }

                    CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                    /* Commented by Janus to eliminate compiler warning
                    						if(param->SizeAttrMode==TTX_SIZE_ATTR_MODE_MICRONAS)
                    						{
                    							if (	(	pkt[col+1] == TTX_SPATT_DOUBLE_HEIGHT ||
                    									pkt[col+1] == TTX_SPATT_DOUBLE_WIDTH  ||
                    									pkt[col+1] == TTX_SPATT_DOUBLE_SIZE )  &&
                    								(	pkt[col+2] != TTX_SPATT_DOUBLE_HEIGHT ||
                    									pkt[col+2] != TTX_SPATT_DOUBLE_WIDTH  ||
                    									pkt[col+2] != TTX_SPATT_DOUBLE_SIZE ) )
                    							{
                    								pkt[col+2] = TTX_CHAR_SPACE;
                    							}
                    						}
                    */
                }
                break;
            case TTX_SPATT_DOUBLE_SIZE: // Set-After
                if ( param->Double_Size_Enable ) // L2.5
                {
                    if (row == 23 || row == 24 || col == 38 || col == 39) break; // DOUBLE HEIGHT should "NOT" insert in row 23 or row 24
                    SIZECFG_next = TTX_STD_SIZE_CFG_DS_L_UPPER;
                    *pIgnoreLine = row + 1;
                    //Strectch_Char_Pos = col + 2; // Remove: gellmann@20050610@ver0.6.42

                    if (!Strectch_enable_now)
                        Strectch_col = (col%2) ? 0x01: 0x00;

                    Strectch_enable_next = TRUE;

                    if (	SIZECFG_now != TTX_STD_SIZE_CFG_DS_L_UPPER &&
                            SIZECFG_now != TTX_STD_SIZE_CFG_DS_R_UPPER &&
                            SIZECFG_now != TTX_STD_SIZE_CFG_DS_L_LOWER &&
                            SIZECFG_now != TTX_STD_SIZE_CFG_DS_R_LOWER )  // Held Mosaics Character Reset
                    {
                        HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                    }

                    // add by timhuang@20060313
                    if (	SIZECFG_now == TTX_STD_SIZE_CFG_DW_LEFT ||
                            SIZECFG_now == TTX_STD_SIZE_CFG_DS_L_UPPER )
                    {
                        SIZECFG_now = TTX_STD_SIZE_CFG_NORMAL;
                    }

                    CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                    /* Commented by Janus to eliminate compiler warning
                    						if(param->SizeAttrMode==TTX_SIZE_ATTR_MODE_MICRONAS)
                    						{
                    							if (	(	pkt[col+1]==TTX_SPATT_DOUBLE_HEIGHT ||
                    									pkt[col+1]==TTX_SPATT_DOUBLE_WIDTH  ||
                    									pkt[col+1]==TTX_SPATT_DOUBLE_SIZE  ) &&
                    								(	pkt[col+2]!=TTX_SPATT_DOUBLE_HEIGHT ||
                    									pkt[col+2]!=TTX_SPATT_DOUBLE_WIDTH  ||
                    									pkt[col+2]!=TTX_SPATT_DOUBLE_SIZE  ) )
                    							{
                    								pkt[col+2] = TTX_CHAR_SPACE;
                    							}
                    						}
                    */
                }
                else
                { // double height
                    CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                    if (row == 23 || row == 24)
                        break; // DOUBLE HEIGHT should "NOT" insert in row 23 or row 24
                    //Strectch_enable_now = Strectch_enable_next = FALSE;
                    Strectch_enable_next = FALSE;
                    SIZECFG_next = TTX_STD_SIZE_CFG_DH_UPPER;
                    *pIgnoreLine = row + 1;

                    if (	SIZECFG_now != TTX_STD_SIZE_CFG_DH_UPPER &&
                            SIZECFG_now != TTX_STD_SIZE_CFG_DH_LOWER )  // Held Mosaics Character Reset
                    {
                        HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                        //if(HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) HELD_MOSAICS_CHAR_next = TTX_CHAR_SPACE;
                    }

                    // add by timhuang@20060313
                    if (	SIZECFG_now == TTX_STD_SIZE_CFG_DW_LEFT ||
                            SIZECFG_now == TTX_STD_SIZE_CFG_DS_L_UPPER )
                    {
                        SIZECFG_now = TTX_STD_SIZE_CFG_NORMAL;
                    }
                }
                break;
            case TTX_SPATT_MOSAICS_BLACK: // Set-After
                if ( param->MOSAICS_BLACK_Enable ) // L2.5
                {
                    FGC_next = TTX_COLOR_BLACK;
                    Graphic_next = TTX_GRAPHIC_ON;
                    CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                    //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                    CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                    CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                }
                break;
            case TTX_SPATT_MOSAICS_RED: // Set-After
                FGC_next = TTX_COLOR_RED;
                Graphic_next = TTX_GRAPHIC_ON;
                CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_MOSAICS_GREEN: // Set-After
                FGC_next = TTX_COLOR_GREEN;
                Graphic_next = TTX_GRAPHIC_ON;
                CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_MOSAICS_YELLOW: // Set-After
                FGC_next = TTX_COLOR_YELLOW;
                Graphic_next = TTX_GRAPHIC_ON;
                CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_MOSAICS_BLUE: // Set-After
                FGC_next = TTX_COLOR_BLUE;
                Graphic_next = TTX_GRAPHIC_ON;
                CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_MOSAICS_MAGENTA: // Set-After
                FGC_next = TTX_COLOR_MAGENTA;
                Graphic_next = TTX_GRAPHIC_ON;
                CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_MOSAICS_CYAN: // Set-After
                FGC_next = TTX_COLOR_CYAN;
                Graphic_next = TTX_GRAPHIC_ON;
                CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_MOSAICS_WHITE: // Set-After
                FGC_next = TTX_COLOR_WHITE;
                Graphic_next = TTX_GRAPHIC_ON;
                CHARSET_next = param->G1_CHARSET;//G1_BASIC;
                //CONCEAL_now = TTX_CONCEAL_OFF;  // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_next = TTX_CONCEAL_OFF;   // Add: gellmann@20050615@ver0.6.43
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_CONCEAL: // Set-At
                //CONCEAL_now = TTX_CONCEAL_ON;              // Remove: gellmann@20050615@ver0.6.43
                CONCEAL_now = CONCEAL_next = TTX_CONCEAL_ON; // Add: gellmann@20050615@ver0.6.43
                break;
            case TTX_SPATT_CON_MOSAICS_GRAPHICS: // Set-At
                MOSAICSTYPE_now = TTX_MOSAICS_CON;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_SEP_MOSAICS_GRAPHICS: // Set-At
                MOSAICSTYPE_now = TTX_MOSAICS_SEP;
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_ESC: // Set-After
                if (param->ESC_Enable)
                {
                    G0G2_toggle = !G0G2_toggle;
                    if ( Graphic_now == TTX_GRAPHIC_OFF )
                    {
                        CHARSET_next = (G0G2_toggle) ? param->DEFAULT_G0_CHARSET:param->SECOND_G0_CHARSET;
                    }
                    CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                }
                break;
            case TTX_SPATT_BLACK_BACKGROUND: // Set-At
                //if(Strectch_enable_now && ((col%2) == Strectch_col)){ // Remove: bug gellmann@20050613@ver0.6.43
                //   BGC_next = TTX_COLOR_BLACK;
                //}else{
                BGC_now = BGC_next = TTX_COLOR_BLACK;
                //}
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_NEW_BACKGROUND: // Set-At
                //if(param->NewBackground_Enable) BGCLUT_now = param->REMAP_CLUT; // L2.5
                //BGC_now = BGC_next = FGC_now;

                //Modified by TimHuang@20051220
                if (param->NewBackground_Enable)
                {
                    BGCLUT_now = param->REMAP_CLUT; // L2.5
                    BGC_now = BGC_next = FGC_now;
                }
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                break;
            case TTX_SPATT_HOLD_MOSAICS: // Set-At
                HOLD_MOSAICS_now = HOLD_MOSAICS_next = TTX_SPATT_HOLD_MOSAICS;
                CINDEX_now = HELD_MOSAICS_CHAR_now;
                break;
            case TTX_SPATT_RELEASE_MOSAICS: // Set-After
                CINDEX_now = (HOLD_MOSAICS_now == TTX_SPATT_HOLD_MOSAICS) ? HELD_MOSAICS_CHAR_now:TTX_CHAR_SPACE;
                HOLD_MOSAICS_next = TTX_SPATT_RELEASE_MOSAICS;
                break;
            default:
                //if(CONCEAL_now == TTX_CONCEAL_ON) CINDEX_now = TTX_CHAR_SPACE;
                //else
                CINDEX_now = pkt[col];

                if ( ttx_x26_info.counter != 0 )
                {
                    for ( x26info_num=0 ; x26info_num<ttx_x26_info.counter ; x26info_num++ )
                    {
                        if (	(row == ttx_x26_info.x26_decoded_data[x26info_num].row) &&
                                (col == ttx_x26_info.x26_decoded_data[x26info_num].col ) )
                        {
                            CINDEX_now = ttx_x26_info.x26_decoded_data[x26info_num].c_index;
                            switch ( ttx_x26_info.x26_decoded_data[x26info_num].function )
                            {
                            case TTX_X26Function_G0_CHAR_WITHOUT_DM:
                            case TTX_X26Function_G0_CHAR_WITH_DM:
                                dm_idx = ttx_x26_info.x26_decoded_data[x26info_num].dm_index;
                                break;

                            case TTX_X26Function_CHAR_FROM_G2:
                                CHARSET_x26 = TTX_CHARSET_G2_LATIN;
                                break;

                            case TTX_X26Function_SMOOTHED_MOSAIC_CHAR_FROM_G3:
                                CHARSET_x26 = TTX_CHARSET_G3_MOSAICS;
                                break;
                            }
                            break;
                        }
                    }
                }

                //G1 Block Mosaics Set(en706 p.118), mosaic characters will reset to "space" because of
                //different alphanumeric set (G0/G2). Mosaic character can't be replaced between 0x4x~0x5x.
                //Modified by TimHuang@2005.11.28
                if (Graphic_now == TTX_GRAPHIC_ON && (CINDEX_now&0x70)!=0x40 && (CINDEX_now&0x70)!=0x50)
                {
                    HELD_MOSAICS_CHAR_now = HELD_MOSAICS_CHAR_next = CINDEX_now;
                }

                break;
            }   // end of switch( pkt[col] )

            // Double Width Process
            if (Strectch_enable_now && ((col%2) == Strectch_col))
            {

                CINDEX_now = DisAttrMem[col-1].charIdx;

                // Add: keep previous seperate attr for G0/G1 set by timhuang@20060221
                if ( !G0_at_G1_flag )
                {
                    MOSAICSTYPE_now = DisAttrMem[col-1].mosaics;
                    Graphic_now = DisAttrMem[col-1].graphic;
                }

                CHARSET_now = DisAttrMem[col-1].charset; // Add: bug gellmann@20050613@ver0.6.43
                dm_idx = DisAttrMem[col-1].dmIdx;
                FGC_now = DisAttrMem[col-1].fgColorIdx; // Add: bug gellmann@20050613@ver0.6.43

                // Add: bug gellmann@20050613@ver0.6.43
                BGC_now = DisAttrMem[col-1].bgColorIdx;                 // Add: bug gellmann@20050613@ver0.6.43
                FLASH_now = DisAttrMem[col-1].flashMode;                // Add: bug gellmann@20050613@ver0.6.43
                BGCLUT_now = DisAttrMem[col-1].bgColorTab;              // Add: bug gellmann@20050613@ver0.6.43
                FGCLUT_now = DisAttrMem[col-1].fgColorTab;              // Add: bug gellmann@20050613@ver0.6.43
                CONCEAL_now = DisAttrMem[col-1].conceal;                // Add: bug gellmann@20050613@ver0.6.43

                //if(DisAttrMem[col-1].transparent) BOXAREA_now = TTX_BOX_END;   // Add: bug gellmann@20050613@ver0.6.43 // Remove: bug gellmann@20050627@ver0.6.50
                //else                              BOXAREA_now = TTX_BOX_START; // Add: bug gellmann@20050613@ver0.6.43 // Remove: bug gellmann@20050627@ver0.6.50

                BOXAREA_now = DisAttrMem[col-1].box;                       // Add: bug gellmann@20050627@ver0.6.50
                SIZECFG_now = DisAttrMem[col-1].size;                   // Add: bug gellmann@20050613@ver0.6.43

                if (SIZECFG_now == TTX_STD_SIZE_CFG_DW_LEFT)
                {
                    SIZECFG_now = TTX_STD_SIZE_CFG_DW_RIGHT;
                }

                if (SIZECFG_now == TTX_STD_SIZE_CFG_DS_L_UPPER)
                {
                    SIZECFG_now = TTX_STD_SIZE_CFG_DS_R_UPPER;
                }
            }

            if ( Strectch_enable_next )
            {
                if (col == 38)
                {
                    if (SIZECFG_now == TTX_STD_SIZE_CFG_DW_RIGHT)
                    {
                        Strectch_enable_next = FALSE;
                        SIZECFG_next = TTX_STD_SIZE_CFG_NORMAL;
                    }

                    if (SIZECFG_now == TTX_STD_SIZE_CFG_DS_R_UPPER)
                    {
                        SIZECFG_next = TTX_STD_SIZE_CFG_DH_UPPER;
                    }
                }
            }
        }   // end of if( *pIgnoreLine != row )
        else
        {
            // Modified by timhuang@2005.12.15
            // Follow the attribute of upper line character
            CINDEX_now = DisAttrMem[col].charIdx;

            MOSAICSTYPE_now = DisAttrMem[col].mosaics;
            CHARSET_now = DisAttrMem[col].charset;
            dm_idx = DisAttrMem[col].dmIdx;
            FGC_now = DisAttrMem[col].fgColorIdx;
            Graphic_now = DisAttrMem[col].graphic;
            BGC_now = DisAttrMem[col].bgColorIdx;
            BGCLUT_now = DisAttrMem[col].bgColorTab;
            FGCLUT_now = DisAttrMem[col].fgColorTab;
            //DisAttrMem[col].conceal = CONCEAL_now; // why ? replaced by following line
            CONCEAL_now = DisAttrMem[col].conceal;
            BOXAREA_now = DisAttrMem[col].box;
            FLASH_now = DisAttrMem[col].flashMode;

            if ( DisAttrMem[col].size == TTX_STD_SIZE_CFG_DH_UPPER )
                SIZECFG_now = TTX_STD_SIZE_CFG_DH_LOWER;
            else if ( DisAttrMem[col].size == TTX_STD_SIZE_CFG_DS_L_UPPER )
                SIZECFG_now = TTX_STD_SIZE_CFG_DS_L_LOWER;
            else if ( DisAttrMem[col].size == TTX_STD_SIZE_CFG_DS_R_UPPER )
                SIZECFG_now = TTX_STD_SIZE_CFG_DS_R_LOWER;
            else
            {
                CINDEX_now = TTX_CHAR_SPACE;
                SIZECFG_now = TTX_STD_SIZE_CFG_NORMAL;
            }
        }

        // row*DISPLAY_COL_NUM+
        memset( &DisAttrMem[col], 0, sizeof(DisAttrMem[col]) );
        //vbiVte_memset((UINT32)(&DisAttrMem[col]),0,8);

        DisAttrMem[col].size       = SIZECFG_now;
        DisAttrMem[col].graphic    = Graphic_now;
        DisAttrMem[col].bUnderline = FALSE;
        if (CHARSET_now == param->G1_CHARSET)
            DisAttrMem[col].mosaics = MOSAICSTYPE_now;
        else
            DisAttrMem[col].mosaics = 0;
        G0_at_G1_flag = 0;
#if 0
        DisAttrMem[col].charset = CHARSET_now;
        DisAttrMem[col].dmIdx   = TTX_DIAC_MARK_DISABLE;
#else
        if ( CHARSET_x26 == 0 )
            DisAttrMem[col].charset = CHARSET_now;
        else
            DisAttrMem[col].charset = CHARSET_x26;
        DisAttrMem[col].dmIdx   = dm_idx;
#endif
        DisAttrMem[col].conceal = CONCEAL_now;

        if (FLASH_now == TTX_FLASH_STEADY)
            DisAttrMem[col].flashMode = TTX_FLASH_STEADY;
        else if (FLASH_now == TTX_FLASH_NORMAL)
            DisAttrMem[col].flashMode = TTX_FLASH_NORMAL;

#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
        DisAttrMem[col].flashRate = TTX_FLASH_SLOW;
#endif  /* end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25) */

        DisAttrMem[col].fgColorIdx = FGC_now;
        DisAttrMem[col].bgColorIdx = BGC_now;
        DisAttrMem[col].bgColorTab = BGCLUT_now;
        DisAttrMem[col].fgColorTab = FGCLUT_now;

        //if(param->Newsflash_Subtitle){ // Remove: bug gellmann@20050627@ver0.6.50
        //	if(BOXAREA_now == TTX_BOX_END && (row < 24)) DisAttrMem[col].transparent = 1;
        //	else                       DisAttrMem[col].transparent = 0;
        //}else DisAttrMem[col].transparent = 0;

        if (param->Newsflash_Subtitle) // Add: bug gellmann@20050627@ver0.6.50
        {
            if ( BOXAREA_now == TTX_BOX_END && (row < 24) )
                DisAttrMem[col].box = TTX_BOX_END;
            else
                DisAttrMem[col].box = TTX_BOX_START;
        }
        else
            DisAttrMem[col].box = TTX_BOX_END;

        //if( param->Mode == 0 ) // 0: WITH_SDRM_MODE
        {
            DisAttrMem[col].charIdx = CINDEX_now;
            if (CHARSET_now == param->G1_CHARSET)
            {
                if (CINDEX_now >=0x40 && CINDEX_now <= 0x5F)
                {
                    DisAttrMem[col].graphic = TTX_GRAPHIC_OFF;
                    DisAttrMem[col].mosaics = TTX_MOSAICS_CON;
                    G0_at_G1_flag = 1;

                    // Added by Janus (Apr. 22, 2008): To fix Mantis bug #0009750.
                    {
                        // Implements the Note 1 of Clause 15.7.1.
                        //   This can be found in P474 of SpainTVE01.trp (SpainTVE01_pid0x66.PES).
                        DisAttrMem[col].charset = param->DEFAULT_G0_CHARSET;
                    }

                }
            }
        }

// <Janus> Test:
        /*
        DisAttrMem[col].fgc      = TTX_COLOR_WHITE;
        DisAttrMem[col].csel     = TTX_CHARSET_G0_LATIN;
        DisAttrMem[col].cindex   = 'A';
        DisAttrMem[col].bgc      = TTX_COLOR_RED;
        DisAttrMem[col].box      = FALSE;
        DisAttrMem[col].graphic  = FALSE;
        DisAttrMem[col].conceal  = FALSE;
        DisAttrMem[col].flash    = FALSE;
        DisAttrMem[col].size     = TTX_STD_SIZE_CFG_NORMAL;
        DisAttrMem[col].ul       = FALSE;
        DisAttrMem[col].dm       = 0;
        DisAttrMem[col].separate = FALSE;
        */

        /* Commented by Janus: We always use WITH_SDRM_MODE
                 else
                 {
                     if( SIZECFG_now == TTX_STD_SIZE_CFG_DW_RIGHT   || SIZECFG_now == TTX_STD_SIZE_CFG_DH_LOWER   ||
                         SIZECFG_now == TTX_STD_SIZE_CFG_DS_L_LOWER || SIZECFG_now == TTX_STD_SIZE_CFG_DS_R_LOWER ||
                         SIZECFG_now == TTX_STD_SIZE_CFG_DS_R_UPPER ) // Add: bug gellmann@20050627@ver0.6.43
                     {
                             DisAttrMem[col].cindex = CINDEX_now & 0xff;
                     }
                     else
                     {
                         if( CHARSET_now == param->DEFAULT_G0_CHARSET ) // G0
                         {
                             switch( CINDEX_now )
                             {
                                case 0x23:
                                    DisAttrMem[col].cindex = 0x70 + 0x40 + param->C12C13C14 - 0x20; break;
                                case 0x24:
                                    DisAttrMem[col].cindex = 0x70 + 0x48 + param->C12C13C14 - 0x20; break;
                                case 0x40:
                                    DisAttrMem[col].cindex = 0x70 + 0x50 + param->C12C13C14 - 0x20; break;
                                case 0x5B:
                                    DisAttrMem[col].cindex = 0x70 + 0x58 + param->C12C13C14 - 0x20; break;
                                case 0x5C:
                                    DisAttrMem[col].cindex = 0x190 + param->C12C13C14; break;
                                case 0x5D:
                                    DisAttrMem[col].cindex = 0x198 + param->C12C13C14; break;
                                case 0x5E:
                                    DisAttrMem[col].cindex = 0x1A0 + param->C12C13C14; break;
                                case 0x5F:
                                    DisAttrMem[col].cindex = 0x1A8 + param->C12C13C14; break;
                                case 0x60:
                                    DisAttrMem[col].cindex = 0x1B0 + param->C12C13C14; break;
                                case 0x7B:
                                    DisAttrMem[col].cindex = 0x1B8 + param->C12C13C14; break;
                                case 0x7C:
                                    DisAttrMem[col].cindex = 0x1C0 + param->C12C13C14; break;
                                    break;
                                case 0x7D:
                                    DisAttrMem[col].cindex = 0x1C8 + param->C12C13C14; break;
                                    break;
                                case 0x7E:
                                    DisAttrMem[col].cindex = 0x1D0 + param->C12C13C14; break;
                                    break;
                                default:
                                    DisAttrMem[col].cindex = 0x10 + CINDEX_now - 0x20;
                                    break;
                             }  // end of switch( CINDEX_now )
                        }
                        else
                        {
                            if( CHARSET_now == param->G1_CHARSET )
                            {
                                if( CINDEX_now >=0x40 && CINDEX_now <= 0x5F )
                                {
                                    DisAttrMem[col].graphic = TTX_GRAPHIC_OFF;
                                    DisAttrMem[col].separate = TTX_MOSAICS_CON;
                                    G0_at_G1_flag = 1;
                                    switch( CINDEX_now )
                                    {
                                        case 0x40:
                                            DisAttrMem[col].cindex = 0x70 + 0x50 + param->C12C13C14 - 0x20;
                                            break;
                                        case 0x5B:
                                            DisAttrMem[col].cindex = 0x70 + 0x58 + param->C12C13C14 - 0x20;
                                            break;
                                        case 0x5C:
                                            DisAttrMem[col].cindex = 0x190 + param->C12C13C14;
                                            break;
                                        case 0x5D:
                                            DisAttrMem[col].cindex = 0x198 + param->C12C13C14;
                                            break;
                                        case 0x5E:
                                            DisAttrMem[col].cindex = 0x1A0 + param->C12C13C14;
                                            break;
                                        case 0x5F:
                                            DisAttrMem[col].cindex = 0x1A8 + param->C12C13C14;
                                            break;
                                        default:
                                            DisAttrMem[col].cindex = 0x10 + CINDEX_now - 0x20;
                                            break;
                                    }   // end of switch( CINDEX_now )
                                }   // end of if( CINDEX_now >=0x40 && CINDEX_now <= 0x5F )
                                else
                                {
                                    DisAttrMem[col].cindex = 0x70 + CINDEX_now - 0x20;
                                }
                            }
                        }
                    }
                 }   // end of if( *pIgnoreLine != row )
        */  // end of Commented by Janus: We always use WITH_SDRM_MODE

        // Attribute Extend
        BGC_now = BGC_next; // Add: gellmann@20050610@ver0.6.42
        FGC_now = FGC_next;
        Graphic_now = Graphic_next;
        CHARSET_now = CHARSET_next;
        SIZECFG_now = SIZECFG_next;
        HOLD_MOSAICS_now = HOLD_MOSAICS_next;
        FLASH_now = FLASH_next;
        BOXAREA_now = BOXAREA_next;
        HELD_MOSAICS_CHAR_now = HELD_MOSAICS_CHAR_next;
        Strectch_enable_now = Strectch_enable_next; // Add: gellmann@20050610@ver0.6.42
        CONCEAL_now = CONCEAL_next; // Add: gellmann@20050615@ver0.6.43
    } // for(col=0;col<40;col++)

    if ( *pIgnoreLine == row )
        *pIgnoreLine = 0xff;

    return( TRUE );
}
#if 0
/*---------------------------------------------------------------------------*/
void
TTX_TestSoftware    (void)
/*****************************************************************************\
 *  @brief   test programming errors
\*****************************************************************************/
{

//TTX_DecodePage (NULL, NULL, NULL, 0, 26);
    TTX_STD_Test(); // Test TTX_STD module

    // Test TTX_PacketParse()
    {
        bool_t bOk;
        TTX_STD_Packet_t pkt;
        size_t i;
        const uint8_t pktBuf[PKT_BUF_SIZE_BYTES] =
        {
            1,  2,  3,  4,  5,  6,  7,  8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24,
            25, 26, 27, 28, 29, 30, 31, 32,
            33, 34, 35, 36, 37, 38, 39, 40
        };

        TTX_REQUIRE( PKT_BUF_SIZE_BYTES >= TTX_STD_PACKET_PAYLOAD_SIZE_BYTES );  // ERROR: Insufficient PKT_BUF_SIZE_BYTES.

        // run Unit-Under-Test
        bOk = TTX_PacketParse( &pkt, pktBuf );

        // Check post-conditions of UUT.
        TTX_ENSURE( bOk == TRUE );          // ERROR: unexpected return from TTX_PacketParse().
        for ( i=0; i<TTX_STD_PACKET_PAYLOAD_SIZE_BYTES; i++ )
        {
            TTX_ENSURE( pkt.dataBytes[i] == i+1 ); // ERROR: parse error in TTX_PacketParse().
        }
    }
}

#endif

/*---------------------------------------------------------------------------*\
 * end of ttx_dec.c                                                          *
\*---------------------------------------------------------------------------*/
#endif
