/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_dec_top.c
 * @brief    Teletext Table of Pages software decoder routines.
 * @author   Janus Cheng
 * @date     Jan. 23, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_dec_top.h>    /* Provides functions to be implemented */
#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */
#include <ttx_buff.h>       /* Provides: TTX_BUF_KeepPage() */
#include <ttx_disp.h>       /* Provides: TTX_DISP_SetAttr() */
#include <vte.h> /* Provides: VTE_Decode() */
#include <ttx_buff_pkg.h> /* TTX_TOP_BTT_PAGE_ADDRESS */
#include <ttx_dec.h>	/* Provides: TTX_LopParam_t */

#include <string.h>         /* Provides: memset() */

#define TTX_TOP_AIT_TITLE_SIZE  (12)    /* unit: bytes */

#define	TTX_TOP_RED_SIZE			(5)
#define	TTX_TOP_GREEN_SIZE		(5)
#define	TTX_TOP_YELLOW_SIZE		(15)
#define	TTX_TOP_CYAN_SIZE			(14)

TTX_DEFINE_THIS_MODULE( ttx_dec_top.c )

/* the Basic TOP table List (BTL) */
typedef struct TTX_TOP_BttList_t
{
    /*
     * array index-0: magazine-8
     *             1: magazine-1
     *             2: magazine-2
     *             3: magazine-3
     *             4: magazine-4
     *             5: magazine-5
     *             6: magazine-6
     *             7: magazine-7
     */
    TTX_STD_TOP_BttListCode_t mag[TTX_MAG_NUM_TOTAL];
} TTX_TOP_BttList_t;

typedef struct
{
    TTX_TOP_BttList_t   btl;    /* Basic TOP table List */
} TTX_TOP_Obj, *TTX_TOP_Handle;

static TTX_TOP_Obj ttxTopObj;

/* -- Prototypes of Private Functions ---------------------------------------*/
static bool_t TTX_TOP_GetTopPages                   ( const uint8_t TTX_MEM*            pDecodedBttPage    , \
        const TTX_STD_Page_Address_Dec_t  nowPage            , \
        TTX_STD_Page_Address_Dec_t* pBlockPage         , \
        TTX_STD_Page_Address_Dec_t* pGroupPage         , \
        TTX_STD_Page_Address_Dec_t* pNextPage          , \
        TTX_STD_Page_Address_Dec_t* pPrevPage          , \
        uint8_t*                    pbSubtitleAvailable, \
        TTX_STD_Page_Addr_t*     pSubtitlePage        );

static bool_t TTX_TOP_GetBttPageAddr                (TTX_STD_Page_AddrWithSubCode_t* pTopPageAddr, \
        const TTX_STD_TOP_BttListCode_t    btlCode       );

static bool_t TTX_TOP_GetAitTitle                   ( const uint8_t TTX_MEM*                   pDecodedBttPage, \
        const TTX_STD_Page_AddrWithSubCode_t* pTopPageAddr   , \
        uint8_t                            aitTitle[TTX_TOP_AIT_TITLE_SIZE]);

static bool_t TTX_TOP_GetAitTitleFromAit            ( const TTX_STD_Page_AddrWithSubCode_t* pAitPageAddr   , \
        const TTX_STD_Page_AddrWithSubCode_t* pDispPageAddr  , \
        uint8_t                            aitTitle[TTX_TOP_AIT_TITLE_SIZE]);

static bool_t TTX_TOP_GetAitTitleFromMpe            ( const TTX_STD_Page_AddrWithSubCode_t* pMpePageAddr   , \
        const TTX_STD_Page_AddrWithSubCode_t* pDispPageAddr  , \
        uint8_t                            aitTitle[TTX_TOP_AIT_TITLE_SIZE]);

static void TTX_TOP_Display (
    const uint8_t TTX_MEM* pDecodedBttPage,
    const TTX_STD_Page_Address_Dec_t blockPage,
    const TTX_STD_Page_Address_Dec_t groupPage,
    const TTX_LopParam_t* pParam );

static void TTX_TOP_Display_Prev_Next (void);

static bool_t TTX_PageAddrDecToPageAddrWithSubCode  (const TTX_STD_Page_Address_Dec_t   pFrom, \
        TTX_STD_Page_AddrWithSubCode_t* pTo    );

static bool_t TTX_ExpandPageAddressWithZeroSubCode  (const TTX_STD_Page_Addr_t*      pFrom, \
        TTX_STD_Page_AddrWithSubCode_t* pTo    );


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
void
TTX_TOP_Init                (void)
/*****************************************************************************\
 *  @brief   Initialize internal variables of TTX_TOP module.
 *  @note    Must run this function BEFORE calling any other TTX_TOP_xxxx()
 *             functions.
\*****************************************************************************/
{
    TTX_TOP_Handle hTop = &ttxTopObj;
    uint8_t i;

    for ( i = 0; i < TTX_MAG_NUM_TOTAL ; i++ )
    {
        hTop->btl.mag[i] = TTX_STD_TOP_BTT_LIST_CODE_NO_DATA;
    }

}

/*---------------------------------------------------------------------------*/
bool_t TTX_TOP_Decode (
    const TTX_STD_Page_AddrWithSubCode_t* pShowingPage,
    TTX_TOP_Service_t* pTop, uint8_t* pbSubtitleAvailable,
    TTX_STD_Page_Addr_t* pSubtitlePage, const TTX_LopParam_t* pParam )
/*****************************************************************************\
 *  @brief   Decode TOP service info for page *pPageAddr.
 *  @note    Mimic vbiVte_topDecode()
 *  @param   *pShowingPage   is the page address of current viewing.
\*****************************************************************************/
{
    TTX_TOP_Handle hTop = &ttxTopObj;
    TTX_STD_Page_AddrWithSubCode_t bttPageAddr;
    /* Obtain nowPageAddrDec */
    TTX_STD_Page_Addr_t         nowPageAddr;
    TTX_STD_Page_Address_Dec_t  nowPageAddrDec;
    TTX_STD_Page_Address_Dec_t blockPage, groupPage, nextPage, prevPage;
    TTX_FLOF_Key_t         flofKey;


    TTX_STD_MagazineNum_t magNum = pShowingPage->pageA >> 8;
    uint8_t pReceivedBttPage;  /* Points to the     received    BTT page (with    X/0) */
    uint8_t TTX_MEM* pDecodedBttPage;   /* Points to the Hamming decoded BTT page (without X/0) */

    if ( ! TTX_STD_MagNumIsValid( magNum ) )
        return( FALSE );

    if ( ! TTX_STD_isDisplayablePage2(pShowingPage) )
        return( FALSE );


    TTX_Log(LOGGER_VERBOSE, "top %x\n", (uint32_t) magNum);

    /* To ensure magNum is within valid range of hTop->btl.mag[] */
    if ( magNum == 8 )
        magNum = 0;

    if ( magNum >= TTX_MAG_NUM_TOTAL )
        return (FALSE);

    if (!TTX_TOP_GetBttPageAddr( &bttPageAddr, hTop->btl.mag[ magNum ] ) )
        return (FALSE);

#ifdef BUFFER_TOP_PAGE
    //TTX_Log(LOGGER_VERBOSE, "BTT page (%x.%x )\n", (uint32_t) bttPageAddr.pageA, (uint32_t) bttPageAddr.subcodeA);
    ttxBuf.BttPage.pageA = bttPageAddr.pageA;
    ttxBuf.BttPage.subcodeA = bttPageAddr.subcodeA;
#endif

    //    if( TTX_BUF_FindMainPage( &bttPageAddr, &pReceivedBttPage, &bttPageAddr ) )
    if (!TTX_BUF_FindPage( &bttPageAddr, &pReceivedBttPage, FALSE) )
        return (FALSE);

    TTX_BUF_DecodeBttPage( pReceivedBttPage ); /* Performs Hamming decode of pReceivedBttPage */
    pDecodedBttPage = TTX_BUF_GetDecodedBttPage();

    TTX_REQUIRE( pDecodedBttPage != NULL );
    if ( pDecodedBttPage == NULL )
        return( FALSE );


    /* Transforms from *pPageAddr to nowPageAddr. */
    if (!TTX_STD_RemoveSubCode( pShowingPage, &nowPageAddr ) )
        return (FALSE);


    /* Use nowPageAddr to obtain its decimal form: nowPageAddrDec */
    nowPageAddrDec = TTX_STD_PageAddrToDec( &nowPageAddr );

    /* Obtain TOP pages: blockPage, groupPage, nextPage and prevPage
    *   of nowPageAddrDec by using info provided by pDecodedBttPage.
    */
    if (!TTX_TOP_GetTopPages( pDecodedBttPage, nowPageAddrDec,              \
                              &blockPage, &groupPage, &nextPage, &prevPage, \
                              pbSubtitleAvailable, pSubtitlePage ) )
        return FALSE;


    /* Draw controls of TOP service */
    TTX_TOP_Display( pDecodedBttPage, blockPage, groupPage, pParam );

    flofKey = TTX_FLOF_KEY_RED;
    TTX_PageAddrDecToPageAddrWithSubCode( prevPage , &(pTop->flofKeys[flofKey]) );

    flofKey = TTX_FLOF_KEY_GREEN;
    TTX_PageAddrDecToPageAddrWithSubCode( nextPage, &(pTop->flofKeys[flofKey]) );
#if 0
#ifdef CONFIG_TTX_MS_TOP // morning star TOP
    TTX_PageAddrDecToPageAddrWithSubCode( groupPage, &(pTop->flofKeys[flofKey]) );
#else
    TTX_PageAddrDecToPageAddrWithSubCode( blockPage, &(pTop->flofKeys[flofKey]) ); // according to the definition of TT SPEC Annex I.2.1
#endif
#endif

    flofKey = TTX_FLOF_KEY_YELLOW;
    TTX_PageAddrDecToPageAddrWithSubCode( groupPage, &(pTop->flofKeys[flofKey]) );
#if 0
#ifdef CONFIG_TTX_MS_TOP // morning star TOP
    TTX_PageAddrDecToPageAddrWithSubCode( blockPage, &(pTop->flofKeys[flofKey]) );
#else
    TTX_PageAddrDecToPageAddrWithSubCode( groupPage, &(pTop->flofKeys[flofKey]) ); // according to the definition of TT SPEC Annex I.2.1
#endif
#endif

    flofKey = TTX_FLOF_KEY_CYAN;
    //TTX_PageAddrDecToPageAddrWithSubCode( nextPage , &(pTop->flofKeys[flofKey]) );
    TTX_PageAddrDecToPageAddrWithSubCode( blockPage, &(pTop->flofKeys[flofKey]) );

    pTop->bPrevPageReady = ( prevPage == nowPageAddrDec )? FALSE : TRUE;
    pTop->bNextPageReady = ( nextPage == nowPageAddrDec )? FALSE : TRUE;

    pTop->bAvailable = TRUE;
    TTX_BUF_SetBufStatus(pReceivedBttPage, TTX_BUFFER_DISPLAYED);
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
void TTX_TOP_Hide (void)
/*****************************************************************************\
 *  @brief   Hide FLOF/TOP
\*****************************************************************************/
{
    TTX_TOP_Display( NULL, 0, 0, NULL ); // we still have to check FLOF
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_FLOF_Decode (const TTX_STD_Page_AddrWithSubCode_t* pShowingPage,
                 TTX_FLOF_Service_t* pFlof, uint8_t idx)
/*****************************************************************************\
 *  @brief   Decode FLOF service info for page *pShowingPage.
 *  @param   *pShowingPage   is the page address of current viewing.
\*****************************************************************************/
{
    uint8_t showX24 = 0, i, M1, M2, M3;
    uint8_t TTX_MEM* pPageBuf = TTX_BUF_GetMemByIdx(idx);
    uint8_t TTX_MEM* pX27 = NULL;
#if SPARROW_LEVEL_15_MEMORY_LAYOUT
    uint8_t TTX_MEM* pCDX27 = (uint8_t TTX_MEM*) TTX_DEFAULT_CD_PAGE_ADDRESS + 41*40;
#else
    uint8_t TTX_MEM* pCDX27 = (uint8_t TTX_MEM*) TTX_TOP_BTT_PAGE_ADDRESS; // In this case, we don't have TOP, so we can use TOP buffer.
#endif
    uint8_t TTX_MEM* pDecodeBuf = &pCDX27[1];

    // default is not available
    pFlof->bAvailable = FALSE;

    if ( idx > 10 )
    {
        RTD_Log(LOGGER_INFO, "Invalid index\n");
        return FALSE;
    }

    if ( pPageBuf == NULL )
    {
        RTD_Log(LOGGER_INFO, "Invalid buf addr\n");
        return FALSE;
    }

#if SPARROW_LEVEL_15_MEMORY_LAYOUT
    pX27 = &pPageBuf[41*40]; // X/27/0 in row 41 of page buffer
#else
    pX27 = &pPageBuf[25*40]; // X/27/0 in row 25 of page buffer
#endif

    if ( (pX27[37] == 0x00) && (pX27[38] == 0x00) && (pX27[39] == 0x00) )   // LC && CRC are all zero
    {
        return FALSE; // X/27/0 is empty
    }

    pX27[0] = pX27[38] = pX27[39] = 0x15; // prevent decode error
    if ( VTE_Decode( VTE_CD_MODE_H84M0, pCDX27, pX27, 40 ) == TRUE )
    {
        showX24 = pCDX27[37]>>3;
    }
    else
    {
        RTD_Log(LOGGER_INFO, "fail to decode line 27\n");
        RTD_Log(LOGGER_INFO, "pX27 Addr=%x, pCDX27 Addr= %x (%x)\n", (uint32_t)pX27, (uint32_t)pCDX27); // address should be 4 alignment
        RTD_Log(LOGGER_INFO, "pX27[37]=%x, pCDX27[37]= %x\n", (uint32_t)pX27[37], (uint32_t)pCDX27[37]); // check decoded result
        return FALSE; // decode fail
    }

    // don't have to show links, so stop parsing.
    if ( showX24 == 0 )
    {
        return FALSE;
    }

    // available
    pFlof->bAvailable = TRUE;

    // get the page links.
    for ( i=0 ; i<TTX_FLOF_KEY_NUM ; ++i )
    {
        pFlof->flofKeys[i].pageA = ((pDecodeBuf[i*6+1]&0xF)<<4) + (pDecodeBuf[i*6]&0xF);

        if ( pFlof->flofKeys[i].pageA == 0xFF )
            continue; // no link.

        // Get magazine number
        if ( ((pDecodeBuf[i*6+3]>>3)&1) == 1 )
        {
            if ( ( (pShowingPage->pageA>>8) & 1 ) == 0 )
                M1 = 1;
            else
                M1 = 0;
        }
        else
        {
            M1 = (pShowingPage->pageA>>8) & 1;
        }

        if ( ((pDecodeBuf[i*6+5]>>2)&1) == 1 )
        {
            if ( ( (pShowingPage->pageA>>9) & 1 ) == 0 )
                M2 = 1;
            else
                M2 = 0;
        }
        else
        {
            M2 = (pShowingPage->pageA>>9) & 1;
        }

        if ( ((pDecodeBuf[i*6+5]>>3)&1) == 1 )
        {
            if ( ( (pShowingPage->pageA>>10) & 1) == 0 )
                M3 = 1;
            else
                M3 = 0;
        }
        else
        {
            M3 = (pShowingPage->pageA>>10) & 1;
        }

        pFlof->flofKeys[i].pageA |= ((4*M3+2*M2+M1)<<8);

        if ((pFlof->flofKeys[i].pageA >> 8)&0xF == 0)
            pFlof->flofKeys[i].pageA |= (8<<8); //set magzine 0 as mag 8

        // Get subpage
        pDecodeBuf[i*6+5] = pDecodeBuf[i*6+5] & 3;
        pDecodeBuf[i*6+3] = pDecodeBuf[i*6+3] & 7;

        pFlof->flofKeys[i].subcodeA =
            ((pDecodeBuf[i*6+5]&0x3)<<12) | ((pDecodeBuf[i*6+4]&0xF)<<8) |
            ((pDecodeBuf[i*6+3]&0x7)<<4) | (pDecodeBuf[i*6+2]&0xF);

        // 0x3F7F is any subpage is ok.
        if ( pFlof->flofKeys[i].subcodeA == 0x3F7F )
            pFlof->flofKeys[i].subcodeA = 0;

        // debug
        //RTD_Log(LOGGER_INFO, "[%d] page=%x, subpage=%x\n", (uint32_t)i, (uint32_t)pFlof->flofKeys[i].pageA, (uint32_t)pFlof->flofKeys[i].subcodeA );
    }

    return TRUE;
}



/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*/
static bool_t
TTX_TOP_GetTopPages (const uint8_t TTX_MEM*            pDecodedBttPage    ,
                     const TTX_STD_Page_Address_Dec_t  nowPage            ,
                     TTX_STD_Page_Address_Dec_t* pBlockPage         ,
                     TTX_STD_Page_Address_Dec_t* pGroupPage         ,
                     TTX_STD_Page_Address_Dec_t* pNextPage          ,
                     TTX_STD_Page_Address_Dec_t* pPrevPage          ,
                     uint8_t*                    pbSubtitleAvailable,
                     TTX_STD_Page_Addr_t*        pSubtitlePage        )
/*****************************************************************************
 *  @brief   Obtain the TOP pages address of magNum.
 *  @details Obtain TOP pages: *pBlockPage, *pGroupPage, *pNextPage and *pPrevPage
 *            of nowPage by using info provided by pDecodedBttPage.
 *  @note    Mimic vbiVte_topFindBttInfo()
 *  @param   *pDecodedBttPage  points to the starting address of Basic TOP Table
 *                               (without X/0).
 *  @param   nowPage           is current viewing page.
 *  @param   *pBlockPage       will be the found block    page address.
 *  @param   *pGroupPage       will be the found group    page address.
 *  @param   *pNextPage        will be the found next     page address.
 *  @param   *pPrevPage        will be the found previous page address.
\*****************************************************************************/
{
    uint16_t start_page_index, cnt, page_index;
    TTX_STD_TOP_BttCode_t page_type;

    cnt = 0;

    TTX_REQUIRE( ( 100 <= nowPage )&&( nowPage <= 899 ) );

    start_page_index = nowPage - 100 + 1;

//   	TTX_Log(LOGGER_VERBOSE, "t5 \n");

    if (   ( pBlockPage != NULL )  \
            &&( pGroupPage != NULL )  \
            &&( pNextPage  != NULL )  \
            &&( pPrevPage  != NULL )  )
    {
        /* Initialize *pBlockPage, *pGroupPage, *pNextPage and *pPrevPage to
         *   indicate they all need updated.
         */
        *pBlockPage = *pGroupPage = *pNextPage = *pPrevPage = nowPage;

//		   	TTX_Log(LOGGER_VERBOSE, "t6 %x\n", (uint32_t) *pPrevPage);

        while ( cnt < (TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM-1) )
        {
            /* find backward */
            if ( *pPrevPage == nowPage ) /* *pPrevPage needs update */
            {
                page_index = (start_page_index - cnt - 1 + TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM-1) % TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM;
                TTX_ENSURE( page_index < TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM );

                page_type  = pDecodedBttPage[ page_index ];
                if (   ( page_type >= TTX_STD_TOP_BTT_CODE_SUBTITLE_PAGE_A )  \
                        &&( page_type <= TTX_STD_TOP_BTT_CODE_NRM_PAGE_MA     )  )
                {
                    *pPrevPage = page_index + 100;  /* update *pPrevPage */
//		   	TTX_Log(LOGGER_VERBOSE, "tr %x\n", (uint32_t) *pPrevPage);
                }
            }

            /* find forward */
            if (   ( *pBlockPage == nowPage )  \
                    ||( *pGroupPage == nowPage )  \
                    ||( *pNextPage  == nowPage )  )
            {
                page_index = (start_page_index + cnt)%TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM;
                TTX_ENSURE( page_index < TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM );

                page_type  = pDecodedBttPage[ page_index ];

                if ( *pNextPage == nowPage ) /* *pNextPage needs update */
                {
                    if (   ( page_type >= TTX_STD_TOP_BTT_CODE_SUBTITLE_PAGE_A )  \
                            &&( page_type <= TTX_STD_TOP_BTT_CODE_NRM_PAGE_MA     )  )
                    {
                        *pNextPage = page_index + 100;  /* update *pNextPage */
//			   	TTX_Log(LOGGER_VERBOSE, "tn %x\n", (uint32_t) *pNextPage);

                    }
                }

                switch ( page_type )
                {
                case TTX_STD_TOP_BTT_CODE_PREV_BLK_PAGE_A:
                case TTX_STD_TOP_BTT_CODE_PREV_BLK_PAGE_MA:
                case TTX_STD_TOP_BTT_CODE_BLK_PAGE_A:
                case TTX_STD_TOP_BTT_CODE_BLK_PAGE_MA:
                {
                    if ( *pBlockPage == nowPage )   /* *pBlockPage needs update */
                    {
                        *pBlockPage = page_index + 100; /* update *pBlockPage */
//			   	TTX_Log(LOGGER_VERBOSE, "tb %x\n", (uint32_t) *pBlockPage);
                    }
                    break;
                }

                case TTX_STD_TOP_BTT_CODE_GRP_PAGE_A:
                case TTX_STD_TOP_BTT_CODE_GRP_PAGE_MA:
                {
                    if ( *pGroupPage == nowPage )   /* *pGroupPage needs update */
                    {
                        *pGroupPage = page_index + 100; /* update *pGroupPage */
//			   	TTX_Log(LOGGER_VERBOSE, "tg %x\n", (uint32_t) *pGroupPage);
                    }
                    break;
                }

                default:
                {
                    break;
                }
                }   /* end of switch( page_type ) */
            }

            /* loop-termination condition: all TOP pages are found */
            if (   ( *pBlockPage != nowPage )  \
                    &&( *pGroupPage != nowPage )  \
                    &&( *pNextPage  != nowPage )  \
                    &&( *pPrevPage  != nowPage )  )
            {
                break;
            }

            cnt++;  /* move on to next entry of Basic TOP Table */

        }   /* end of while( cnt < TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM ) */


        /* find subpage */
        /* @todo Merge this while() loop into above one */
        cnt = 0;
        while ( cnt < TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM - 1 )
        {
            // find forward
            page_index = (start_page_index + cnt )%TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM;
            TTX_ENSURE( page_index < TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM );

            page_type  = pDecodedBttPage[ page_index ];

            if ( page_type == TTX_STD_TOP_BTT_CODE_SUBTITLE_PAGE_A ) /* found */
            {
                bool_t              bSubtitleAvailable;
                TTX_STD_Page_Addr_t pageSubtitle;

                bSubtitleAvailable = TTX_STD_DecToPageAddr( page_index + 100, &pageSubtitle );

                if ( pbSubtitleAvailable != NULL )
                {
                    *pbSubtitleAvailable = bSubtitleAvailable;
                }

                if ( pSubtitlePage != NULL )
                {
                    *pSubtitlePage = pageSubtitle;
                }
                break;
            }

            cnt++;  /* move on to next entry of Basic TOP Table */
        }   /* end of while( cnt < TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM ) */


        /* Update */
        {
            const uint8_t TTX_MEM* pBtl = &( pDecodedBttPage[TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM+40*2+32] );
            for ( cnt = 0; cnt < TTX_MAG_NUM_TOTAL; cnt ++ )
            {
                /* @todo check error value of VTE_CD
                 *  if( pBtl[cnt] != VBIVTE_CD_DEFAULT_VALUE )
                 */
                {
                    TTX_TOP_Handle hTop = &ttxTopObj;
                    hTop->btl.mag[cnt] = pBtl[cnt];
                }
            }

        }

        return( TRUE );
    }

    return( FALSE );
}

#if 0
/*---------------------------------------------------------------------------*/
static bool_t
TTX_TOP_GetBttPageAddr ( TTX_STD_Page_AddrWithSubCode_t*     pTopPageAddr,
                         const TTX_STD_MagazineNum_t         magNum      ,
                         const TTX_STD_TOP_BttListCode_t     btlCode       )
/*****************************************************************************
 *  @brief   Obtain the BTT page number of magNum.
 *  @note    Mimic vbiVte_topFindBttPage()
 *  @param   *pTopPageAddr  is the computed corresponding BTT page address of
 *                            *pPageAddr.
 *  @param   magNum         is the page address of current page.
 *  @param   btlCode        is the content of Basic TOP Table List (BTL) associated
 *                            with magNum.
 *  @post    *pTopPageAddr  will be modified.
\*****************************************************************************/
{
    switch ( btlCode )
    {
    default:
    case TTX_STD_TOP_BTT_LIST_CODE_NO_DATA:
    {
//            TTX_REQUIRE( TTX_STD_MagNumIsValid( magNum ) );
//            if( TTX_STD_MagNumIsValid( magNum ) )
        if (  magNum )
        {
            if ( pTopPageAddr != NULL )
            {
                pTopPageAddr->pageA   = ( magNum <<8) | 0xF0;
                pTopPageAddr->subcodeA = 0;
                TTX_BUF_KeepPage( pTopPageAddr);
                return TRUE;
            }
        }
        break;
    }

    case 1: /* magazine-1 */
    case 2: /* magazine-2 */
    case 3: /* magazine-3 */
    case 4: /* magazine-4 */
    case 5: /* magazine-5 */
    case 6: /* magazine-6 */
    case 7: /* magazine-7 */
    case 8: /* magazine-8 */
    {
        if ( pTopPageAddr != NULL )
        {
//                TTX_REQUIRE( TTX_STD_MagNumIsValid( pTopPageAddr->magNum ) );
            pTopPageAddr->pageA  = (btlCode<<8) | 0xF0;
            pTopPageAddr->subcodeA = 0;

            TTX_BUF_KeepPage( pTopPageAddr);
            return TRUE;

        }
        break;
    }

    case TTX_STD_TOP_BTT_LIST_CODE_EXT1:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT2:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT3:
    case TTX_STD_TOP_BTT_LIST_CODE_MAG_WITHOUT_TOP:
    case TTX_STD_TOP_BTT_LIST_CODE_MAG_NOT_TRANSMITTED:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT4:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT5:
    {
        /* do nothing */
        break;
    }

    }

    return( FALSE );
}
#endif

static bool_t TTX_TOP_GetBttPageAddr (
    TTX_STD_Page_AddrWithSubCode_t* pTopPageAddr,
    const TTX_STD_TOP_BttListCode_t btlCode )
/*****************************************************************************\
 * @brief		Obtain the BTT page number of magNum.
 * @note		Mimic vbiVte_topFindBttPage()
 * @param	<pTopPageAddr>	point to BTT page address
 * @param	<btlCode>		is the content of Basic TOP Table List (BTL)
 *							associated with magNum.
 * @post		*pTopPageAddr will be modified.
\*****************************************************************************/
{
    switch ( btlCode )
    {
    default:
    case TTX_STD_TOP_BTT_LIST_CODE_NO_DATA:
    {
        pTopPageAddr->pageA   = ( 1 <<8) | 0xF0;
        pTopPageAddr->subcodeA = 0;
        TTX_BUF_KeepPage( pTopPageAddr);
        return(TRUE);
    }

    case 1: /* magazine-1 */
    case 2: /* magazine-2 */
    case 3: /* magazine-3 */
    case 4: /* magazine-4 */
    case 5: /* magazine-5 */
    case 6: /* magazine-6 */
    case 7: /* magazine-7 */
    case 8: /* magazine-8 */
    {
        if ( pTopPageAddr != NULL )
        {
            pTopPageAddr->pageA  = (btlCode<<8) | 0xF0;
            pTopPageAddr->subcodeA = 0;
            TTX_BUF_KeepPage( pTopPageAddr);
            return TRUE;
        }
        break;
    }

    case TTX_STD_TOP_BTT_LIST_CODE_EXT1:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT2:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT3:
    case TTX_STD_TOP_BTT_LIST_CODE_MAG_WITHOUT_TOP:
    case TTX_STD_TOP_BTT_LIST_CODE_MAG_NOT_TRANSMITTED:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT4:
    case TTX_STD_TOP_BTT_LIST_CODE_EXT5:
    {
        /* do nothing */
        break;
    }

    }

    return( FALSE );
}

extern uint8_t TTX_SRC_isDisplayablePage (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress);

/*---------------------------------------------------------------------------*/
static bool_t
TTX_TOP_GetAitTitle ( const uint8_t TTX_MEM* pDecodedBttPage,
                      const TTX_STD_Page_AddrWithSubCode_t* pTopPageAddr,
                      uint8_t aitTitle [TTX_TOP_AIT_TITLE_SIZE] )
/*****************************************************************************\
 *  @brief   Parse page-linking table of BTT page to obtain the AIT-title[].
 *  @note    Mimic vbiVte_topFindLinkInfo().
 *  @note    Refer to ARD/ZDF p.30.
 *  @param   *pDecodedBttPage  points to the starting address of Basic TOP Table
 *                               (without X/0).
\*****************************************************************************/
{
    const uint8_t pltFields = 10;    /* number of fields of Page-Linking Table */
    uint8_t i;

    TTX_STD_Page_AddrWithSubCode_t linkPageAddr;

    TTX_REQUIRE( pDecodedBttPage != NULL );
    if ( pDecodedBttPage != NULL )
    {
        /* Page-Linking Table is just located after Basic TOP Table in a BTT page*/
        const uint8_t TTX_MEM* pPlt = pDecodedBttPage + TTX_STD_TOP_BASIC_TOP_TABLE_ENTRY_NUM;
        const uint8_t TTX_MEM* pField;

        for ( i = 0; i<pltFields; i++ )
        {
            /* The number of bytes of a field of Page-Linking Table is 8 bytes,
             *   so we shift i left 3 bits to times eight to i
             */
            pField = (pPlt + (i << 3) );

            linkPageAddr.pageA =
                (((uint16_t)pField[0] & 0x0F) << 8) | // magazine
                (((uint16_t)pField[1] & 0x0F) << 4) | // page number tens
                ((uint16_t)pField[2] & 0x0F) ; // page number units
            linkPageAddr.subcodeA =
                (((uint16_t)pField[3] & 0x03) <<12) | // s4: upper subcode tens
                (((uint16_t)pField[4] & 0x0F) <<8) | // s3: upper subcode units
                (((uint16_t)pField[5] & 0x07) <<4) | // s2: lower subcode tens
                ((uint16_t)pField[6] & 0x0F); // s1: lower subcode units

            switch ( pField[7] & 0x0F ) /* table identification */
            {
            case 0x2: /* additional information table */
            {
                //TTX_BUF_KeepPage( &linkPageAddr); // imply in TTX_TOP_GetAitTitleFromAit()
                if ( TTX_TOP_GetAitTitleFromAit( &linkPageAddr, pTopPageAddr, aitTitle ) )
                {
                    return( TRUE );
                }
                else
                    break;
            }

            case 0x3: /* multipage extension table */
            {
                //TTX_BUF_KeepPage( &linkPageAddr); // imply in TTX_TOP_GetAitTitleFromMpe()
                if ( TTX_TOP_GetAitTitleFromMpe( &linkPageAddr, pTopPageAddr, aitTitle ) )
                {
                    return( TRUE );
                }
                else
                    break;
            }

            case 0xF: /* end symbol */
            {
                return( FALSE );
            }

            case 0x1: /* multipage table */
            default:
            {
                break;
            }
            }
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_TOP_GetAitTitleFromAit ( const TTX_STD_Page_AddrWithSubCode_t* pAitPageAddr,
                             const TTX_STD_Page_AddrWithSubCode_t* pDispPageAddr,
                             uint8_t aitTitle[TTX_TOP_AIT_TITLE_SIZE])
/*****************************************************************************
 *  @brief   Find AIT page (*pAitPageAddr) from page buffer and descramble it
 *             to find the AIT title (aitTitle[]) of *pDispPageAddr.
 *  @note    Mimic vbiVte_topFindAitInfo().
 *  @param   *pAitPageAddr  points to the page address of an AIT page.
\*****************************************************************************/
{
    TTX_REQUIRE( pAitPageAddr  != NULL );
    TTX_REQUIRE( pDispPageAddr != NULL );
    if ( ( pAitPageAddr != NULL ) && ( pDispPageAddr != NULL ) )
    {
        uint8_t pAitPage;
        static uint8_t count = 0;

        TTX_BUF_KeepPage( pAitPageAddr );

#ifdef BUFFER_TOP_PAGE
        if ((pAitPageAddr->pageA != 0)&&(!TTX_SRC_isDisplayablePage(pAitPageAddr)))
        {
            if ((ttxBuf.Ait_Mpe_Page[0].pageA != pAitPageAddr->pageA)&&(ttxBuf.Ait_Mpe_Page[1].pageA != pAitPageAddr->pageA))
            {
                if (ttxBuf.Ait_Mpe_Page[0].pageA == 0)
                {
                    ttxBuf.Ait_Mpe_Page[0].pageA= pAitPageAddr->pageA;
                    ttxBuf.Ait_Mpe_Page[0].subcodeA= pAitPageAddr->subcodeA;
                }
                else if (ttxBuf.Ait_Mpe_Page[1].pageA == 0)
                {
                    ttxBuf.Ait_Mpe_Page[1].pageA= pAitPageAddr->pageA;
                    ttxBuf.Ait_Mpe_Page[1].subcodeA= pAitPageAddr->subcodeA;
                }
                else
                {
                    count = (count+1) % BUFFER_MAX_AIT_NUM;
                    ttxBuf.Ait_Mpe_Page[count].pageA= pAitPageAddr->pageA;
                    ttxBuf.Ait_Mpe_Page[count].subcodeA= pAitPageAddr->subcodeA;
                }
            }
        }
        /*
        			   RTD_Log(LOGGER_INFO, "pAitPageAddr (%x.%x), AIT 1 (%x.%x), AIT 2 (%x.%x) \n", (UINT32) pAitPageAddr->pageA, (UINT32) pAitPageAddr->subcodeA,
        			   	(UINT32) ttxBuf.Ait_Mpe_Page[0].pageA, (UINT32) ttxBuf.Ait_Mpe_Page[0].subcodeA,
        			   	(UINT32) ttxBuf.Ait_Mpe_Page[1].pageA, (UINT32) ttxBuf.Ait_Mpe_Page[1].subcodeA);
        */
#endif

        // perfect match
        if ( TTX_BUF_FindPage( pAitPageAddr, &pAitPage, FALSE ) )
        {
            const uint8_t aitFields    = 2 * 22;   /* number of fields of an AIT talbe */
            const uint8_t aitFieldSize = 20;       /* number of bytes of a field of an AIT talbe */
            uint8_t i;
            TTX_STD_Page_AddrWithSubCode_t aitPageAddr;
            uint8_t TTX_MEM* pField;

            TTX_BUF_DecodeAitPage( pAitPage );

            for ( i=0; i < aitFields; i++ )
            {
                pField = TTX_BUF_GetDecodedPacket( (i+2)>>1 ) + (i%2)*aitFieldSize; /* each packet has two fields */
                /* magazine number 1~8 */
                aitPageAddr.pageA = ((pField[0] & 0x0F) << 8) |  ((pField[1] & 0x0F)<<4) | (pField[2] & 0x0F) ;

                if ( pDispPageAddr->pageA == aitPageAddr.pageA ) // found
                {
                    memcpy( aitTitle, &(pField[8]), TTX_TOP_AIT_TITLE_SIZE );
                    TTX_BUF_SetBufStatus(pAitPage, TTX_BUFFER_DISPLAYED);
                    return( TRUE );
                }
            }
            TTX_BUF_SetBufStatus(pAitPage, TTX_BUFFER_DISPLAYED);
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_TOP_GetAitTitleFromMpe ( const TTX_STD_Page_AddrWithSubCode_t* pMpePageAddr,
                             const TTX_STD_Page_AddrWithSubCode_t* pDispPageAddr,
                             uint8_t aitTitle[TTX_TOP_AIT_TITLE_SIZE])
/*****************************************************************************\
 *  @brief   Find MPE page (*pMpePageAddr) from page buffer and descramble it
 *             to find the AIT title (aitTitle[]) of *pDispPageAddr.
 *  @note    Mimic vbiVte_topFindMpeInfo().
 *  @param   *pMpePageAddr  points to the page address of an Multi-Page Extension page.
\*****************************************************************************/
{
    bool_t ret = FALSE;
    TTX_REQUIRE( pMpePageAddr  != NULL );
    TTX_REQUIRE( pDispPageAddr != NULL );
    if ( ( pMpePageAddr != NULL ) && ( pDispPageAddr != NULL ) )
    {
        uint8_t pMpePage;
        TTX_BUF_KeepPage( pMpePageAddr );

        // perfect match
        if ( TTX_BUF_FindPage( pMpePageAddr, &pMpePage, FALSE ) )
        {
            const uint8_t mpeFields    = 5 * 22;   /* number of fields of an MPE talbe */
            uint8_t i;
            TTX_STD_Page_AddrWithSubCode_t mpePageAddr;
            uint8_t TTX_MEM* pField;

            TTX_BUF_DecodeMpePage( pMpePage );

            for ( i=0; i < mpeFields; i++ )
            {
                /* Why shift left by 3 bits? Because the number of bytes of a field of an MPE tabel is
                 *   eight bytes, so we shift left by three.
                 */
                pField = TTX_BUF_GetDecodedPacket( (i+5)/5 ) + ((i%5)<<3); /* each packet has five fields */

                mpePageAddr.pageA =
                    ((pField[0] & 0x0F) << 8) | // magazine
                    ((pField[1] & 0x0F) << 4) | // page number tens
                    (pField[2] & 0x0F); // page number units
                mpePageAddr.subcodeA =
                    ((pField[3] & 0x03) <<12) | // s4: upper subcode tens
                    ((pField[4] & 0x0F) << 8) | // s3: upper subcode units
                    ((pField[5] & 0x07) << 4) | // s2: lower subcode tens
                    (pField[6] & 0x0F); // s1: lower subcode units


                switch ( pField[7] & 0x0F ) /* table identification */
                {
                case 0x2: /* additional information table */
                {
                    ret = TTX_TOP_GetAitTitleFromAit( &mpePageAddr, pDispPageAddr, aitTitle );
                    TTX_BUF_SetBufStatus(pMpePage, TTX_BUFFER_DISPLAYED);

                    if ( ret )
                        return ret;
                    else
                        break;
                }

#ifndef __KEIL__    /* KeilC 8051 compiler doesn't support recursive calls */
                case 0x3: /* multipage extension table */
                {
                    ret = TTX_TOP_GetAitTitleFromMpe( &mpePageAddr, pDispPageAddr, aitTitle );
                    TTX_BUF_SetBufStatus(pMpePage, TTX_BUFFER_DISPLAYED);

                    if ( ret )
                        return ret;
                    else
                        break;
                }
#endif  /* end of #ifndef __KEIL__ */

                case 0xF: /* end symbol */
                {
                    return( FALSE );
                }

                case 0x1: /* multipage table */
                default:
                {
                    break;
                }
                }
            }
            TTX_BUF_SetBufStatus(pMpePage, TTX_BUFFER_DISPLAYED);

        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static void TTX_TOP_Display (
    const uint8_t TTX_MEM* pDecodedBttPage,
    const TTX_STD_Page_Address_Dec_t blockPage,
    const TTX_STD_Page_Address_Dec_t groupPage,
    const TTX_LopParam_t* pParam )
/*****************************************************************************\
 *  @brief   Draw controls of TOP services.
 *  @note    Mimic vbiVte_topDisplay().
 *  @param   *pDecodedBttPage  points to the starting address of Basic TOP Table
 *                               (without X/0). If you leave this param == NULL,
 *                               this __FUNCTION__ will hide the TOP control bar.
\*****************************************************************************/
{
    const uint8_t row = TTX_DISP_TOP_CTRL_ROW_NUM;

    TTX_DISP_Attr_t attr;

    uint8_t aitTitle[TTX_TOP_AIT_TITLE_SIZE];
    bool_t  bAitTitleFound = FALSE;
    TTX_STD_Page_AddrWithSubCode_t pageAddr;
    uint8_t i;

    if ( pDecodedBttPage == NULL )  /* Hide TOP control bar */
    {
        memset( &attr, 0, sizeof( attr ) );

        attr.conceal = TRUE;

        TTX_DISP_FillRowAttr( row, &attr );
    }
    else    /* Show TOP control bar */
    {
        /* Block */
        {
            const uint8_t aitTitleDefault[TTX_TOP_AIT_TITLE_SIZE] = "NEXT BLOCK  ";

            memset( aitTitle, 0, TTX_TOP_AIT_TITLE_SIZE ); /* clear ait_title[] */

            /* get AIT block title info */
            {
                if ( TTX_PageAddrDecToPageAddrWithSubCode( blockPage, &pageAddr ) )
                {
                    bAitTitleFound = TTX_TOP_GetAitTitle( pDecodedBttPage, &pageAddr, aitTitle );
                }
            }

            /* Display */
            {
                memset( &attr, 0, sizeof( attr ) );

                if ( pParam == NULL )
                    attr.charset = TTX_CHARSET_G0_LATIN;
                else
                    attr.charset = pParam->DEFAULT_G0_CHARSET;

                attr.fgColorIdx = TTX_COLOR_BLACK;
                attr.bgColorIdx = TTX_COLOR_CYAN;

                for ( i=0 ; i<TTX_TOP_CYAN_SIZE ; i++ )
                {
                    if ( (i<2) || (i>=(2+TTX_TOP_AIT_TITLE_SIZE)) )
                    {
                        attr.charIdx = ' ';
                    }
                    else if ( bAitTitleFound )
                    {
                        attr.charIdx = aitTitle[i-2];
                    }
                    else
                    {
                        attr.charIdx = aitTitleDefault[i-2];
                    }

                    TTX_DISP_SetAttr( row, 26+i, &attr ); // according to the definition of TT SPEC Annex I.2.1
                }

            }
        }

        /* Group */
        {
            const uint8_t aitTitleDefault[TTX_TOP_AIT_TITLE_SIZE] = "NEXT GROUP  ";

            memset( aitTitle, 0, TTX_TOP_AIT_TITLE_SIZE ); /* clear ait_title[] */

            /* get AIT block title info */
            {
                if ( TTX_PageAddrDecToPageAddrWithSubCode( groupPage, &pageAddr ) )
                {
                    bAitTitleFound = TTX_TOP_GetAitTitle( pDecodedBttPage, &pageAddr, aitTitle );
                }
            }

            /* Display */
            {
                memset( &attr, 0, sizeof( attr ) );

                if ( pParam == NULL )
                    attr.charset = TTX_CHARSET_G0_LATIN;
                else
                    attr.charset = pParam->DEFAULT_G0_CHARSET;
                attr.fgColorIdx = TTX_COLOR_BLACK;
                attr.bgColorIdx = TTX_COLOR_YELLOW;

                for ( i=0 ; i<TTX_TOP_YELLOW_SIZE ; i++ )
                {
                    if ( (i<2) || (i>=(2+TTX_TOP_AIT_TITLE_SIZE)) )
                    {
                        attr.charIdx = ' ';
                    }
                    else if ( bAitTitleFound )
                    {
                        attr.charIdx = aitTitle[i-2];
                    }
                    else
                    {
                        attr.charIdx = aitTitleDefault[i-2];
                    }
                    TTX_DISP_SetAttr( row, 11+i, &attr ); // according to the definition of TT SPEC Annex I.2.1
                }
            }
        }
        TTX_TOP_Display_Prev_Next();
    }
}


/*---------------------------------------------------------------------------*/
static void TTX_TOP_Display_Prev_Next (void)
/*****************************************************************************\
 * @brief		Draw navigation sign of previous page and next page
\*****************************************************************************/
{
    const uint8_t row = TTX_DISP_TOP_CTRL_ROW_NUM;
    TTX_DISP_Attr_t attr;
    uint8_t i;

    /* Draw first black */
    {
        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_BLACK;
        attr.charIdx = 0x20;
        TTX_DISP_SetAttr( row, 0, &attr );
    }

    /* Draw previous page */
    {
        enum { len = 5 };
        const uint8_t label[len] = "  -  ";

        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_RED;

        for ( i=0 ; i<len ; i++ )
        {
            attr.charIdx = label[i];
            TTX_DISP_SetAttr( row, 1+i, &attr );
        }
    }

    /* Draw next page */
    {
        enum { len = 5 };
        const uint8_t label[len] = "  +  ";

        memset( &attr, 0, sizeof( attr ) );

        attr.charset = TTX_CHARSET_G0_LATIN;
        attr.fgColorIdx = TTX_COLOR_BLACK;
        attr.bgColorIdx = TTX_COLOR_GREEN;

        for ( i=0 ; i<len ; i++ )
        {
            attr.charIdx = label[i];
            TTX_DISP_SetAttr( row, 6+i, &attr );
        }
    }
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_PageAddrDecToPageAddrWithSubCode (const TTX_STD_Page_Address_Dec_t   from,
                                      TTX_STD_Page_AddrWithSubCode_t* pTo    )
/*****************************************************************************\
 *  @brief   Convert TTX_STD_Page_Addr_t to TTX_STD_Page_AddrWithSubCode_t
 *             with zero SubCode
\*****************************************************************************/
{
    TTX_STD_Page_Addr_t pageAddr;
    if ( TTX_STD_DecToPageAddr( from, &pageAddr ) )
    {
        return( TTX_ExpandPageAddressWithZeroSubCode( &pageAddr, pTo ) );
    }
    return( FALSE );
}


/*---------------------------------------------------------------------------*/
static bool_t
TTX_ExpandPageAddressWithZeroSubCode (const TTX_STD_Page_Addr_t*      pFrom,
                                      TTX_STD_Page_AddrWithSubCode_t* pTo)
/*****************************************************************************\
 *  @brief   Convert TTX_STD_Page_Addr_t to TTX_STD_Page_AddrWithSubCode_t
 *             with zero SubCode
\*****************************************************************************/
{
    if ( ( pFrom != NULL ) && ( pTo != NULL ) )
    {
        pTo->pageA =(pFrom->magNum<<8) | (pFrom->pageNum.tens<<4) |(pFrom->pageNum.units) ;
        pTo->subcodeA = 0;

        return( TRUE );
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*\
 * end of ttx_dec_top.c                                                      *
\*---------------------------------------------------------------------------*/
#endif
