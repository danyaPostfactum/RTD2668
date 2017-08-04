/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_buff.c
 * @brief    Teletext page-buffer management.
 * @author   Janus Cheng
 * @date     Nov. 11, 2008
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_buff.h> /* Provides: functions to be implemented */
#include <ttx_buff_pkg.h> /* Provides: TTX_DEFAULT_CD_PAGE_ADDRESS */
#include <ttx_log.h> /* Provides: OSD_Log( LOGGER_WARN,  () */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */
#include <ttx_std.h> /* Provides: TTX_STD_Page_AddrWithSubCode_Hex_t */
#include <ttx_dec.h> /* Provides: TTX_X26Function_t, TTX_X26Info_t */

#include <string.h> /* Provides: memset() */
#include <stdint.h> /* Provides: UINT_LEAST8_MAX */

#include <vte.h> /* Provides: VTE_Decode() */
#include <reg52.h> /* Provides: VTE_Decode() */

TTX_DEFINE_THIS_MODULE( ttx_buff.c )



/* -- BUF constants ---------------------------------------------------------*/
TTX_ASSERT_COMPILE( TTX_MAX_PAGE_KEEP_NUM <= TTX_MAX_PAGE_NUM );


#if TTX_BUF_SOFTWARE_EMULATOR
#if TTX_USE_MALLOC
#include <malloc.h>     /* Provides: malloc() */
#else
#if TTX_MAX_PAGE_NUM <= 10
TTX_ASSERT_COMPILE( TTX_CFG_PRESENT_LEVEL == TTX_STD_PRESENT_LEVEL(15) );       /* Becasue ( _TTX_MAX_ROW_LEN == 40 )*/
#define _STATIC_PAGE_MEM_SIZE  (0x4000) /* the memory size in bytes used when not using TTX_USE_MALLOC */
#else
/* @todo The following size is only an APPROXIMATION. Try to use more accurate one. */
#define _STATIC_PAGE_MEM_SIZE  ((TTX_MAX_PAGE_NUM+10)*TTX_DEFAULT_PAGE_SIZE) /* the memory size in bytes used when not using TTX_USE_MALLOC */
#endif

static uint8_t _gpPageMemBase [_STATIC_PAGE_MEM_SIZE];
#endif
void TTX_MEM* TTX_gpPageMemAligned = NULL;  /* To meet VTE_Decode()'s constraint, this pointer points to 4-byte and 40-byte aligned memory */
#else
TTX_ASSERT_COMPILE( TTX_DEFAULT_PAGE_LAST_ADDR    <= 0xB3FF );
#endif  // end of #if TTX_BUF_SOFTWARE_EMULATOR

typedef enum TTX_BUF_DecodePageType_t
{
    TTX_BUF_DECODE_PAGE_NORMAL,
    TTX_BUF_DECODE_PAGE_AIT,
    TTX_BUF_DECODE_PAGE_MPE,
    TTX_BUF_DECODE_PAGE_X24,
    TTX_BUF_DECODE_PAGE_X0
} TTX_BUF_DecodePageType_t;


/* -- Prototypes of Private Functions ---------------------------------------*/
bool_t               TTX_BUF_KeepQueFindPage                 ( const TTX_STD_Page_AddrWithSubCode_t* pPageAddr, uint8_t * idx);
static void                 TTX_BUF_KeepQuePushPage               (       const TTX_STD_Page_AddrWithSubCode_t* pPageToIns);
static void                 TTX_BUF_KeepQuePopPage                (       const TTX_STD_Page_AddrWithSubCode_t* pPageToDel);
static void                 TTX_BUF_KeepQuePopOldestPage          (      );
static bool_t               TTX_BUF_GetPrevDiffMainPageIdx        ( const TTX_BUF_PageListIdx_t idxStart  , const TTX_BUF_PageListIdx_t* pIdxStop, TTX_BUF_PageListIdx_t* pIdxFound);
static bool_t               TTX_BUF_GetNextDispDiffMainPageIdx    ( const TTX_BUF_PageListIdx_t idxShowing, TTX_BUF_PageListIdx_t* pIdxFound);
static bool_t               TTX_BUF_GetPrevDispDiffMainPageIdx    ( const TTX_BUF_PageListIdx_t idxShwoing, TTX_BUF_PageListIdx_t* pIdxFound);
static bool_t               TTX_BUF_GetNextSubPageIdx             ( const TTX_BUF_PageListIdx_t idxStart, const TTX_BUF_PageListIdx_t* pIdxStop, TTX_BUF_PageListIdx_t* pIdxFound);
static bool_t               TTX_BUF_GetPrevSubPageIdx             ( const TTX_BUF_PageListIdx_t idxStart, const TTX_BUF_PageListIdx_t* pIdxStop, TTX_BUF_PageListIdx_t* pIdxFound);

static bool_t TTX_BUF_DecodePageHelper (const TTX_BUF_DecodePageType_t pageType, uint8_t  src);
static bool_t TTX_BUF_DecodeNormalPageHelper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src);
static bool_t TTX_BUF_DecodeAitPageHelper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src);
static bool_t TTX_BUF_DecodeMpePageHelper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src);
static bool_t TTX_BUF_DecodeX24Helper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src);
static bool_t TTX_BUF_DecodeX0Helper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src);
static bool_t TTX_BUF_DecodeX26Helper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src);

static TTX_X26Function_t TTX_BUF_GetX26Function(uint8_t x26_address, uint8_t x26_mode);

#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */
static bool_t               TTX_BUF_TestSoftware                  (void);
#endif  /* end of #if TTX_VERIFY */


#define TTX_BUF_KeepQueIsEmpty()        (ttxBuf.keepQue.cnt == 0)
#define TTX_BUF_KeepQueIsFull()         (ttxBuf.keepQue.cnt >= TTX_MAX_PAGE_KEEP_NUM)
#define TTX_PACKET_SIZE                 (40)        /* bytes */

uint8_t ttxbuf_pageList[80];
extern void TTX_NAV_SetPageToShow(const TTX_STD_Page_AddrWithSubCode_t* pPageToShow );


TTX_BUF_Obj ttxBuf =                          \
{
    // pageList
    {
        // pageToShow
        {
            0x0100, 0x0000, // pageA, 	subcodeA
            0,                      // idx
            FALSE                   // bIsBuffered
        },
        // pageShowing
        {
            0x0100, 0x0000, // pageA, 	subcodeA
            0,                      // idx
            FALSE                   // bIsBuffered
        },

        // elements
#if SPARROW_LEVEL_15_MEMORY_LAYOUT
#if 1
//#ifdef CONFIG_TTX_PFE_BUG
        {
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
        }
#else
        {
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
        }
#endif
#else
#ifdef CONFIG_TTX_PFE_BUG
        {
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, TTX_X27_EMPTY, NULL },  \
        }
#else
        {
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
            { 0, 0, 0, TTX_BUFFER_IDLE, NULL },  \
        }
#endif
#endif
    },
    // keepQue
    {
        // elements
        {
            {0x01000000},  \
            {0x01000000},  \
            {0x01000000}   \
        },
        0   // cnt
    },
#ifdef BUFFER_TOP_PAGE
    // Btt page
    {
        0, 0
    },
    // Ait Mip page
    {
        {0,0}, \
        {0,0}
    }
#endif
};


/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
void
TTX_BUF_Open                     (void)
/*****************************************************************************\
 *  @brief   reinitialize all space to freely available
\*****************************************************************************/
{
    TTX_ENSURE( TTX_PAGE_MEM_START_ADDRESS %  4 == 0 );         // ERROR: TTX_PAGE_MEM_START_ADDRESS is mis-aligned to work in VTE_Decode().
    TTX_ENSURE( TTX_PAGE_MEM_START_ADDRESS % 40 == 0 );         // ERROR: TTX_PAGE_MEM_START_ADDRESS is mis-aligned to work in VTE_Decode().

    VTE_Open( VTE_OPEN_RESET );

    TTX_BUF_Reset();
}

/*---------------------------------------------------------------------------*/
void
TTX_BUF_Close                     (void)
/*****************************************************************************\
 *  @brief   reinitialize all space to freely available
\*****************************************************************************/
{
#if TTX_BUF_SOFTWARE_EMULATOR

    TTX_REQUIRE( ttxBuf.cntUsage > 0 );  // ERROR: Unbalanced TTX_DISP_Open() and TTX_DISP_Close()

    ttxBuf.cntUsage--;

    if ( ttxBuf.cntUsage == 0 )
    {
        if ( ttxBuf.pAddrBase != NULL )
        {
#if TTX_USE_MALLOC
            free( ttxBuf.pAddrBase );
#endif  /* end of #if TTX_USE_MALLOC */
        }
        ttxBuf.pAddrBase      = NULL;
        TTX_gpPageMemAligned = NULL;
    }



#endif  /* end of #if TTX_BUF_SOFTWARE_EMULATOR */

    VTE_Close();

    TTX_BUF_Reset();
}


/*---------------------------------------------------------------------------*/
void
TTX_BUF_Reset                     (void)
/*****************************************************************************\
 *  @brief   reinitialize all space to freely available
\*****************************************************************************/
{
    // Reset page queue
    {
        TTX_BUF_PageListIdx_t i;
        for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
        {
            //ttxBuf.pageList.elements[i].addrMem = NULL;   // mark this space as freely available
            ttxBuf.pageList.elements[i].addrMem  = ( void TTX_MEM* )( TTX_DEFAULT_PAGEx_ADDRESS(i) );
            ttxBuf.pageList.elements[i].pageA = 0;
            ttxBuf.pageList.elements[i].subcodeA = 0;
            ttxBuf.pageList.elements[i].collecting_count = 0;
            ttxBuf.pageList.elements[i].X27_status = TTX_X27_EMPTY;
            ttxBuf.pageList.elements[i].keep = 0;
            ttxBuf.pageList.elements[i].bstatus = TTX_BUFFER_IDLE;
        }

        for ( i=0; i<10; i++ )   // initial subPage data.
        {
            ttxBuf.pageList.subPageIndicator[i]=0;
        }
        //	ttxBuf.pageList.elements[TTX_MAX_PAGE_NUM] is always used for X27

        ttxBuf.pageList.pageToShow.bIsBuffered = FALSE;
        ttxBuf.pageList.pageToShow.idx         = 0;
        ttxBuf.pageList.pageToShow.pageA = 0x000;
        ttxBuf.pageList.pageToShow.subcodeA = 0;
        ttxBuf.pageList.pageShowing = ttxBuf.pageList.pageToShow;
        ttxBuf.pageList.pageShowing.pageA = 0x0;

#ifdef BUFFER_TOP_PAGE
        ttxBuf.BttPage.pageA = 0x1F0;
        ttxBuf.BttPage.subcodeA= 0;
        for (i=0; i<BUFFER_MAX_AIT_NUM; i++)
        {
            ttxBuf.Ait_Mpe_Page[i].pageA = 0;
            ttxBuf.Ait_Mpe_Page[i].subcodeA= 0;
        }
#endif

    }

    // Reset keep queue
    {
        TTX_BUF_PageListIdx_t i;

        for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
        {
            ttxBuf.keepQue.elements[i].pageA = 0;
            ttxBuf.keepQue.elements[i].subcodeA= 0;
            ttxBuf.keepQue.elements[i].bindex = 0xFF;
        }
        ttxBuf.keepQue.cnt = 0;
    }

//    TTX_ENSURE( TTX_BUF_GetFreeSpace()  == TTX_MAX_PAGE_NUM );
//    TTX_ENSURE( TTX_BUF_isEmpty()       == TRUE  );
//    TTX_ENSURE( TTX_BUF_isFull()        == FALSE );
//    TTX_ENSURE( TTX_BUF_KeepQueIsFull() == FALSE );
}


/*---------------------------------------------------------------------------*/
uint8_t
TTX_BUF_PageListGetIdx (
    const TTX_STD_Page_AddrWithSubCode_t*  pPageToFind,
    TTX_BUF_PageListIdx_t*                 pIdxFound)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.records[] whose page address is the
 *              the same as *pPageAddr and the records[] is currently been used.
 *  @note    When bCompareSubPage is set to FALSE, this function will returns
 *             the least SubCode page available.
 *  @param   [in]  *pPageAddr is the page address in page buffer.
\*****************************************************************************/
{
    uint8_t i, far_index=0;
    uint32_t dist=0, far_dist=0;

    // check if it is already in buffer
    for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
    {
        if ((ttxBuf.pageList.elements[i].pageA == pPageToFind->pageA) &&
                (ttxBuf.pageList.elements[i].subcodeA == pPageToFind->subcodeA))    // already in buffer
        {
            *pIdxFound = i;
            return TRUE;
        }
    }
    return( FALSE );
}

/*---------------------------------------------------------------------------*/
void
TTX_BUF_SetPageToShow (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   get number of pages free
 *  @note    Since rule of page memory mangement is based on currently
 *              displaying page, so let TTX_BUF now which page is currently
 *              displaying is very important.
 *  @ret     the number of pages can be TTX_BUF_InsPage()
\*****************************************************************************/
{
    uint8_t i;

    EA=0;
    ttxBuf.pageList.pageToShow.bIsBuffered = TTX_BUF_PageListGetIdx(  pPageAddr, &(ttxBuf.pageList.pageToShow.idx));
    ttxBuf.pageList.pageToShow.pageA= pPageAddr->pageA;
    ttxBuf.pageList.pageToShow.subcodeA= pPageAddr->subcodeA;
    if (pPageAddr->subcodeA==0)
    {
        for ( i=0; i<10; i++ )    // initial subPage data.
        {
            ttxBuf.pageList.subPageIndicator[i]=0;
        }
    }
    EA=1;

    TTX_BUF_KeepPage( pPageAddr);
}

/*---------------------------------------------------------------------------*/
void
TTX_BUF_SetShowingPage (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   get number of pages free
 *  @note    Since rule of page memory mangement is based on currently
 *              displaying page, so let TTX_BUF now which page is currently
 *              displaying is very important.
 *  @ret     the number of pages can be TTX_BUF_InsPage()
\*****************************************************************************/
{
    EA=0;
    ttxBuf.pageList.pageShowing.bIsBuffered = TTX_BUF_PageListGetIdx(  pPageAddr, &(ttxBuf.pageList.pageShowing.idx));
    ttxBuf.pageList.pageShowing.pageA = pPageAddr->pageA;
    ttxBuf.pageList.pageShowing.subcodeA= pPageAddr->subcodeA;
    ttxBuf.pageList.elements[ttxBuf.pageList.pageShowing.idx].bstatus = TTX_BUFFER_DISPLAYED;
    EA=1;

    TTX_BUF_KeepPage( pPageAddr);
}


/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_DecodeBttPage (uint8_t src)
/*****************************************************************************\
 *  @brief   Overwrite the TTX_TOP_BTT_PAGE_ADDRESS with channel decoded *src.
 *  @param   [in]  *src is the starting address of X/0 of the received BTT page.
\*****************************************************************************/
{

    const uint8_t TTX_MEM* pStartOfBasicTopTable =
        ttxBuf.pageList.elements[src].addrMem + 40;    /* Skip X/0 */

    return( VTE_Decode( VTE_CD_MODE_H84M0, (uint8_t TTX_MEM*) TTX_TOP_BTT_PAGE_ADDRESS, pStartOfBasicTopTable, TTX_TOP_BTT_SIZE ) );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_FindPage(const TTX_STD_Page_AddrWithSubCode_t* pPageAddr,
                 uint8_t * buff_idx, bool_t lock) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   find a page in buffer and returns its memory address
 *  @ret     the page whose page address is *pPageAddr is in page buffer or not
\*****************************************************************************/
{
    uint8_t i;


    //TTX_Log(LOGGER_VERBOSE, "h5 %x %x\n",(uint32_t) pPageAddr->pageA, (uint32_t) pPageAddr->subcodeA);

    // check if it is already in buffer
    for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
    {
        EA = 0;  // due to bstatus may be modified by ISR
//		if ((ttxBuf.pageList.elements[i].pageA == pPageAddr->pageA) &&
//			(ttxBuf.pageList.elements[i].subcodeA == pPageAddr->subcodeA)) {  // already in buffer

        //RTD_Log(LOGGER_INFO, "ttx_buf.pageA (%x.%x), PageAddr.pageA (%x.%x) \n", (UINT32) ttxBuf.pageList.elements[i].pageA, (UINT32) ttxBuf.pageList.elements[i].subcodeA,
        //(UINT32) pPageAddr->pageA, (UINT32) pPageAddr->subcodeA);

        if ((ttxBuf.pageList.elements[i].pageA == pPageAddr->pageA))   // check main number only
        {
            if (pPageAddr->subcodeA !=0)   // if have subcode, need to check it
            {
                if (ttxBuf.pageList.elements[i].subcodeA != pPageAddr->subcodeA)
                {
                    continue;
                }
            }
            //RTD_Log(LOGGER_INFO, "TTX_BUF_FindPage : buf [%d] Status => %d \n", (UINT32) i, (UINT32) ttxBuf.pageList.elements[i].bstatus);
            if  ((!lock) || (ttxBuf.pageList.elements[i].bstatus == TTX_BUFFER_COLLECTED))
            {

                //*pSaveAddr = ttxBuf.pageList.elements[i].addrMem;
                *buff_idx = i;
                pPageAddr->subcodeA = ttxBuf.pageList.elements[i].subcodeA; // restore subcodeA to display page
//				TTX_Log(LOGGER_VERBOSE, "buf %d %d %d\n", (uint32_t)i , (uint32_t) ttxBuf.pageList.elements[i].bstatus, (uint32_t)lock);
                ttxBuf.pageList.elements[i].bstatus = TTX_BUFFER_ONDECODE;
                EA = 1;
                return TRUE;
            }
        }
        EA = 1;
    }

    return FALSE;
#if 0
    const

    TTX_BUF_PageListIdx_t idx;

    if ( TTX_BUF_PageListGetIdx(  pPageAddr, &idx, TRUE, NULL )  )
    {
//        TTX_ALLEGE( ttxBuf.pageList.elements[idx].addrMem != NULL );  /* ERROR: TTX_BUF_PageListGetIdx() got an empty space */

        /*
                // Print debug log
                {
                    const TTX_STD_Page_AddrWithSubCode_Hex_t pageAddrHex = \
                        TTX_STD_PageAddrWithSubCodeToHex( pPageAddr );
                    TTX_Log(LOGGER_VERBOSE, "[TTX]" "[Trace] Found P%X.%X in index 0x%X\n", (pageAddrHex>>16), (pageAddrHex & 0x0000FFFF), idx );
                    if( pageAddrHex>>16 == 0x100 )
                    {
                        int a = 3;
                    }
                }
        */
//        if( pSaveAddr != NULL ) // if caller wants to retrieve its page buffer memory address
//        {
        *pSaveAddr = ttxBuf.pageList.elements[idx].addrMem;
//        }
        return( TRUE ); // found
    }
    return( FALSE );
#endif

}

void TTX_BUF_SetBufStatus(uint8_t idx, uint8_t status)
{
    ttxBuf.pageList.elements[idx].bstatus = status;
}


void TTX_MEM* TTX_BUF_GetMemByIdx(uint8_t idx)
{
    return ttxBuf.pageList.elements[idx].addrMem;
}

/*---------------------------------------------------------------------------*/
uint8_t TTX_MEM*
TTX_BUF_GetDecodedBttPage           (void)
/*****************************************************************************\
 *  @brief   get the space for a new page.
 *           May delete the most far page address if buffer is full.
 *  @pre     *pPageAddr shouldn't already be placed in page buff.
 *  @param   [in]  *pPageAddr is the page address in page buffer.
 *  @param   [out] *pAddr     is the found buffer address
\*****************************************************************************/
{
    return( (uint8_t TTX_MEM*) TTX_TOP_BTT_PAGE_ADDRESS );
}


/*---------------------------------------------------------------------------*/
void
TTX_BUF_KeepPage (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   Mark *pPageAddr can be removed or not when TTX_BUF_InsPage().
 *           Mark *pPageAddr must be TTX_BUF_InsPage() successfully.
 *  @details Mimic vbiSys_insertToKeepMemQueue()
 *  @param   *pPageAddr  is the page you want
\*****************************************************************************/
{
    uint8_t idx;
    uint8_t curIdx;
    TTX_BUF_KeepQueElement_t tmp_elm;

    ttxBuf.keepQue.cnt = (ttxBuf.keepQue.cnt + 1)%TTX_MAX_PAGE_KEEP_NUM;
    curIdx = ttxBuf.keepQue.cnt;
    // Insert *pPageAddr into keep queue
    if ( !TTX_BUF_KeepQueFindPage(  pPageAddr, &idx )  )   // insert only if *pPageAddr is not in keep queue
    {
        // Insert at tail (element with larget index number)
        EA =0;
        if (ttxBuf.keepQue.elements[ curIdx].bindex!=0xFF)
        {
            ttxBuf.pageList.elements[ttxBuf.keepQue.elements[curIdx].bindex].keep = FALSE;
        }
        ttxBuf.keepQue.elements[ curIdx ].pageA = pPageAddr->pageA;
        ttxBuf.keepQue.elements[ curIdx ].subcodeA= pPageAddr->subcodeA;
        ttxBuf.keepQue.elements[ curIdx].bindex = 0xFF;
        EA =1;
    }
    else
    {
        if (idx!=curIdx)
        {
            // swap found idx with now one
            EA =0;
            tmp_elm = ttxBuf.keepQue.elements[idx];
            ttxBuf.keepQue.elements[idx] = ttxBuf.keepQue.elements[ curIdx ];
            ttxBuf.keepQue.elements[ curIdx ] = tmp_elm;
            EA =1;
        }

    }

}


/*---------------------------------------------------------------------------*/
bool_t TTX_BUF_DecodeNormalPage (uint8_t src)
/*****************************************************************************\
 *  @brief   Allocates a "cd mem" and fill it with channel-decoded data from src.
 *           May delete the most far page address if buffer is full.
 *  @pre     *src is the encoded packets of a whole normal page.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be loaded with new channel-decoded data.
 *  @see     TTX_BUF_GetDecodedPacket()
\*****************************************************************************/
{
    return( TTX_BUF_DecodePageHelper( TTX_BUF_DECODE_PAGE_NORMAL, src ) );
}

#if 0
/*---------------------------------------------------------------------------*/
bool_t TTX_BUF_DecodeX0 (uint8_t src)
/*****************************************************************************\
 *  @brief   channel decode X/0
 *  @pre     *src is the encoded packets of a whole normal page.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be loaded with new channel-decoded data.
 *  @see     TTX_BUF_GetDecodedPacket()
\*****************************************************************************/
{
    return( TTX_BUF_DecodePageHelper( TTX_BUF_DECODE_PAGE_X0, src ) );
}
#endif

/*---------------------------------------------------------------------------*/
bool_t TTX_BUF_DecodeX24 (uint8_t src)
/*****************************************************************************\
 *  @brief   channel decode X/24
 *  @pre     *src is the encoded packets of a whole normal page.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be loaded with new channel-decoded data.
 *  @see     TTX_BUF_GetDecodedPacket()
\*****************************************************************************/
{
    return( TTX_BUF_DecodePageHelper( TTX_BUF_DECODE_PAGE_X24, src ) );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_DecodeAitPage  (uint8_t src)
/*****************************************************************************\
 *  @brief   Allocates a "cd mem" and fill it with channel-decoded data from src.
 *           May delete the most far page address if buffer is full.
 *  @pre     *src is the encoded packets of a whole AIT page.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be loaded with new channel-decoded data.
 *  @see     TTX_BUF_GetDecodedPacket()
\*****************************************************************************/
{
    return( TTX_BUF_DecodePageHelper( TTX_BUF_DECODE_PAGE_AIT, src ) );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_DecodeMpePage  (uint8_t  src)
/*****************************************************************************\
 *  @brief   Allocates a "cd mem" and fill it with channel-decoded data from src.
 *           May delete the most far page address if buffer is full.
 *  @pre     *src is the encoded packets of a whole AIT page.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be loaded with new channel-decoded data.
 *  @see     TTX_BUF_GetDecodedPacket()
\*****************************************************************************/
{
    return( TTX_BUF_DecodePageHelper( TTX_BUF_DECODE_PAGE_MPE, src ) );
}



/*---------------------------------------------------------------------------*/
uint8_t TTX_MEM*
TTX_BUF_GetDecodedPacket            (const TTX_STD_Packet_Num_t pktNum)
/*****************************************************************************\
 *  @brief   After calling TTX_BUF_DecodeXxxPage() once, and call this function
 *             multiple times to obtain various contents of decoded packets with pktNum.
 *  @pre     *src is the encoded packets of a whole page.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be loaded with new channel-decoded data.
 *  @see     TTX_BUF_GetDecodedPacket()
\*****************************************************************************/
{
    uint8_t TTX_MEM* pPageBuf = (uint8_t TTX_MEM*)TTX_DEFAULT_CD_PAGE_ADDRESS;

    TTX_REQUIRE( TTX_DEFAULT_CD_PAGE_ADDRESS != 0 );
    TTX_REQUIRE( pPageBuf != NULL );

    if ( pPageBuf != NULL )
    {
        return( pPageBuf + pktNum * TTX_PACKET_SIZE );
    }

    TTX_ERROR();
    return( NULL );
}



#if 0
/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_FindNextDisplayableDifferentMainPage (void TTX_MEM**                  pSaveAddr,
        TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound)
/*****************************************************************************\
 *  @brief   find a page in buffer and returns its memory address
 *  @pre     TTX_BUF_SetShowingPage() is invoked before this function.
 *  @ret     the page whose page address is *pPageAddr is in page buffer or not
\*****************************************************************************/
{
    const

    if ( ttxBuf.pageList.pageShowing.bIsBuffered )  /* if ttxBuf.pageList.pageShowing.idx is valid */
    {
        const TTX_BUF_PageListIdx_t idxShowing = ttxBuf.pageList.pageShowing.idx;

        TTX_BUF_PageListIdx_t idxFound;

        /* Find the next displayable page index with respect to current showing page */
        if ( TTX_BUF_GetNextDispDiffMainPageIdx(  idxShowing, &idxFound ) )
        {
            const TTX_BUF_PageListElement_t* const pElement = \
                    &( ttxBuf.pageList.elements[idxFound] );

            TTX_ALLEGE( TTX_STD_isDisplayablePage( &(pElement->addrPage) ) );    /* ERROR: found page is not displayable. Error in TTX_BUF_GetNextDispDiffMainPageIdx()? */

            if ( pSaveAddr != NULL )
            {
                *pSaveAddr = pElement->addrMem;
            }

            if ( pPageAddrFound != NULL )
            {
                *pPageAddrFound = pElement->addrPage;
            }

            return( TRUE );
        }
    }
    else
    {
        //TTX_Log( LOGGER_WARN, "[TTX] Showing page is not buffered!\n" );   /* ERROR: Showing page is not buffered, but you want to change to next displayable page */
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_FindPrevDisplayableDifferentMainPage (void TTX_MEM**                  pSaveAddr,
        TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound)
/*****************************************************************************\
 *  @brief   find a page in buffer and returns its memory address
 *  @pre     TTX_BUF_SetShowingPage() is invoked before this function.
 *  @ret     the page whose page address is *pPageAddr is in page buffer or not
\*****************************************************************************/
{
    const

    if ( ttxBuf.pageList.pageShowing.bIsBuffered )  /* if ttxBuf.pageList.pageShowing.idx is valid */
    {
        const TTX_BUF_PageListIdx_t idxShowing = ttxBuf.pageList.pageShowing.idx;

        TTX_BUF_PageListIdx_t idxFound;

        if ( TTX_BUF_GetPrevDispDiffMainPageIdx(  idxShowing, &idxFound ) )
        {
            const TTX_BUF_PageListElement_t* pElement;

            /* Trying to move to the first sub-page of idxFound. */
            TTX_BUF_GetPrevMostFarSubPageIdx(  idxFound, &idxFound, &idxFound, NULL );
            pElement = &( ttxBuf.pageList.elements[idxFound] );

            TTX_ALLEGE( TTX_STD_isDisplayablePage( &(pElement->addrPage) ) );    /* ERROR: found page is not displayable. Error in TTX_BUF_GetNextDispDiffMainPageIdx()? */

            if ( pSaveAddr != NULL )
            {
                *pSaveAddr = pElement->addrMem;
            }

            if ( pPageAddrFound != NULL )
            {
                *pPageAddrFound = pElement->addrPage;
            }

            return( TRUE );
        }
    }
    else
    {
        //TTX_Log( LOGGER_WARN, "[TTX] Showing page is not buffered!\n" );   /* ERROR: Showing page is not buffered, but you want to change to next displayable page */
    }

    return( FALSE );
}


/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_FindNextSubPage  (void TTX_MEM**                        pSaveAddr,
                          TTX_STD_Page_AddrWithSubCode_t*       pPageAddrFound)
/*****************************************************************************\
 *  @brief   Erase content of a whole page.
 *  @param   *pPageAddr is the page address you want to remove.
 *  @pre     TTX_BUF_SetShowingPage() is invoked before this function.
\*****************************************************************************/
{
    const

    TTX_REQUIRE( ttxBuf.pageList.pageShowing.bIsBuffered );   /* ERROR: Showing page is not buffered, but you want to change to next displayable page */
    if ( ttxBuf.pageList.pageShowing.bIsBuffered )  /* if ttxBuf.pageList.pageShowing.idx is valid */
    {
        const TTX_BUF_PageListIdx_t idxShowing = ttxBuf.pageList.pageShowing.idx;

        TTX_BUF_PageListIdx_t idxFound;

        TTX_REQUIRE( ttxBuf.pageList.pageShowing.bIsBuffered );   /* ERROR: idxShowing is not valid */
        TTX_ALLEGE( ttxBuf.pageList.elements[idxShowing].addrMem != NULL );  /* ERROR: TTX_BUF_PageListGetIdx() got an empty space */

        if ( TTX_BUF_GetNextSubPageIdx(  idxShowing, NULL, &idxFound ) )
        {
            if ( pPageAddrFound != NULL )
            {
                *pPageAddrFound = ttxBuf.pageList.elements[idxFound].addrPage;
            }

            if ( pSaveAddr != NULL ) // if caller wants to retrieve its page buffer memory address
            {
                *pSaveAddr = ttxBuf.pageList.elements[idxFound].addrMem;
            }
            return( TRUE ); // found
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_FindPrevSubPage  (void TTX_MEM**                        pSaveAddr,
                          TTX_STD_Page_AddrWithSubCode_t*       pPageAddrFound)
/*****************************************************************************\
 *  @brief   Erase content of a whole page.
 *  @param   *pPageAddr is the page address you want to remove.
 *  @pre     TTX_BUF_SetShowingPage() is invoked before this function.
\*****************************************************************************/
{
    const

    TTX_REQUIRE( ttxBuf.pageList.pageShowing.bIsBuffered );   /* ERROR: Showing page is not buffered, but you want to change to next displayable page */
    if ( ttxBuf.pageList.pageShowing.bIsBuffered )  /* if ttxBuf.pageList.pageShowing.idx is valid */
    {
        const TTX_BUF_PageListIdx_t idxShowing = ttxBuf.pageList.pageShowing.idx;

        TTX_BUF_PageListIdx_t idxFound;

        TTX_REQUIRE( ttxBuf.pageList.pageShowing.bIsBuffered );   /* ERROR: idxShowing is not valid */
        TTX_ALLEGE( ttxBuf.pageList.elements[idxShowing].addrMem != NULL );  /* ERROR: TTX_BUF_PageListGetIdx() got an empty space */

        if ( TTX_BUF_GetPrevSubPageIdx(  idxShowing, NULL, &idxFound ) )
        {
            if ( pPageAddrFound != NULL )
            {
                *pPageAddrFound = ttxBuf.pageList.elements[idxFound].addrPage;
            }

            if ( pSaveAddr != NULL ) // if caller wants to retrieve its page buffer memory address
            {
                *pSaveAddr = ttxBuf.pageList.elements[idxFound].addrMem;
            }
            return( TRUE ); // found
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_GetSubPageNavCtrlInfo  (const TTX_STD_Page_AddrWithSubCode_t* pPageToFind,
                                TTX_DISP_SubPageNavCtrl_t*            pPageNavInfo)
/*****************************************************************************\
 *  @brief   Update *pPageAddr's sub-page info *pPageNavInfo by searching
 *             all content of page-buffer.
 *  @param   *pPageAddr is the page address you want to look up.
\*****************************************************************************/
{
    if ( ( pPageToFind != NULL )&&( pPageNavInfo != NULL ) )
    {
        const

        TTX_BUF_PageListIdx_t idxFound;

        if ( TTX_BUF_PageListGetIdx(  pPageToFind, &idxFound, TRUE, NULL ) )
        {
            pPageNavInfo->cnt          = 0;
            pPageNavInfo->cntTotal     = 0;
            pPageNavInfo->idxHighlight = 0;
            if ( TTX_STD_canHaveSubPage( pPageToFind ) )
            {
                TTX_STD_SubPage_Displayable_Num_t i, subpage_cmd, subpage;

                bool_t subpage_hit = FALSE;

                const TTX_BUF_PageListElement_t* pElm;

                TTX_BUF_PageListIdx_t idxFirstSubPage = idxFound;
                TTX_BUF_GetPrevMostFarSubPageIdx(  idxFound, NULL, &idxFirstSubPage, NULL );
                TTX_ENSURE( TTX_STD_PageAddrWithSubCodeAreEqual( &(ttxBuf.pageList.elements[idxFirstSubPage].addrPage), \
                            &(ttxBuf.pageList.elements[idxFound       ].addrPage), FALSE ) );   /* ERROR: first subpage (idxFirstSubPage) doesn't have the same main-page number with found page (idxFound). */


                TTX_STD_GetDisplayableSubPageNum( &subpage_cmd, &(pPageToFind->subCode) );

                pElm = &( ttxBuf.pageList.elements[idxFirstSubPage]   );

                do
                {
                    TTX_ALLEGE( pElm->addrMem != NULL );    /* ERROR: all linked-elements must be occupied */

                    if ( TTX_STD_GetDisplayableSubPageNum( &subpage, &(pElm->addrPage.subCode) ) )
                    {
                        // We have gathered all information of pSubPageList[]
                        if ( subpage_hit && ( pPageNavInfo->cnt >= TTX_DISP_SUBPAGE_MAX_NUM ) )
                        {
                        }
                        else
                        {
                            i = pPageNavInfo->cnt % TTX_DISP_SUBPAGE_MAX_NUM;

                            pPageNavInfo->list[ i ] = subpage;

                            if ( subpage == subpage_cmd )
                            {
                                subpage_hit = TRUE;
                                pPageNavInfo->idxHighlight = i;
                            }

                            pPageNavInfo->cnt++;
                        }
                    }
                    else
                    {
                        TTX_ERROR();    /* ERROR: Shouldn't run here */
                    }

                    pPageNavInfo->cntTotal++;

                    /* move toward next */
                    pElm  = &( ttxBuf.pageList.elements[ pElm->next ] );
                }
                while ( TTX_STD_PageAddrWithSubCodeAreEqual( pPageToFind, &(pElm->addrPage), FALSE ) );

                if ( pPageNavInfo->cnt > TTX_DISP_SUBPAGE_MAX_NUM )
                {
                    pPageNavInfo->idxStart = (pPageNavInfo->cnt)%TTX_DISP_SUBPAGE_MAX_NUM;
                }
                else
                {
                    pPageNavInfo->idxStart = 0;
                }

                TTX_ENSURE( pPageNavInfo->idxStart <  TTX_DISP_SUBPAGE_MAX_NUM );
                TTX_ENSURE( pPageNavInfo->cnt      <= TTX_STD_SUBPAGE_DISPLAYABLE_MAX_NUM );
                TTX_ENSURE( pPageNavInfo->cntTotal <= TTX_STD_SUBPAGE_DISPLAYABLE_MAX_NUM );
            }

            return( TRUE );

        }
        else
        {
            TTX_ERROR();    /* ERROR: Trying to get navigation info of non-buffered page */
        }
    }

    return( FALSE );
}
#endif
#if 0  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
void
TTX_BUF_DelPage    (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr)
/*****************************************************************************\
 *  @brief   Remove a page whose page address is *pPageAddr from page buffer.
 *  @param   *pPageAddr is the page address you want to remove.
\*****************************************************************************/
{


    TTX_BUF_PageListIdx_t idx;

    // Remove *pPageAddr from page list
    if ( TTX_BUF_PageListGetIdx(  pPageAddr, &idx, TRUE, NULL )  )
    {
        TTX_BUF_PageListDelPage(  idx );
    }

    // Remove *pPageAddr from keep queue
    TTX_BUF_KeepQuePopPage(  pPageAddr );
}

#endif  /* end of #if TTX_VERIFY */



#if TTX_VERIFY  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_Test (void)
/*****************************************************************************\
 *  @brief   get the space for a new page.
 *           May delete the most far page address if buffer is full.
 *  @pre     *pPageAddr shouldn't already be placed in page buff.
 *  @param   [in]  *pPageAddr is the page address in page buffer.
 *  @param   [out] *pAddr     is the found buffer address
\*****************************************************************************/
{
    bool_t bSwOk, bVteOk;

    TTX_Log(LOGGER_VERBOSE, "[TTX]" "[BUF]" "[Trace]" "Entering TTX_BUF_Test()... ");

    bSwOk  = TTX_BUF_TestSoftware();
    TTX_ENSURE( bSwOk == TRUE );

    bVteOk = VTE_Test();
    TTX_ENSURE( bVteOk == TRUE );

    TTX_Log(LOGGER_VERBOSE, "[done]\n");

    return( bSwOk && bVteOk );
}
#endif  /* end of #if TTX_VERIFY */


/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_KeepQueFindPage  (
    const TTX_STD_Page_AddrWithSubCode_t*  pPageAddr, uint8_t * page_index)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.records[] whose page address is the
 *              the same as *pPageAddr and the records[] is currently been used.
 *  @param   [in]  *pPageAddr is the page address in page buffer.
\*****************************************************************************/
{
    TTX_BUF_KeepQueIdx_t i;

    for ( i=0; i<TTX_MAX_PAGE_KEEP_NUM; i++ )
    {
        if (( ttxBuf.keepQue.elements[i].pageA == pPageAddr->pageA) &&
                ( ttxBuf.keepQue.elements[i].subcodeA== pPageAddr->subcodeA))      /* ttxBuf.keepQue.elements[i].addrPage == *pPageAddr */
        {
            *page_index = i;
            return( TRUE ); /* found. We stop iterating here because we assume each records has unique page-address */
        }
    }

    return( FALSE );
}

#if 0
/*---------------------------------------------------------------------------*/
static void
TTX_BUF_KeepQuePushPage          (
    const TTX_STD_Page_AddrWithSubCode_t* pPageToIns)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.records[] whose page address is the
 *              most far away from *pPageAddr.
 *  @param   [in]  *pPageAddr is the page address you want to insert.
 *  @param   [out] *pIndex is the found records index can be replaced.
 *  @return  [out] the record index can be replaced.
\*****************************************************************************/
{
    // Insert *pPageAddr into keep queue
    if ( ! TTX_BUF_KeepQueFindPage(  pPageToIns )  ) // insert only if *pPageAddr is not in keep queue
    {
        if ( TTX_BUF_KeepQueIsFull() )
        {
            TTX_BUF_KeepQuePopOldestPage(  );
        }

        TTX_REQUIRE( !TTX_BUF_KeepQueIsFull() );

        if ( pPageToIns != NULL )
        {
            TTX_REQUIRE( ttxBuf.keepQue.cnt < TTX_MAX_PAGE_KEEP_NUM );
            if ( ttxBuf.keepQue.cnt < TTX_MAX_PAGE_KEEP_NUM )
            {
                // Insert at tail (element with larget index number).
                ttxBuf.keepQue.elements[ ttxBuf.keepQue.cnt ].pageA = pPageToIns->pageA;
                ttxBuf.keepQue.elements[ ttxBuf.keepQue.cnt ].subcodeA= pPageToIns->subcodeA;
                ttxBuf.keepQue.cnt++;
            }
        }
    }
}


/*---------------------------------------------------------------------------*/
static void
TTX_BUF_KeepQuePopPage          (
    const TTX_STD_Page_AddrWithSubCode_t* pPageToDel)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.records[] whose page address is the
 *              most far away from *pPageAddr.
 *  @param   [in]  *pPageAddr is the page address you want to insert.
 *  @param   [out] *pIndex is the found records index can be replaced.
 *  @return  [out] the record index can be replaced.
\*****************************************************************************/
{
    TTX_BUF_KeepQueIdx_t i, idx;

    // Remove *pPageAddr from keep queue
    if ( TTX_BUF_KeepQueGetIdx(  pPageToDel, &idx )  )
    {
        ttxBuf.pageList.elements[ttxBuf.keepQue.elements[idx].bindex].keep = FALSE;
//		TTX_Log(LOGGER_VERBOSE, "remove %x %x ",(uint32_t) ttxBuf.keepQue.elements[idx].bindex, (uint32_t)idx);

        for ( i=idx; i<TTX_MAX_PAGE_KEEP_NUM-1; i++ )
        {
            ttxBuf.keepQue.elements[i] = ttxBuf.keepQue.elements[i+1];
        }
        ttxBuf.keepQue.cnt--;
    }


}

/*---------------------------------------------------------------------------*/
static void
TTX_BUF_KeepQuePopOldestPage    ()
/*****************************************************************************\
 *  @brief   Removes oldest page of keep queue.
 *  @param   [out] *pOldestPage is the oldest page in keepQue.
\*****************************************************************************/
{
    TTX_BUF_PageListIdx_t i;

    ttxBuf.pageList.elements[ttxBuf.keepQue.elements[0].bindex].keep = FALSE;

    // Remove *pPageAddr from keep queue
    for ( i=0; i<TTX_MAX_PAGE_KEEP_NUM-1; i++ )
    {
        ttxBuf.keepQue.elements[i] = ttxBuf.keepQue.elements[i+1];
    }
    ttxBuf.keepQue.cnt--;

}


/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_GetPrevDiffMainPageIdx   (const TTX_BUF_Handle
                                  const TTX_BUF_PageListIdx_t  idxStart,
                                  const TTX_BUF_PageListIdx_t* pIdxStop,
                                  TTX_BUF_PageListIdx_t*       pIdxFound)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.pageList.elements[] whose page address is the
 *              nearest but greater than *pPageAddr.
 *  @param   [in]  *pPageAddr           is the page address you want to find nearest  from.
 *  @param   [in]  *pIdxStop is the stopping page when encountered.
 *                   If you don't know the value of this, just specify NULL here, and
 *                   it's the same as setting *pIdxStop==idxStart.
 *  @return  [out] the record index found.
\*****************************************************************************/
{
    if ( hBuf != NULL )
    {
        TTX_REQUIRE( idxStart < TTX_MAX_PAGE_NUM );   /* ERROR: idxStart is invalid */
        if ( idxStart < TTX_MAX_PAGE_NUM )
        {
            TTX_STD_Page_AddrWithSubCode_t pageStart;
            TTX_BUF_PageListIdx_t idxStop;

            const TTX_BUF_PageListElement_t* pElmLarger  = &(ttxBuf.pageList.elements[idxStart]);
            const TTX_BUF_PageListElement_t* pElmSmaller = &(ttxBuf.pageList.elements[pElmLarger->prev]);

            TTX_ALLEGE( pElmSmaller->addrMem != NULL );    /* ERROR: all linked-elements must be occupied */
            TTX_ALLEGE( pElmLarger->addrMem  != NULL );    /* ERROR: all linked-elements must be occupied */

            pageStart = ttxBuf.pageList.elements[ idxStart ].addrPage;

            /* when pIdxStop == NULL, loop until all elements are visited */
            idxStop = ( pIdxStop == NULL )? idxStart : *pIdxStop;

            while ( pElmLarger->prev != idxStop )
            {
                if ( ! TTX_STD_PageAddrWithSubCodeAreEqual( &pageStart, &( pElmSmaller->addrPage ), FALSE ) )
                {
                    if ( pIdxFound != NULL )
                    {
                        *pIdxFound = pElmLarger->prev;
                    }
                    return( TRUE );  /* found */
                }

                /* move toward prev */
                pElmLarger  = pElmSmaller;
                pElmSmaller = &( ttxBuf.pageList.elements[ pElmLarger->prev ] );
            }
        }
    }

    return( FALSE );    /* not found */
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_GetNextDispDiffMainPageIdx   (const TTX_BUF_Handle
                                      const TTX_BUF_PageListIdx_t idxShowing,
                                      TTX_BUF_PageListIdx_t*      pIdxFound)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.pageList.elements[] whose page address
 *             1. main-page number is different from *pPageToFind.
 *             2. is on the right side of *pPageToFind.
 *             3. is displayable.
 *  @param   [in]  *pPageToFind is the page address you want to find nearest  from.
 *  @return  [out] the record index found.
\*****************************************************************************/
{
    const TTX_BUF_PageListIdx_t idxInit  = idxShowing;
    TTX_BUF_PageListIdx_t idxStart = idxInit,
                                     idxFound;

    const TTX_BUF_PageListElement_t* pElmFound;
#if 0  // temporary marked by ghyu, due to REENTRANT, should be fixed later
    while ( TTX_BUF_GetNextDiffMainPageIdx(  idxStart, &idxInit, &idxFound ) )
    {
        pElmFound = &( ttxBuf.pageList.elements[idxFound] );
        if ( TTX_STD_isDisplayablePage( &( pElmFound->addrPage ) ) )
        {
            if ( pIdxFound != NULL )
            {
                *pIdxFound = idxFound;
            }
            return( TRUE ); /* found */
        }
        else
        {
            idxStart = idxFound;    /* move toward next */
        }
    }
#endif
    return( FALSE );    /* not found */
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_GetPrevDispDiffMainPageIdx   (const TTX_BUF_Handle
                                      const TTX_BUF_PageListIdx_t idxShowing,
                                      TTX_BUF_PageListIdx_t*      pIdxFound)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.pageList.elements[] whose page address is the
 *              nearest but greater than *pPageAddr.
 *  @param   [in]  *pPageAddr           is the page address you want to find nearest  from.
 *  @return  [out] the record index found.
\*****************************************************************************/
{
    const TTX_BUF_PageListIdx_t idxInit  = idxShowing;
    TTX_BUF_PageListIdx_t idxStart = idxInit,
                                     idxFound;

    const TTX_BUF_PageListElement_t* pElmFound;

    while ( TTX_BUF_GetPrevDiffMainPageIdx(  idxStart, &idxInit, &idxFound ) )
    {
        pElmFound = &( ttxBuf.pageList.elements[idxFound] );
        if ( TTX_STD_isDisplayablePage( &( pElmFound->addrPage ) ) )
        {
            if ( pIdxFound != NULL )
            {
                *pIdxFound = idxFound;
            }
            return( TRUE ); /* found */
        }
        else
        {
            idxStart = idxFound;    /* move toward next */
        }
    }

    return( FALSE );    /* not found */
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_GetNextSubPageIdx  (const TTX_BUF_Handle
                            const TTX_BUF_PageListIdx_t  idxStart,
                            const TTX_BUF_PageListIdx_t* pIdxStop,
                            TTX_BUF_PageListIdx_t*       pIdxFound )
/*****************************************************************************\
 *  @brief   Get the last sub-page index of idxStart within the range from idxStart to *pIdxStop.
 *  @param   [in] *pIdxStop is NULL: if you want to search all pages of page-queue;
 *                          is not NULL: if encountering it, search will be stopped.
 *  @param   [out] *pNumOfSubPages will be the number of sub-pages (not-including idxStart).
 *                                 Therefore, the total number of sub-pages of idxStart found
 *                                 is (*pNumOfSubPages+1).
\*****************************************************************************/
{
    if ( hBuf != NULL )
    {
        TTX_REQUIRE( idxStart < TTX_MAX_PAGE_NUM );   /* ERROR: idxStart is invalid */
        if ( idxStart < TTX_MAX_PAGE_NUM )
        {
            TTX_STD_Page_AddrWithSubCode_t pageStart;
            TTX_BUF_PageListIdx_t idxStop;

            const TTX_BUF_PageListElement_t* pElmSmaller = &(ttxBuf.pageList.elements[idxStart]);
            const TTX_BUF_PageListElement_t* pElmLarger  = &(ttxBuf.pageList.elements[pElmSmaller->next]);

            TTX_ALLEGE( pElmSmaller->addrMem != NULL );    /* ERROR: all linked-elements must be occupied */
            TTX_ALLEGE( pElmLarger->addrMem  != NULL );    /* ERROR: all linked-elements must be occupied */

            pageStart = ttxBuf.pageList.elements[idxStart].addrPage;

            /* when pIdxStop == NULL, loop until all elements are visited */
            idxStop = ( pIdxStop == NULL )? idxStart : *pIdxStop;

            if ( pElmSmaller->next != idxStop )
            {
                if ( TTX_STD_PageAddrWithSubCodeAreEqual( &pageStart, &( pElmLarger->addrPage ), FALSE ) )
                {
                    if ( pIdxFound != NULL )
                    {
                        *pIdxFound = pElmSmaller->next;  /* return the index of *pElmLarger */
                    }
                    return( TRUE ); /* found */
                }
            }
        }
    }

    return( FALSE );    /* not found */
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_GetPrevSubPageIdx   (const TTX_BUF_Handle
                             const TTX_BUF_PageListIdx_t  idxStart,
                             const TTX_BUF_PageListIdx_t* pIdxStop,
                             TTX_BUF_PageListIdx_t*       pIdxFound)
/*****************************************************************************\
 *  @brief   Computes the minimum distance between *pAddr1 and *pAddr2.
 *  @note    Because P100 is next to P899, so the distance is 1, not 799.
\*****************************************************************************/
{
    if ( hBuf != NULL )
    {
        TTX_REQUIRE( idxStart < TTX_MAX_PAGE_NUM );   /* ERROR: idxStart is invalid */
        if ( idxStart < TTX_MAX_PAGE_NUM )
        {
            TTX_STD_Page_AddrWithSubCode_t pageStart;
            TTX_BUF_PageListIdx_t idxStop;

            const TTX_BUF_PageListElement_t* pElmLarger  = &(ttxBuf.pageList.elements[idxStart]);
            const TTX_BUF_PageListElement_t* pElmSmaller = &(ttxBuf.pageList.elements[pElmLarger->prev]);

            TTX_ALLEGE( pElmLarger->addrMem  != NULL );    /* ERROR: all linked-elements must be occupied */
            TTX_ALLEGE( pElmSmaller->addrMem != NULL );    /* ERROR: all linked-elements must be occupied */

            pageStart = ttxBuf.pageList.elements[idxStart].addrPage;

            /* when pIdxStop == NULL, loop until all elements are visited */
            idxStop = ( pIdxStop == NULL )? idxStart : *pIdxStop;

            if ( pElmLarger->prev != idxStop )
            {
                if ( TTX_STD_PageAddrWithSubCodeAreEqual( &pageStart, &( pElmSmaller->addrPage ), FALSE ) )
                {
                    if ( pIdxFound != NULL )
                    {
                        *pIdxFound = pElmLarger->prev;  /* return the index of *pElmSmaller */
                    }
                    return( TRUE ); /* found */
                }
            }
        }
    }

    return( FALSE );    /* not found */
}

#endif

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_DecodePageHelper  (const TTX_BUF_DecodePageType_t pageType,  uint8_t idx )
/*****************************************************************************\
 *  @brief   Allocates a "cd mem" and fill it with channel-decoded data from src.
 *           May delete the most far page address if buffer is full.
 *  @pre     *src is the encoded packets of a whole normal page.
 *  @post    TTX_DEFAULT_CD_PAGE_ADDRESS will be loaded with new channel-decoded data.
 *  @see     TTX_BUF_GetDecodedPacket()
\*****************************************************************************/
{
    uint8_t TTX_MEM* pPageBuf = (uint8_t TTX_MEM*)TTX_DEFAULT_CD_PAGE_ADDRESS;

    const uint8_t TTX_MEM* src = ttxBuf.pageList.elements[idx].addrMem;

    TTX_REQUIRE( TTX_DEFAULT_CD_PAGE_ADDRESS != 0 );
    TTX_REQUIRE( pPageBuf != NULL );

    if ( pPageBuf != NULL )
    {
        // reset content of "cd mem" to zero. It will be filled with working data.
        memset( pPageBuf, 0, TTX_DEFAULT_CD_PAGE_TOTAL_SIZE );

        if ( src != NULL )
        {
            switch ( pageType )
            {
            case TTX_BUF_DECODE_PAGE_NORMAL:
            {
                return( TTX_BUF_DecodeNormalPageHelper( pPageBuf, src ) );
            }
            case TTX_BUF_DECODE_PAGE_AIT:
            {
                return( TTX_BUF_DecodeAitPageHelper( pPageBuf, src ) );
            }
            case TTX_BUF_DECODE_PAGE_MPE:
            {
                return( TTX_BUF_DecodeMpePageHelper( pPageBuf, src ) );
            }
            case TTX_BUF_DECODE_PAGE_X24:
            {
                return( TTX_BUF_DecodeX24Helper( pPageBuf, src ) );
            }
            case TTX_BUF_DECODE_PAGE_X0:
            {
                return( TTX_BUF_DecodeX0Helper( pPageBuf, src ) );
            }
            default:
            {
                TTX_ERROR();    /* ERROR: Unsupported pageType */
                break;
            }
            }
        }
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_DecodeNormalPageHelper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src)
/*****************************************************************************\
 *  @brief   Descramble normal page.
 *  @param   dest : destination address
 *  @param   src  : source address
\*****************************************************************************/
{
    const uint8_t startRow = 0;
    const uint8_t endRow = 23;

    uint8_t s = startRow;

    VTE_Reset();

    //TTX_REQUIRE( startRow <= endRow );

    if ( s == 0 )
    {
        // X/0 has already been descrambled by PPR. No need to descramble here.
        memcpy( dest, src, TTX_PACKET_SIZE );
        s = 1;  /* for X/1~X/25, we shall skip X/0 */
        if ( dest[6]&0x1 )   // C10: Inhibit Display
        {
            return( TRUE );
        }
    }

    if ( endRow > 0 )
    {
        // X/1 ~ X/25
        VTE_Decode( VTE_CD_MODE_ODDPAR, (dest + s*TTX_PACKET_SIZE), (src + s*TTX_PACKET_SIZE), (endRow-s+1) * TTX_PACKET_SIZE );
    }

    TTX_BUF_DecodeX26Helper( dest, src );

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_DecodeAitPageHelper  (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src)
/*****************************************************************************\
 *  @brief   Descramble normal page.
 *  @param   dest : destination address
 *  @param   src  : source address
\*****************************************************************************/
{
    // X/0 has already been descrambled by PPR. No need to descramble here.
    memcpy( dest, src, TTX_PACKET_SIZE );

    VTE_Decode( VTE_CD_MODE_AIT, (dest + TTX_PACKET_SIZE), (src + TTX_PACKET_SIZE), 22 * TTX_PACKET_SIZE );

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_DecodeMpePageHelper  (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src)
/*****************************************************************************\
 *  @brief   Descramble normal page.
 *  @param   dest : destination address
 *  @param   src  : source address
\*****************************************************************************/
{
    // X/0 has already been descrambled by PPR. No need to descramble here.
    memcpy( dest, src, TTX_PACKET_SIZE );

    VTE_Decode( VTE_CD_MODE_H84M0, (dest + TTX_PACKET_SIZE), (src + TTX_PACKET_SIZE), 22 * TTX_PACKET_SIZE );

    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_DecodeX24Helper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src)
/*****************************************************************************\
 *  @brief   Descramble X/24.
 *  @param   dest : destination address
 *  @param   src  : source address
\*****************************************************************************/
{
    VTE_Decode( VTE_CD_MODE_ODDPAR, (dest + 24*TTX_PACKET_SIZE), (src + 24*TTX_PACKET_SIZE), TTX_PACKET_SIZE );
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_DecodeX0Helper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src)
/*****************************************************************************\
 *  @brief   Descramble X/0.
 *  @param   dest : destination address
 *  @param   src  : source address
\*****************************************************************************/
{
    // X/0 has already been descrambled by PPR. No need to descramble here.
    memcpy( dest, src, TTX_PACKET_SIZE );
    return( TRUE );
}

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_DecodeX26Helper (uint8_t TTX_MEM* dest, const uint8_t TTX_MEM* src)
/*****************************************************************************\
 *  @brief   Descramble X/26.
 *  @param   dest : destination address
 *  @param   src  : source address
\*****************************************************************************/
{
    uint8_t s = 0, i = 0;
    uint32_t addr = 0;

    uint8_t x26_row = 0xFF, x26_function = TTX_X26Function_UNKNOWN,
                                           x26_address = 0xFF, x26_mode = 0xFF, x26_data = 0xFF;

    bool_t skip_level_25 = FALSE;

    // X/26/0 ~ X/26/14
    ttx_x26_info.counter = 0;
    for ( s=0, addr=26*TTX_PACKET_SIZE ; s<=14 ; ++s, addr+=TTX_PACKET_SIZE )
    {
        if ( VTE_Decode( VTE_CD_MODE_H2418, (dest + addr), (src + addr), TTX_PACKET_SIZE ) )
        {
            // decode X/26
            for ( i=1 ; i<TTX_PACKET_SIZE ; i+=3 )
            {
                x26_address = *(dest+addr+i);
                x26_mode = *(dest+addr+i+1);
                x26_data = *(dest+addr+i+2);
                x26_function = TTX_BUF_GetX26Function(x26_address, x26_mode);

                if ( skip_level_25 )
                {
                    if ( x26_function == TTX_X26Function_TERMINATION_MARKER )
                    {
                        skip_level_25 = FALSE;
                    }
                    continue;
                }

                switch ( x26_function )
                {
                    // ----- row address group -----
                    //   Level 1.5
                case TTX_X26Function_SET_ACTIVE_POSITION:
                    x26_row = x26_address - 40;
                    //RTD_Log(LOGGER_INFO,"TTX_X26Function_SET_ACTIVE_POSITION\n");
                    break;

                case TTX_X26Function_ADDRESS_DISPLAY_ROW_0:
                    x26_row = 0;
                    //RTD_Log(LOGGER_INFO,"TTX_X26Function_ADDRESS_DISPLAY_ROW_0\n");
                    break;

                    //   Level 2.5
                case TTX_X26Function_ACTIVE_OBJECT_INVOCATION:
                case TTX_X26Function_ADAPTIVE_OBJECT_INVOCATION:
                case TTX_X26Function_PASSIVE_OBJECT_INVOCATION:
                case TTX_X26Function_ACTIVE_OBJECT_DEFINITION:
                case TTX_X26Function_ADAPTIVE_OBJECT_DEFINITION:
                case TTX_X26Function_PASSIVE_OBJECT_DEFINITION:
                    skip_level_25 = TRUE;
                    break;

                    // ----- column address group -----
                case TTX_X26Function_G0_CHAR_WITHOUT_DM:

                    if ( x26_row == 0xFF ) // Is row ready ?
                        break;

                    //RTD_Log(LOGGER_INFO,"TTX_X26Function_G0_CHAR_WITHOUT_DM\n");

                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].row = x26_row;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].col = x26_address;

                    if ( x26_data == 0x2a ) // why?
                        ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].c_index = 0x40;
                    else
                        ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].c_index = x26_data;

                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].dm_index = x26_mode & 0x1F;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].function = TTX_X26Function_G0_CHAR_WITHOUT_DM;
                    ttx_x26_info.counter++;
                    break;

                case TTX_X26Function_G0_CHAR_WITH_DM:

                    if ( x26_row == 0xFF ) // Is row ready ?
                        break;

                    //RTD_Log(LOGGER_INFO,"TTX_X26Function_G0_CHAR_WITH_DM\n");

                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].row = x26_row;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].col = x26_address;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].c_index = x26_data;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].dm_index = x26_mode & 0x1F;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].function = TTX_X26Function_G0_CHAR_WITH_DM;
                    ttx_x26_info.counter++;
                    break;

                case TTX_X26Function_CHAR_FROM_G2:

                    if ( x26_row == 0xFF ) // Is row ready ?
                        break;

                    //RTD_Log(LOGGER_INFO,"TTX_X26Function_CHAR_FROM_G2\n");

                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].row = x26_row;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].col = x26_address;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].c_index = x26_data;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].dm_index = 0x00;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].function = TTX_X26Function_CHAR_FROM_G2;
                    ttx_x26_info.counter++;
                    break;

                case TTX_X26Function_SMOOTHED_MOSAIC_CHAR_FROM_G3:

                    if ( x26_row == 0xFF ) // Is row ready ?
                        break;

                    //RTD_Log(LOGGER_INFO,"TTX_X26Function_SMOOTHED_MOSAIC_CHAR_FROM_G3\n");

                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].row = x26_row;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].col = x26_address;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].c_index = x26_data;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].dm_index = 0x00;
                    ttx_x26_info.x26_decoded_data[ttx_x26_info.counter].function = TTX_X26Function_SMOOTHED_MOSAIC_CHAR_FROM_G3;
                    ttx_x26_info.counter++;
                    break;

                default:
                    break;
                }
            }
        }
    }

    return( TRUE );
}

static TTX_X26Function_t
TTX_BUF_GetX26Function(uint8_t x26_address, uint8_t x26_mode)
{
    if ( x26_address <=39 )   // 0 ~ 39, column address group
    {
        switch (x26_mode)
        {
            // Level 1.5
        case 0x02:
            return TTX_X26Function_SMOOTHED_MOSAIC_CHAR_FROM_G3;
        case 0x0F:
            return TTX_X26Function_CHAR_FROM_G2;
        case 0x10:
            return TTX_X26Function_G0_CHAR_WITHOUT_DM;
            // PDC
        case 0x06:
            return TTX_X26Function_PDC;
            // Level 2.5
        case 0x00:
            return TTX_X26Function_FOREGROUND_COLOUR;
        case 0x01:
            return TTX_X26Function_BLOCK_MOSAIC_CHARACTER_FROM_THE_G1_SET;
        case 0x03:
            return TTX_X26Function_BACKGROUND_COLOUR;
        case 0x07:
            return TTX_X26Function_ADDITIONAL_FLASH_FUNCTIONS;
        case 0x08:
            return TTX_X26Function_MODIFIED_G0_AND_G2_CHARACTER_SET_DESIGN;
        case 0x09:
            return TTX_X26Function_CHARACTER_FROM_THE_G0_SET;
        case 0x0B:
            return TTX_X26Function_LINE_DRAWING_OR_SMOOTHED_MOSAIC_CHARACTER_FROM_THE_G3_SET;
        case 0x0C:
            return TTX_X26Function_DISPLAY_ATTRIBUTES;
        case 0x0D:
            return TTX_X26Function_DRCS_CHARACTER_INVOCATION;
            // Level 3.5
        case 0x0E:
            return TTX_X26Function_FONT_STYLE;
            // Others
        default:
            // Level 1.5
            if ( (x26_mode >= 0x11) && (x26_mode <= 0x1F) )
                return TTX_X26Function_G0_CHAR_WITH_DM;
            // Reserved (Unknown)
            break;
        }
    }
    else if ( x26_address <= 63 )   // 40 ~ 63, row address group
    {
        switch (x26_mode)
        {
            // Level 1.5
        case 0x04:
            return TTX_X26Function_SET_ACTIVE_POSITION;
        case 0x07:
            return TTX_X26Function_ADDRESS_DISPLAY_ROW_0;
            // Level 2.5
        case 0x00:
            return TTX_X26Function_FULL_SCREEN_COLOUR;
        case 0x01:
            return TTX_X26Function_FULL_ROW_COLOUR;
        case 0x10:
            return TTX_X26Function_ORIGIN_MODIFIER;
        case 0x11:
            return TTX_X26Function_ACTIVE_OBJECT_INVOCATION;
        case 0x12:
            return TTX_X26Function_ADAPTIVE_OBJECT_INVOCATION;
        case 0x13:
            return TTX_X26Function_PASSIVE_OBJECT_INVOCATION;
        case 0x15:
            return TTX_X26Function_ACTIVE_OBJECT_DEFINITION;
        case 0x16:
            return TTX_X26Function_ADAPTIVE_OBJECT_DEFINITION;
        case 0x17:
            return TTX_X26Function_PASSIVE_OBJECT_DEFINITION;
        case 0x18:
            return TTX_X26Function_DRCS_MODE;
        case 0x1F:
            return TTX_X26Function_TERMINATION_MARKER;
            // Others
        default:
            // PDC
            if ( (x26_mode >= 0x08) && (x26_mode <= 0x0D) )
                return TTX_X26Function_PDC;
            // Reserved (Unknown)
            break;
        }
    }
    return TTX_X26Function_UNKNOWN;
}

#if 0  /* if enabled verficiation of TTX related IPs */

/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_TestSoftware       (void)
/*****************************************************************************\
 *  @brief   Test this software module.
 *  @post    page buffer will be reset.
 *  @ret     test status
 *  @retval  TRUE  if test passed
 *  @retval  FALSE if test failed
\*****************************************************************************/
{
#if( TTX_MAX_PAGE_NUM == 10 ) || ( TTX_MAX_PAGE_NUM ==  5 )   // These test cases requires TTX_MAX_PAGE_NUM==10 or 5

    size_t i;
    bool_t bOk;
#endif

    TTX_BUF_Open();


#if( TTX_MAX_PAGE_NUM == 10 )   // These test cases requires TTX_MAX_PAGE_NUM==10
    // Test TTX_BUF_InsPage()
    {
        TTX_STD_Page_AddrWithSubCode_t addr, removedPage;
        void TTX_MEM* pageBuf[TTX_MAX_PAGE_NUM];
        uint8_t bRemoveOccurs;

        // Case-0
        {
            TTX_BUF_Reset();

            addr.magNum        = 2;
            addr.pageNum.tens  = 9;
            addr.pageNum.units = 3;
            addr.subCode.s4    = 0x1;   // MSB: 0x00~0x03
            addr.subCode.s3    = 0x2;   //      0x00~0x0F
            addr.subCode.s2    = 0x3;   //      0x00~0x07
            addr.subCode.s1    = 0x4;   // LSB: 0x00~0x0F

            bOk = TTX_BUF_FindPage( &addr, &pageBuf[0] );
            TTX_ENSURE( bOk == FALSE );

            bOk = TTX_BUF_InsPage( &addr, &pageBuf[0], &bRemoveOccurs, &removedPage );
            TTX_ENSURE( bOk == TRUE );
            TTX_ENSURE( pageBuf[0] == (void TTX_MEM*)TTX_DEFAULT_PAGEx_ADDRESS(0) );
            TTX_ENSURE( bRemoveOccurs == FALSE );

            bOk = TTX_BUF_FindPage( &addr, &pageBuf[0] );
            TTX_ENSURE( bOk == TRUE );
            TTX_ENSURE( pageBuf[0] == (void TTX_MEM*)TTX_DEFAULT_PAGEx_ADDRESS(0) );
            TTX_ENSURE( bRemoveOccurs == FALSE );

            // page with the same page address should be located in the same page buffer
            bOk = TTX_BUF_InsPage( &addr, &pageBuf[0], &bRemoveOccurs, &removedPage );
            TTX_ENSURE( bOk == TRUE );
            TTX_ENSURE( pageBuf[0] == (void TTX_MEM*)TTX_DEFAULT_PAGEx_ADDRESS(0) );
            TTX_ENSURE( bRemoveOccurs == FALSE );

            TTX_BUF_DelPage( &addr );

            TTX_ENSURE( TTX_BUF_isEmpty() == TRUE  );
            TTX_ENSURE( TTX_BUF_isFull()  == FALSE );
        }

        // Case-1
        {
            TTX_BUF_Reset();

            addr.magNum        = 1;
            addr.pageNum.tens  = 2;
            addr.pageNum.units = 4;
            addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
            addr.subCode.s3    = 0x0;   //      0x00~0x0F
            addr.subCode.s2    = 0x7;   //      0x00~0x07
            addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F


            // Insert different unique pages to full
            for ( i=0; i< TTX_MAX_PAGE_NUM; i++ )
            {
                addr.pageNum.units ++;

                if ( i == 0 )
                {
                    // Because all buffer management rules are based on current displaying page,
                    //   we shall call TTX_BUF_SetShowingPage() before any other functions.
                    TTX_BUF_SetPageToShow( &addr );
                    TTX_BUF_SetShowingPage( &addr );
                }

                bOk = TTX_BUF_FindPage( &addr, NULL );
                TTX_ENSURE( bOk == FALSE );

                bOk = TTX_BUF_InsPage( &addr, &pageBuf[i], &bRemoveOccurs, &removedPage );
                TTX_ENSURE( bOk == TRUE );
                TTX_ENSURE( pageBuf[i] == (void TTX_MEM*)TTX_DEFAULT_PAGEx_ADDRESS(i) );
                TTX_ENSURE( bRemoveOccurs == FALSE );
            }

            TTX_ENSURE( TTX_BUF_isFull()  == TRUE );

            // Page buffer is now P125.70, P126.70, P127.70, P128.70, P129.70,
            //                    P12A.70, P12B.70, P12C.70, P12D.70, P12E.70.

            TTX_ENSURE( TTX_BUF_GetNumOfDiffMainPages( hBuf ) == TTX_MAX_PAGE_NUM );

            // Try to insert a most far page: P129.71
            addr.magNum        = 1;
            addr.pageNum.tens  = 2;
            addr.pageNum.units = 9;
            addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
            addr.subCode.s3    = 0x0;   //      0x00~0x0F
            addr.subCode.s2    = 0x7;   //      0x00~0x07
            addr.subCode.s1    = 0x1;   // LSB: 0x00~0x0F
            bOk = TTX_BUF_InsPage( &addr, &pageBuf[TTX_MAX_PAGE_NUM-1], &bRemoveOccurs, &removedPage );
            TTX_ENSURE( bOk == FALSE );

            TTX_ENSURE( TTX_BUF_isFull()  == TRUE );

            // Reinitialize to first page addr of page queue: P125.70
            addr.magNum        = 1;
            addr.pageNum.tens  = 2;
            addr.pageNum.units = 5;
            addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
            addr.subCode.s3    = 0x0;   //      0x00~0x0F
            addr.subCode.s2    = 0x7;   //      0x00~0x07
            addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F
            bOk = TTX_BUF_FindPage( &addr, &pageBuf[0] );
            TTX_ENSURE( bOk == TRUE );
            TTX_ENSURE( pageBuf[0] == (void TTX_MEM*)TTX_DEFAULT_PAGEx_ADDRESS(0) );

            // page with the same page address should be located in the same page buffer
            bOk = TTX_BUF_InsPage( &addr, &pageBuf[0], &bRemoveOccurs, &removedPage );
            TTX_ENSURE( bOk == TRUE );
            TTX_ENSURE( pageBuf[0] == (void TTX_MEM*)TTX_DEFAULT_PAGEx_ADDRESS(0) );
            TTX_ENSURE( bRemoveOccurs == FALSE );

            TTX_ENSURE( TTX_BUF_isFull()  == TRUE );

            {
                bOk = TTX_BUF_FindPage( &addr, NULL );
                TTX_ENSURE( bOk == TRUE ); // ERROR: Above TTX_BUF_InsPage() fail.

                TTX_BUF_DelPage( &addr );

                bOk = TTX_BUF_FindPage( &addr, NULL );
                TTX_ENSURE( bOk == FALSE ); // ERROR: Above TTX_BUF_DelPage() fail.

                TTX_ENSURE( TTX_BUF_GetFreeSpace() == 1 );
                TTX_ENSURE( TTX_BUF_isEmpty() == FALSE  );
                TTX_ENSURE( TTX_BUF_isFull()  == FALSE );
            }

            {
                addr.pageNum.units = 6;

                bOk = TTX_BUF_FindPage( &addr, NULL );
                TTX_ENSURE( bOk == TRUE ); // ERROR: Above TTX_BUF_InsPage() fail.

                TTX_BUF_DelPage( &addr );

                bOk = TTX_BUF_FindPage( &addr, NULL );
                TTX_ENSURE( bOk == FALSE ); // ERROR: Above TTX_BUF_DelPage() fail.

                TTX_ENSURE( TTX_BUF_GetFreeSpace() == 2 );
                TTX_ENSURE( TTX_BUF_isEmpty() == FALSE  );
                TTX_ENSURE( TTX_BUF_isFull()  == FALSE );
            }
        }


        // Case-2: TTX_BUF_KeepPage().
        {
            TTX_BUF_Reset();

            addr.magNum        = 1;
            addr.pageNum.tens  = 2;
            addr.pageNum.units = 4;
            addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
            addr.subCode.s3    = 0x0;   //      0x00~0x0F
            addr.subCode.s2    = 0x7;   //      0x00~0x07
            addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

            // Insert different unique pages to full
            for ( i=0; i< TTX_MAX_PAGE_NUM; i++ )
            {
                addr.pageNum.units ++;

                if ( i == 0 )
                {
                    // Because all buffer management rules are based on current displaying page,
                    //   we shall call TTX_BUF_SetShowingPage() before any other functions.
                    TTX_BUF_SetPageToShow( &addr );
                    TTX_BUF_SetShowingPage( &addr );
                }

                bOk = TTX_BUF_FindPage( &addr, NULL );
                TTX_ENSURE( bOk == FALSE );

                bOk = TTX_BUF_InsPage( &addr, &pageBuf[i], &bRemoveOccurs, &removedPage );
                TTX_ENSURE( bOk == TRUE );
                TTX_ENSURE( pageBuf[i] == (void TTX_MEM*)TTX_DEFAULT_PAGEx_ADDRESS(i) );
                TTX_ENSURE( bRemoveOccurs == FALSE );
            }

            TTX_ENSURE( TTX_BUF_isFull()  == TRUE );

            // Page buffer is now P125.70, P126.70, P127.70, P128.70, P129.70,
            //                    P12A.70, P12B.70, P12C.70, P12D.70, P12E.70.

            {
                // Initialize a page P125.71.
                addr.magNum        = 1;
                addr.pageNum.tens  = 2;
                addr.pageNum.units = 5;
                addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                addr.subCode.s3    = 0x0;   //      0x00~0x0F
                addr.subCode.s2    = 0x7;   //      0x00~0x07
                addr.subCode.s1    = 0x1;   // LSB: 0x00~0x0F

                // page insertion will replace P12A.70
                bOk = TTX_BUF_InsPage( &addr, &pageBuf[0], &bRemoveOccurs, &removedPage );

                TTX_ENSURE( bOk == TRUE );
                TTX_ENSURE( bRemoveOccurs == TRUE );
                TTX_ENSURE( removedPage.magNum        == 0x01 );
                TTX_ENSURE( removedPage.pageNum.tens  == 0x02 );
                TTX_ENSURE( removedPage.pageNum.units == 0x0A );
                TTX_ENSURE( removedPage.subCode.s4    == 0x00 );
                TTX_ENSURE( removedPage.subCode.s3    == 0x00 );
                TTX_ENSURE( removedPage.subCode.s2    == 0x07 );
                TTX_ENSURE( removedPage.subCode.s1    == 0x00 );
            }

            // Page buffer is now P125.70, P126.70, P127.70, P128.70, P129.70,
            //                    P125.71, P12B.70, P12C.70, P12D.70, P12E.70.

            TTX_ENSURE( TTX_BUF_GetNumOfDiffMainPages( hBuf ) == TTX_MAX_PAGE_NUM - 1 );

            // test TTX_BUF_GetNextMostFarSubPageIdx()
            {
                TTX_BUF_PageListIdx_t idxFound, idxStop, numOfSubPages;

                // Case-1
                {
                    bOk = TTX_BUF_GetNextMostFarSubPageIdx(  0, NULL, &idxFound, &numOfSubPages );
                    TTX_ENSURE( bOk );
                    TTX_ENSURE( idxFound      == 5 );    /* ERROR: Cannot find P125.71 */
                    TTX_ENSURE( numOfSubPages == 1 );    /* ERROR: Cannot find P125.71 */
                }

                // Case-2
                {
                    idxStop = 5;
                    bOk = TTX_BUF_GetNextMostFarSubPageIdx(  0, &idxStop, &idxFound, NULL );
                    TTX_ENSURE( ! bOk );
                }

                // Case-3
                {
                    bOk = TTX_BUF_GetNextMostFarSubPageIdx(  5, NULL, &idxFound, NULL );
                    TTX_ENSURE( ! bOk );
                }

                // Case-4
                {
                    for ( i = 1; i<TTX_MAX_PAGE_NUM; i++ )
                    {
                        bOk = TTX_BUF_GetNextMostFarSubPageIdx(  i, NULL, &idxFound, NULL );
                        TTX_ENSURE( ! bOk );
                    }
                }
            }

            // test TTX_BUF_GetPrevMostFarSubPageIdx()
            {
                TTX_BUF_PageListIdx_t idxFound, idxStop, numOfSubPages;

                // Case-1
                {
                    bOk = TTX_BUF_GetPrevMostFarSubPageIdx(  5, NULL, &idxFound, &numOfSubPages );
                    TTX_ENSURE( bOk );
                    TTX_ENSURE( idxFound      == 0 );    /* ERROR: Cannot find P125.2F70 */
                    TTX_ENSURE( numOfSubPages == 1 );    /* ERROR: Cannot find P125.2F70 */
                }

                // Case-2
                {
                    idxStop = 0;
                    bOk = TTX_BUF_GetPrevMostFarSubPageIdx(  5, &idxStop, &idxFound, NULL );
                    TTX_ENSURE( ! bOk );
                }

                // Case-3
                {
                    bOk = TTX_BUF_GetPrevMostFarSubPageIdx(  0, NULL, &idxFound, NULL );
                    TTX_ENSURE( ! bOk );
                }

                // Case-4
                {
                    for ( i = 0; i<TTX_MAX_PAGE_NUM; i++ )
                    {
                        if ( i != 5 )   /* skip P125.3F70 */
                        {
                            bOk = TTX_BUF_GetPrevMostFarSubPageIdx(  i, NULL, &idxFound, NULL );
                            TTX_ENSURE( ! bOk );
                        }
                    }
                }

            }

#if 0   // We temporarily don't support keep queue
            {
                // Initialize a page P12D.2F70.
                addr.magNum        = 1;
                addr.pageNum.tens  = 2;
                addr.pageNum.units = 0xD;
                addr.subCode.s4    = 0x2;   // MSB: 0x00~0x03
                addr.subCode.s3    = 0xF;   //      0x00~0x0F
                addr.subCode.s2    = 0x7;   //      0x00~0x07
                addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

                TTX_BUF_KeepPage( &addr, TRUE );    // Keep P12D.2F70

                // Initialize a page P122.0000.
                addr.magNum        = 1;
                addr.pageNum.tens  = 2;
                addr.pageNum.units = 2;
                addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                addr.subCode.s3    = 0x0;   //      0x00~0x0F
                addr.subCode.s2    = 0x0;   //      0x00~0x07
                addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

                // page insertion will replace P12C.2F70
                bOk = TTX_BUF_InsPage( &addr, &pageBuf[0], &bRemoveOccurs, &removedPage );

                TTX_ENSURE( bOk == TRUE );
                TTX_ENSURE( bRemoveOccurs == TRUE );
                TTX_ENSURE( removedPage.magNum        == 0x01 );
                TTX_ENSURE( removedPage.pageNum.tens  == 0x02 );
                TTX_ENSURE( removedPage.pageNum.units == 0x0C );
                TTX_ENSURE( removedPage.subCode.s4    == 0x02 );
                TTX_ENSURE( removedPage.subCode.s3    == 0x0F );
                TTX_ENSURE( removedPage.subCode.s2    == 0x07 );
                TTX_ENSURE( removedPage.subCode.s1    == 0x00 );
            }

            // Page buffer is now P125.2F70, P126.2F70, P127.2F70, P128.2F70, P129.2F70,
            //                    P12A.2F70, P12B.2F70, P122.0000, P12D.2F70, P125.3F70.
            // Keep Queue is now  P12D.2F70


            {
                // Initialize a page P12D.2F70.
                addr.magNum        = 1;
                addr.pageNum.tens  = 2;
                addr.pageNum.units = 0xD;
                addr.subCode.s4    = 0x2;   // MSB: 0x00~0x03
                addr.subCode.s3    = 0xF;   //      0x00~0x0F
                addr.subCode.s2    = 0x7;   //      0x00~0x07
                addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

                TTX_BUF_KeepPage( &addr, FALSE );    // Don't keep P12D.2F70

                // Initialize a page P123.0000.
                addr.magNum        = 1;
                addr.pageNum.tens  = 2;
                addr.pageNum.units = 3;
                addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                addr.subCode.s3    = 0x0;   //      0x00~0x0F
                addr.subCode.s2    = 0x0;   //      0x00~0x07
                addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

                // page insertion will replace P12D.2F70
                bOk = TTX_BUF_InsPage( &addr, &pageBuf[0], &bRemoveOccurs, &removedPage );

                TTX_ENSURE( bOk == TRUE );
                TTX_ENSURE( bRemoveOccurs == TRUE );
                TTX_ENSURE( removedPage.magNum        == 0x01 );
                TTX_ENSURE( removedPage.pageNum.tens  == 0x02 );
                TTX_ENSURE( removedPage.pageNum.units == 0x0D );
                TTX_ENSURE( removedPage.subCode.s4    == 0x02 );
                TTX_ENSURE( removedPage.subCode.s3    == 0x0F );
                TTX_ENSURE( removedPage.subCode.s2    == 0x07 );
                TTX_ENSURE( removedPage.subCode.s1    == 0x00 );
            }

            // Page buffer is now P125.2F70, P126.2F70, P127.2F70, P128.2F70, P129.2F70,
            //                    P12A.2F70, P12B.2F70, P122.0000, P123.0000, P125.3F70.
#endif
        }

    }

#elif( TTX_MAX_PAGE_NUM == 5 )   // These test cases requires TTX_MAX_PAGE_NUM==5
    // Test TTX_BUF_InsPage()
    {
        TTX_STD_Page_AddrWithSubCode_t addr, removedPage;
        void TTX_MEM* pageBuf[TTX_MAX_PAGE_NUM];
        uint8_t bRemoveOccurs;

        // Case-0
        {
            TTX_BUF_Reset();

            // P100.1, P100.2, P100.3, P102.0, P899.0
            {
                // P100.1
                {
                    addr.magNum        = 1;
                    addr.pageNum.tens  = 0;
                    addr.pageNum.units = 0;
                    addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                    addr.subCode.s3    = 0x0;   //      0x00~0x0F
                    addr.subCode.s2    = 0x0;   //      0x00~0x07
                    addr.subCode.s1    = 0x1;   // LSB: 0x00~0x0F

                    bOk = TTX_BUF_FindPage( &addr, NULL );
                    TTX_ENSURE( bOk == FALSE );

                    TTX_BUF_SetPageToShow( &addr );

                    bOk = TTX_BUF_InsPage( &addr, NULL, &bRemoveOccurs, &removedPage );
                    TTX_ENSURE( bOk == TRUE );
                    TTX_ENSURE( bRemoveOccurs == FALSE );

                    TTX_BUF_SetShowingPage( &addr );
                }

                // P100.2
                {
                    addr.magNum        = 1;
                    addr.pageNum.tens  = 0;
                    addr.pageNum.units = 0;
                    addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                    addr.subCode.s3    = 0x0;   //      0x00~0x0F
                    addr.subCode.s2    = 0x0;   //      0x00~0x07
                    addr.subCode.s1    = 0x2;   // LSB: 0x00~0x0F

                    bOk = TTX_BUF_FindPage( &addr, NULL );
                    TTX_ENSURE( bOk == FALSE );

                    bOk = TTX_BUF_InsPage( &addr, NULL, &bRemoveOccurs, &removedPage );
                    TTX_ENSURE( bOk == TRUE );
                    TTX_ENSURE( bRemoveOccurs == FALSE );
                }

                // P100.3
                {
                    addr.magNum        = 1;
                    addr.pageNum.tens  = 0;
                    addr.pageNum.units = 0;
                    addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                    addr.subCode.s3    = 0x0;   //      0x00~0x0F
                    addr.subCode.s2    = 0x0;   //      0x00~0x07
                    addr.subCode.s1    = 0x3;   // LSB: 0x00~0x0F

                    bOk = TTX_BUF_FindPage( &addr, NULL );
                    TTX_ENSURE( bOk == FALSE );

                    bOk = TTX_BUF_InsPage( &addr, NULL, &bRemoveOccurs, &removedPage );
                    TTX_ENSURE( bOk == TRUE );
                    TTX_ENSURE( bRemoveOccurs == FALSE );
                }

                // P102.0
                {
                    addr.magNum        = 1;
                    addr.pageNum.tens  = 0;
                    addr.pageNum.units = 2;
                    addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                    addr.subCode.s3    = 0x0;   //      0x00~0x0F
                    addr.subCode.s2    = 0x0;   //      0x00~0x07
                    addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

                    bOk = TTX_BUF_FindPage( &addr, NULL );
                    TTX_ENSURE( bOk == FALSE );

                    bOk = TTX_BUF_InsPage( &addr, NULL, &bRemoveOccurs, &removedPage );
                    TTX_ENSURE( bOk == TRUE );
                    TTX_ENSURE( bRemoveOccurs == FALSE );
                }

                // P899.0
                {
                    addr.magNum        = 8;
                    addr.pageNum.tens  = 9;
                    addr.pageNum.units = 9;
                    addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                    addr.subCode.s3    = 0x0;   //      0x00~0x0F
                    addr.subCode.s2    = 0x0;   //      0x00~0x07
                    addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

                    bOk = TTX_BUF_FindPage( &addr, NULL );
                    TTX_ENSURE( bOk == FALSE );

                    bOk = TTX_BUF_InsPage( &addr, NULL, &bRemoveOccurs, &removedPage );
                    TTX_ENSURE( bOk == TRUE );
                    TTX_ENSURE( bRemoveOccurs == FALSE );
                }

            }

            // P101.0
            {
                addr.magNum        = 1;
                addr.pageNum.tens  = 0;
                addr.pageNum.units = 1;
                addr.subCode.s4    = 0x0;   // MSB: 0x00~0x03
                addr.subCode.s3    = 0x0;   //      0x00~0x0F
                addr.subCode.s2    = 0x0;   //      0x00~0x07
                addr.subCode.s1    = 0x0;   // LSB: 0x00~0x0F

                bOk = TTX_BUF_FindPage( &addr, NULL );
                TTX_ENSURE( bOk == FALSE );

                bOk = TTX_BUF_InsPage( &addr, NULL, &bRemoveOccurs, &removedPage );
                TTX_ENSURE( bOk == TRUE );
                TTX_ENSURE( bRemoveOccurs == TRUE );

                // The page to be removed should be P102.0
                TTX_ENSURE( removedPage.magNum        == 1 );
                TTX_ENSURE( removedPage.pageNum.tens  == 0 );
                TTX_ENSURE( removedPage.pageNum.units == 2 );
                TTX_ENSURE( removedPage.subCode.s4    == 0 );
                TTX_ENSURE( removedPage.subCode.s3    == 0 );
                TTX_ENSURE( removedPage.subCode.s2    == 0 );
                TTX_ENSURE( removedPage.subCode.s1    == 0 );
            }

        }

    }
#endif  /* end of #if( TTX_MAX_PAGE_NUM == 10 ) */


    TTX_BUF_Close();    // clean up
    return( TRUE );
}

#endif  /* end of #if TTX_VERIFY */


void TTX_BUF_Dump()
{

    uint8_t i;

    EA=0;
    for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
    {
        //TTX_Log(LOGGER_VERBOSE, "buf %d %x ",(uint32_t) i, (uint32_t) ttxBuf.pageList.elements[i].bstatus);
        //TTX_Log(LOGGER_VERBOSE, "%x ",(uint32_t) ttxBuf.pageList.elements[i].keep);
        TTX_Log(LOGGER_VERBOSE, "buf[%d] : (%x.%x) ", (uint32_t) i, (uint32_t) ttxBuf.pageList.elements[i].pageA, (uint32_t) ttxBuf.pageList.elements[i].subcodeA);
    }
    TTX_Log(LOGGER_VERBOSE, "\n");

    for ( i=0; i<TTX_MAX_PAGE_KEEP_NUM; i++ )
    {
        //TTX_Log(LOGGER_VERBOSE, "k: %d %x ",(uint32_t) i,(uint32_t) ttxBuf.keepQue.elements[i].bindex);
        TTX_Log(LOGGER_VERBOSE, "keep (%x.%x) ",(uint32_t) ttxBuf.keepQue.elements[i].pageA, (uint32_t) ttxBuf.keepQue.elements[i].subcodeA);
    }
    TTX_Log(LOGGER_VERBOSE, "\n");
    /*
     		TTX_Log(LOGGER_VERBOSE, "sub\n");

    	for( i=0; i<10; i++ ) {
    		TTX_Log(LOGGER_VERBOSE, "%x ", (uint32_t)ttxBuf.pageList.subPageIndicator[i]);
    	}
     		TTX_Log(LOGGER_VERBOSE, "\n");
    */
#ifdef BUFFER_TOP_PAGE
    for ( i=0; i<BUFFER_MAX_AIT_NUM; i++)
    {
        TTX_Log(LOGGER_VERBOSE, "AIT[%d] (%x.%x) ", (uint32_t) i,(uint32_t) ttxBuf.Ait_Mpe_Page[i].pageA, (uint32_t) ttxBuf.Ait_Mpe_Page[i].subcodeA);
    }
#endif

    EA=1;
}

extern TTX_Obj ttx;

bool_t TTX_BUF_FindNextSubPage(uint8_t startPage, TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound)
{
    uint8_t sub_num;
    uint8_t index, offset;
    int8_t i, j, start, end, found=0;
    TTX_Handle hTtx = &ttx;

    sub_num = TTX_STD_SubCodeAddrHexToInt(startPage);
    pPageAddrFound->pageA = hTtx->pageShowing.addr.pageA;

    if (sub_num<79)
        sub_num +=1;
    else
        sub_num = 0;

    index = sub_num/8;
    offset =  sub_num%8;

    TTX_Log(LOGGER_VERBOSE, "n1 %x\n",(uint32_t) sub_num);

    for (i=index;  i<10; i++)
    {
        if (ttxBuf.pageList.subPageIndicator[i]==0)
            continue;

        if (i==index)
        {
            //TTX_Log(LOGGER_VERBOSE, "map %x\n",(uint32_t)ttxBuf.pageList.subPageIndicator[i]);
            start = offset;
        }
        else
            start = 0;

        for (j=start; j<8; j++)
        {
            if (ttxBuf.pageList.subPageIndicator[i] & (0x1<<j))
            {
                found = 1;
                //TTX_Log(LOGGER_VERBOSE, "f %x %x\n",(uint32_t) i, (uint32_t) j);
                break;
            }
        }
        if (found)
            break;
    }

    if (!found)   // search from head
    {

        //TTX_Log(LOGGER_VERBOSE, "nf\n");

        for (i=0;  i<=index; i++)
        {
            if (ttxBuf.pageList.subPageIndicator[i]==0)
                continue;

            if (i==index)
                end = offset;
            else
                end = 8;

            for (j=0; j<end; j++)
            {
                if (ttxBuf.pageList.subPageIndicator[i] & (0x1<<j))
                {
                    found = 1;
                    break;
                }
            }
            if (found)
                break;

        }
    }

    if (found)
    {
        pPageAddrFound->subcodeA = (uint16_t)TTX_STD_SubCodeAddrIntToHex(i*8 + j);

//		if (pPageAddrFound->subcodeA == startPage)
        //		return( FALSE);

        TTX_Log(LOGGER_VERBOSE, "next %x\n",(uint32_t) pPageAddrFound->subcodeA);
        return( TRUE );
    }
    //TTX_Log(LOGGER_VERBOSE, "not found\n");

    return( FALSE);


}

bool_t TTX_BUF_FindPrevSubPage(TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound)
{

    uint8_t sub_num;
    uint8_t index, offset;
    int8_t i, j, start, end, found=0;
    TTX_Handle hTtx = &ttx;

    sub_num = TTX_STD_SubCodeAddrHexToInt(hTtx->pageShowing.addr.subcodeA);
    pPageAddrFound->pageA = hTtx->pageShowing.addr.pageA;

    if (sub_num>0)
        sub_num -=1;
    else
        sub_num = 79;

    TTX_Log(LOGGER_VERBOSE, "s1 %x\n",(uint32_t) sub_num);

    index = sub_num/8;
    offset =  sub_num%8;

    for (i=index;  i>=0; i--)
    {
        if (ttxBuf.pageList.subPageIndicator[i]==0)
            continue;

        if (i==index)
        {
            start = offset;
            TTX_Log(LOGGER_VERBOSE, "map %x\n",(uint32_t)ttxBuf.pageList.subPageIndicator[i]);
        }
        else
            start = 7;

        for (j=start; j>=0; j--)
        {
            if (ttxBuf.pageList.subPageIndicator[i] & (0x1<<j))
            {
                found = 1;
                break;
            }
        }
        if (found)
            break;

    }

    if (!found)   // search from head
    {
        for (i=9;  i>=index; i--)
        {
            if (ttxBuf.pageList.subPageIndicator[i]==0)
                continue;

            if (i==index)
                end = offset;
            else
                end = 0;

            for (j=7; j>=end; j--)
            {
                if (ttxBuf.pageList.subPageIndicator[i] & (0x1<<j))
                {
                    found = 1;
                    break;
                }
            }
            if (found)
                break;

        }
    }

    if (found)
    {
        pPageAddrFound->subcodeA = (uint16_t)TTX_STD_SubCodeAddrIntToHex(i*8 + j);
        TTX_Log(LOGGER_VERBOSE, "prev %x\n",(uint32_t) pPageAddrFound->subcodeA);
        return( TRUE );
    }

    TTX_Log(LOGGER_VERBOSE, "not found\n");

    return( FALSE);
}

void TTX_BUF_ResetSubPageList()
{
    memset(ttxbuf_pageList, 0x0, sizeof(ttxbuf_pageList));
}




/*****************************************************************************\
 *  @brief   Update *pPageAddr's sub-page info *pPageNavInfo by searching
 *             all content of page-buffer.
 *  @param   *pPageAddr is the page address you want to look up.
\*****************************************************************************/
bool_t TTX_BUF_GetSubPageNavCtrlInfo(int* pCursor_Index, TTX_DISP_SubPageNavCtrl_t*     pPageNavInfo)
{
    TTX_STD_SubPage_Displayable_Num_t i=0;
    uint8_t offset = 0;

    /*
    	for(i=0; i<10; i++)
    		RTD_Log(LOGGER_INFO, "index : %d, indicator : %x \n", (UINT32) i, (UINT32) ttxBuf.pageList.subPageIndicator[i]);
    */
    if (m_hTtx->pageShowing.addr.subcodeA ==0x0)
    {
        *pCursor_Index = 0;
        ttx_subpage_auto_rotation.rotate =FALSE;
        //RTD_Log(LOGGER_INFO, "pageShowing.addr.subcodeA == 0, reset all and leave subpage showing\n");
        return FALSE; // It won't have sub pages
    }


    pPageNavInfo->cntTotal = 0; //drop pageList[0] for auto rotate element

    TTX_BUF_ResetSubPageList();

    for (i=0 ; i<10; i++)
        for (offset=0; offset<8; offset++)
        {
            if (ttxBuf.pageList.subPageIndicator[i] & (0x1<<offset))
            {
                ttxbuf_pageList[pPageNavInfo->cntTotal] = i*8+offset;
                //RTD_Log(LOGGER_INFO, "pageList[%d]  : %x \n", (UINT32) pPageNavInfo->cntTotal, (UINT32) ttxbuf_pageList[pPageNavInfo->cntTotal]);
                pPageNavInfo->cntTotal  ++;

            }
        }

    if (*pCursor_Index > pPageNavInfo->cntTotal)
        *pCursor_Index = 0;
    if (*pCursor_Index < 0)
        *pCursor_Index = pPageNavInfo->cntTotal;

    ttx_select_subpage.pageA |= m_hTtx->usrConfig.pageToShow.pageAddr.pageA;

    if (*pCursor_Index != 0)
    {
        ttx_subpage_auto_rotation.rotate =FALSE;
        if (!ttx_select_subpage.user_input_status)
            ttx_select_subpage.subcodeA = m_hTtx->usrConfig.pageToShow.pageAddr.subcodeA;
        //RTD_Log(LOGGER_INFO, "TTX_BUF_GetSubPageNavCtrlInfo : auto rotate = FALSE \n");
    }
    else
    {
        if (!ttx_select_subpage.user_input_status)
            ttx_select_subpage.subcodeA = 0xFFFF;
        ttx_subpage_auto_rotation.rotate =TRUE;
        //RTD_Log(LOGGER_INFO, "TTX_BUF_GetSubPageNavCtrlInfo : auto rotate = TRUE \n");
    }



    pPageNavInfo->idxHighlight = (*pCursor_Index >= TTX_DISP_SUBPAGE_MAX_NUM)? TTX_DISP_SUBPAGE_MAX_NUM:(*pCursor_Index%TTX_DISP_SUBPAGE_MAX_NUM);

    if (pPageNavInfo->idxHighlight>0)
        pPageNavInfo->idxHighlight -= 1; //skip auto rotate

    pPageNavInfo->cnt            = 0;
    pPageNavInfo->idxStart =(*pCursor_Index>=TTX_DISP_SUBPAGE_MAX_NUM)? (*pCursor_Index-TTX_DISP_SUBPAGE_MAX_NUM):0;

    //RTD_Log(LOGGER_INFO, "Total Subpage Number : %x, Cursor_Index %x, idxStart : %x, highlight : %x \n", (UINT32) pPageNavInfo->cntTotal, (UINT32) *pCursor_Index,
    //	(UINT32) pPageNavInfo->idxStart, (UINT32)pPageNavInfo->idxHighlight);

    for (i=pPageNavInfo->idxStart; (i<pPageNavInfo->cntTotal)&&(pPageNavInfo->cnt<TTX_DISP_SUBPAGE_MAX_NUM); i++, pPageNavInfo->cnt++)
    {
        pPageNavInfo->list[pPageNavInfo->cnt] = ttxbuf_pageList[i];
        //RTD_Log(LOGGER_INFO, "i = %d, pPageNavInfo->list[%d]  : %x \n", (UINT32) i, (UINT32) pPageNavInfo->cnt, (UINT32) pPageNavInfo->list[pPageNavInfo->cnt]);
    }
    return( TRUE );
}

uint8_t TTX_BUF_Get_First_Subcode()
{
    int i;

    for (i=0; i<80; i++)
    {
        if (ttxbuf_pageList[i] != 0)
        {
            return ttxbuf_pageList[i];
        }
    }
    return 0xFF;
}


uint8_t TTX_BUF_Get_Last_Subcode()
{
    int i;

    for (i=79; i>=0; i--)
    {
        if (ttxbuf_pageList[i] != 0)
            return ttxbuf_pageList[i];
    }
    return 0xFF;

}

uint16_t TTX_BUF_Get_Index_By_Subcode(uint16_t subcode)
{
    int i;
    bool_t result = FALSE;
    uint16_t index = 0;

    for (i=0; i<80; i++)
    {
        if (ttxbuf_pageList[i] != 0)
        {
            if (ttxbuf_pageList[i] == subcode)
            {
                result = TRUE;
                break;
            }
            index++;
        }
    }
    if (!result)
        index = 0xFF;
    //RTD_Log(LOGGER_INFO, "TTX_BUF_Get_Index_By_Subcode : subcode %x, Find Idx : %d \n",  (UINT32) subcode,  (UINT32) index);
    return index;
}


void TTX_BUF_Sync_Cursor_Page(int cursor_idx)
{
    int i;
    int index = 0;
    bool_t result = FALSE;
    TTX_STD_Page_AddrWithSubCode_t pageAddr;

    for (i=0; i<80; i++)
    {
        if (ttxbuf_pageList[i] != 0)
        {
            index++;
            if (index == cursor_idx)
            {
                result = TRUE;
                break;
            }
        }
    }

    if (result == FALSE)	//can not get the result
        return;
    else
    {
        /*
        	RTD_Log(LOGGER_INFO, "cursor index = %d, index = %d, i = %d, subcode = %x, showing page subcode= %x \n",  (UINT32) cursor_idx, (UINT32) index ,
        		(UINT32) i , (UINT32) ttxbuf_pageList[i],  (UINT32) m_hTtx->pageShowing.addr.subcodeA);
        */
        if (!TTX_STD_PageAddrSubCodeIsValid(ttxbuf_pageList[i]))
        {
            RTD_Log(LOGGER_INFO, "Error ! TTX_BUF_Sync_Cursor_Page : not valid subpage code! \n");
            return;
        }

        if (ttxbuf_pageList[i] != m_hTtx->pageShowing.addr.subcodeA)
        {
            pageAddr.pageA = m_hTtx->pageShowing.addr.pageA;
            pageAddr.subcodeA= ttxbuf_pageList[i];
            TTX_NAV_SetPageToShow(&pageAddr);
        }
    }


}

/*---------------------------------------------------------------------------*\
 * end of ttx_buff.c                                                         *
\*---------------------------------------------------------------------------*/
#endif
