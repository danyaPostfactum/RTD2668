/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_source.c
 * @brief    Teletext input data processor.
 * @author   Janus Cheng
 * @date     Apr. 25, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_source.h> /* Provides: functions to be implemented */

#include <ttx_obj.h>    /* Provides: TTX_Handle */
#include <ttx_buff.h>   /* Provides: TTX_BUF_InsPage() */
#include <ttx_assert.h> /* Provides: TTX_REQUIRE() and TTX_ENSURE() */
#include <ttx_nav.h>    /* Provides: TTX_NAV_UpdateSubPageInfo() */
#include <ttx_buff_pkg.h>   /* Provides: TTX_DEFAULT_CD_PAGE_ADDRESS */
#include <ttx_evt.h>        /* Provides: functions to be implemented */
#include <ttx_disp.h>	/* Provides: ttx_subpage_auto_rotation */

#include "ap_rtirkey.h"
#include "Message.h"

#include <ppr.h>        /* Provides: PPR module */
#include <slr.h>        /* Provides: Slicer module */

#include <string.h>     /* Provides: memcpy() */

#include "rosprintf.h"
#include "char\uart.h"
#include "pcb_customer.h"
#include "flow_conf_struct.h"
#include <stdarg.h>
#include <reg52.h>
#include "ui_ctrl.h"
void TT_Log(_LOGGER_LEVEL level, char code *format, ...);

/* -- Auto-computed Macros --------------------------------------------------*/

/* private object, not to be used by application code */

TTX_SRC_CollectingPage_t   collectingPages[TTX_MAG_NUM_TOTAL];

volatile UINT32 myTimer1 = 0;
volatile UINT32 myTimer2 = 0;
volatile UINT32 myTimer3 = 0;

uint32_t trigger_timer = 0;

uint32_t bdata X0_header1;
sbit tmpBit14          =  X0_header1 ^ 22;
sbit tmpBit13          =  X0_header1 ^ 21;
sbit tmpBit12          =  X0_header1 ^ 20;
sbit tmpBit11          =  X0_header1 ^ 19;
sbit tmpBit10          =  X0_header1 ^ 18;
sbit tmpBit9          =  X0_header1 ^ 17;
sbit tmpBit8          =  X0_header1 ^ 16;
sbit tmpBit3          =  X0_header1 ^ 27;

extern TTX_Handle xdata m_hTtx;


#if 0
/*---------------------------------------------------------------------------*/
TTX_STD_Page_Addr_SubCode_Hex_t
PageAddrSubCodeToHex (const TTX_STD_Page_Addr_SubCode_t* pPageAddressSubCode) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{
    TTX_STD_Page_Addr_SubCode_Hex_t ret = 0;

    ret  =  (TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s1;
    ret |= ((TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s2) << 4;
    ret |= ((TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s3) << 8;
    ret |= ((TTX_STD_Page_Addr_SubCode_Hex_t)pPageAddressSubCode->s4) <<12;

    return( ret );
}

/*---------------------------------------------------------------------------*/
uint8_t
PageAddrWithSubCodeAreEqual (const TTX_STD_Page_AddrWithSubCode_t* pAddr1, const TTX_STD_Page_AddrWithSubCode_t* pAddr2, uint8_t bCompareSubPage ) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Compare the page-addresses *pAddr1 and *pAddr2 are equal or not
 *             based on the comparison option: compSubpage.
 *  @param   *pAddr1          The page-address to be compared.
 *  @param   *pAddr2          The page-address to be compared.
 *  @param   bCompareSubPage  Compare the subcode of *pAddr1 and *pAddr2 or not.
 *
 *  @return  *pAddr1 and *pAddr2 are equal or not.
 *  @retval  TRUE         *pAddr1 and *pAddr2 are identical.
 *  @retval  FALSE        *pAddr1 and *pAddr2 are different.
 *
 *  @pre     pAddr1 != NULL
 *  @pre     pAddr2 != NULL
 *  @pre     bCompareSubPage == TRUE or FALSE
\*****************************************************************************/
{
    TTX_STD_Page_Addr_SubCode_Hex_t subPageNum1, subPageNum2;

    if ( pAddr1->magNum != pAddr2->magNum )
        return( FALSE );

    if ( pAddr1->pageNum.tens != pAddr2->pageNum.tens )
        return( FALSE );

    if ( pAddr1->pageNum.units != pAddr2->pageNum.units )
        return( FALSE );

    if ( bCompareSubPage == FALSE )
        return( TRUE );

    subPageNum1 = PageAddrSubCodeToHex( &( pAddr1->subCode ) );
    subPageNum2 = PageAddrSubCodeToHex( &( pAddr2->subCode ) );
    /*
        // Subpage zero is special. Skip subpage comparison.
        if( (subPageNum1 == 0) || (subPageNum2 == 0) )
            return( TRUE );
    */
    if ( subPageNum1 != subPageNum2 )
        return( FALSE );

    return( TRUE );
}
#endif

/*---------------------------------------------------------------------------*/
static void
TTX_SRC_StartCollectingPage(const TTX_SRC_CollectingPage_t* pPage,
                            uint8_t index)
/*****************************************************************************\
 *  @brief   Set *pPageAddr as being collected.
 *  @param   *pDest points to the destination address you want to collect to.
\*****************************************************************************/
{
    TTX_Address_t addrReg;
    TTX_Address_t dest;
    uint8_t mag =  (pPage->pageAddr.pageA>>8) -1;

    /* Update collectingPagesAddr[] */
    collectingPages[mag].pageAddr = pPage->pageAddr;
    collectingPages[mag].ctrlBits = pPage->ctrlBits;
    collectingPages[mag].buf_index = index;
    pPage->buf_index = index;

    // set the page buffer dest address
    dest   = ((uint16_t)ttxBuf.pageList.elements[index].addrMem) >> 3; // 8-byte basis
    addrReg = VBIPPR_MAG1_DES_BADDR_REG_0x40b8 + (mag)*4;
    rtd_isr_outl(addrReg, dest);

    // enable magzine
    PPR_PfeSetMag(mag, TRUE);

}



/*---------------------------------------------------------------------------*/
static void
TTX_SRC_onStoredPage  ( const TTX_SRC_CollectingPage_t* pPage)
/*****************************************************************************\
 *  @brief   Handling event of the broadcaster is terminating a page whose
 *             address is *pPageAddr.
 *  @note    *pPage is stored in page-buffer.
 *  @param   *pPageAddr is the page address of received page.
\*****************************************************************************/
{
    uint8_t buf_index;
    uint8_t mag =  (pPage->pageAddr.pageA>>8) -1;
    uint8_t TTX_MEM* src = ttxBuf.pageList.elements[pPage->buf_index].addrMem;
    static uint8_t data1 = 0;

    /*	if(pPage->pageAddr.pageA ==0x101) {
    		TTX_Log(LOGGER_VERBOSE, "==> %x %x %x\n",
    			(uint32_t)src[120],
    			(uint32_t)src[121],
    			(uint32_t)src[122]);
    	}
    */
    /* Pattern: P100 of SWISS_Lugano_Television_Suisse_Romande_1108_0659_pid0x4a.PES is
         *           ( pPage->ctrlBits.bUpdate              == 0 ) but
         *           ( pPage->ctrlBits.bInterruptedSequence == 1 ).
         */
//	if( pPage->ctrlBits.bUpdate || pPage->ctrlBits.bInterruptedSequence ) {
    if ((m_hTtx->pageShowing.addr.pageA == pPage->pageAddr.pageA) &&
            (m_hTtx->pageShowing.addr.subcodeA == pPage->pageAddr.subcodeA))
    {
        if (pPage->pageAddr.pageA ==0x101)   // dirty patch for P101 of fluke
        {
            if (data1!= src[120])
            {
                data1 = src[120];
                TTX_EVT_Fire( DEC, UPDATE_PAGE ); // Force to redraw screen
            }
        }
        else
            TTX_EVT_Fire( DEC, UPDATE_PAGE ); // Force to redraw screen
    }
    else
    {
        //TTX_Log(LOGGER_INFO,"Debug area : pageshowing, pageA (%x), subcodeA (%x); pPage, pageA (%x), subcodeA (%x) \n",
        //		(UINT32) m_hTtx->pageShowing.addr.pageA, (UINT32) m_hTtx->pageShowing.addr.subcodeA,  (UINT32) pPage->pageAddr.pageA, (UINT32) pPage->pageAddr.subcodeA); // debug
    }
//	}


    /* *page is currently auto-rotation. */
    if (  (ttx_subpage_auto_rotation.hold == FALSE) &&
            (ttx_subpage_auto_rotation.rotate == TRUE) &&
            // The stored page is showing page, but it doesn't need to be updated.
            (!(TTX_ISR_PageAddrWithSubCodeAreEqual( &(m_hTtx->pageShowing.addr), &(pPage->pageAddr), TRUE ))) &&
            // The stored page has the same page number and different subcode of the showing page.
            // We do auto-rotation for this case.
            (TTX_ISR_PageAddrWithSubCodeAreEqual( &(m_hTtx->pageShowing.addr), &(pPage->pageAddr), FALSE )) )
    {
        //TTX_NAV_SetPageToShow(&(pPage->pageAddr));
        //TTX_Log(LOGGER_INFO,"auto-rotating subpage by source\n"); // debug
        if (trigger_timer > 1)
        {
            fw_isr_ui_message_push(_TTX_NEXT_SUBPAGE);
            trigger_timer = 0;
        }
        else
            trigger_timer++;
    }



//	if((pPage->pageAddr.pageA &0xFF) ==0xF0) {  // received BTT page, call decoder to update
//		TTX_Log(LOGGER_VERBOSE, "UF\n");
//		TTX_EVT_Fire( m_hTtx, DEC, UPDATE_PAGE ); // Force to redraw screen
//	}


//	TTX_Log(LOGGER_VERBOSE, "sp %x %d\n", (uint32_t)pPage->pageAddr.pageA, (uint32_t) pPage->buf_index);

    // ghyu, should add code later!!!
    /* SubPage nav control is only renderned in TTX_STATE_NORMAL */
    /* If *pPage is a subpage of showing page */
    //if( PageAddrWithSubCodeAreEqual( &(m_hTtx->pageShowing.addr), &(pPage->pageAddr), FALSE ) ) {
    //TTX_EVT_Fire( m_hTtx, DISP, UPDATE_NAV_CTRL );
//	}

    /* Update collectingPagesAddr[] */
    buf_index = collectingPages[mag].buf_index;
    ttxBuf.pageList.elements[buf_index].bstatus = TTX_BUFFER_COLLECTED;  // change buffer status to collected
    // StopCollectingMag
    PPR_PfeSetMag(mag, FALSE);
}


/*
	@brief   Allocate memory from page buffer to collect incoming page.
	@param   *pIncomingPage points to the destination address you want to collect to.
*/
void TTX_SRC_AllocateAndStartCollectingPage(const TTX_SRC_CollectingPage_t* pIncomingPage)
{

    /* Allocate memory from page buffer to collect incoming page. */
    uint8_t old_mag;
    void TTX_MEM* pDest;    /* points to page buffer for storing the *pIncomingPage */

    uint8_t buf_index;


    //RtdCtrl_isr_Mask(TCWTR_0x2874, 0xffffffff, (UINT32)_BIT0);

    //TT_Log(LOGGER_INFO, " pIncomingPage (%x.%x) \n", (UINT32 )pIncomingPage->pageAddr.pageA, (UINT32) pIncomingPage->pageAddr.subcodeA);


    if (!TTX_BUF_ISRFindPage( &(pIncomingPage->pageAddr), &buf_index ))
    {
        // no need to collect this page
        //TT_Log(LOGGER_INFO, " no need to collect this page (%x.%x) \n", (UINT32 )pIncomingPage->pageAddr.pageA, (UINT32) pIncomingPage->pageAddr.subcodeA);
        return;
    }

    // if the buffer is on decoding, skip it
    if ( ttxBuf.pageList.elements[buf_index].bstatus == TTX_BUFFER_ONDECODE )
    {
        //TT_Log(LOGGER_INFO, "(page : %x.%x) the buffer is on decoding, skip it \n", (UINT32) pIncomingPage->pageAddr.pageA, (UINT32) pIncomingPage->pageAddr.subcodeA);
        return;
    }

    if ((ttxBuf.pageList.elements[buf_index].pageA != pIncomingPage->pageAddr.pageA ) ||
            (ttxBuf.pageList.elements[buf_index].subcodeA!= pIncomingPage->pageAddr.subcodeA))    // replacing buffer happened
    {
        if ( ttxBuf.pageList.elements[buf_index].bstatus == TTX_BUFFER_COLLECTING )
        {
            // replace happened, sould stop the page collect first!
            old_mag = ((ttxBuf.pageList.elements[buf_index].pageA & 0xf00)>>8) -1;
            PPR_PfeSetMag(old_mag, FALSE);
        }
        pDest   = ((uint16_t)ttxBuf.pageList.elements[buf_index].addrMem); // 8-byte basis
        if ( ttxBuf.pageList.elements[buf_index].X27_status == TTX_X27_EMPTY )
        {
            memset( (uint8_t TTX_MEM*)pDest+40, 0, TTX_DEFAULT_PAGE_SIZE-40 );  // clear the memory other than X/0
        }
        else if (ttxBuf.pageList.elements[buf_index].X27_status != TTX_X27_COLLECTING )    // when collect FLOF, shouldn't clear main page data!!
        {
#if SPARROW_LEVEL_15_MEMORY_LAYOUT
            memset( (uint8_t TTX_MEM*)pDest+40, 0, 25*40 );  // clear the memory other than X/0 and PFE Mode 1
#else
            memset( (uint8_t TTX_MEM*)pDest+40, 0, TTX_DEFAULT_PAGE_SIZE-80 );  // clear the memory other than X/0 and X/27
#endif
        }
//			TTX_Log(LOGGER_VERBOSE, "m2 %d %x\n", (uint32_t)TTX_DEFAULT_PAGE_SIZE, (uint32_t)pDest);

//			TTX_Log(LOGGER_VERBOSE, "m2\n");
    }
    else     // the sam buffer
    {

        /* If *pIncomingPage has already been collected in page buffer */
        if (( pIncomingPage->ctrlBits.bErasePage || pIncomingPage->ctrlBits.bUpdate ) )
        {
            /* Apply bErasePage (C4) and bUpdate (C8) flag to existing page */
            pDest   = ((uint16_t)ttxBuf.pageList.elements[buf_index].addrMem); // 8-byte basis
//			TTX_Log(LOGGER_VERBOSE, "m1 %d %x\n", (uint32_t)TTX_DEFAULT_PAGE_SIZE, (uint32_t)pDest);

            if ( pIncomingPage->ctrlBits.bErasePage )
            {
                if ( ttxBuf.pageList.elements[buf_index].X27_status == TTX_X27_EMPTY )
                {
                    memset( (uint8_t TTX_MEM*)pDest+40, 0, TTX_DEFAULT_PAGE_SIZE-40 );  // clear the memory other than X/0
                }
                else if (ttxBuf.pageList.elements[buf_index].X27_status != TTX_X27_COLLECTING )    // when collect FLOF, shouldn't clear main page data!!
                {
                    memset( (uint8_t TTX_MEM*)pDest+40, 0, 25*40 );  // clear the memory other than X/0 and PFE Mode 1
                }
            }
        }
    }

    /* Now we're collecting *pIncomingPage */
    ttxBuf.pageList.elements[buf_index].bstatus = TTX_BUFFER_COLLECTING ;
    ttxBuf.pageList.elements[buf_index].pageA = pIncomingPage->pageAddr.pageA;
    ttxBuf.pageList.elements[buf_index].subcodeA = pIncomingPage->pageAddr.subcodeA;

//	TTX_Log(LOGGER_VERBOSE, "as %d\n", (uint32_t) buf_index);

    TTX_SRC_StartCollectingPage( pIncomingPage, buf_index);
}

/*
	Set the page corresponding bit, it means page valid
	We use a pageMap (800 bits) to record which page is valid);
*/
static void TTX_SRC_SetPageMap(uint8_t mag, uint8_t tens, uint8_t units)
{
    uint16_t page_num;
    uint8_t index, offset;

    page_num = 100*(mag-1) + tens*10 + units;
    index = page_num/8;
    offset =  page_num%8;

    m_hTtx->pageMap[index] |=  (0x1<<offset);

//	TTX_Log(LOGGER_VERBOSE, "sm %x %x %x %x\n",(uint32_t) index, (uint32_t) offset, (uint32_t) m_hTtx->pageMap[index],  (uint32_t)page_num);

}

/*---------------------------------------------------------------------------*/
//static uint8_t
uint8_t
TTX_SRC_isDisplayablePage               (const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress) TTX_REENTRANT
/*****************************************************************************\
 *  @brief   Convert the presentation of the colors in VbiOsd_clutValue from RGB to YUV
 *  @param   triplet1    bit14~bit8 of triplet1
 *  @param   c12c13c14   national option selection bits
 *  @note    Section 15.2 (P.101)
\*****************************************************************************/
{

    if (( pPageAddress->pageA & 0xf0) > 0x090 )
    {
        return( FALSE );
    }

    if (( pPageAddress->pageA &0xf) > 0x9 )
    {
        return( FALSE );
    }

    // currently, we don't care about subcode
    /*
            if( pPageAddress->subcodeA & 0xff00 != 0 )
            {
                return( FALSE );
            }

            if( (pPageAddress->subcodeA & 0xf0) > (( TTX_STD_SUBPAGE_DISPLAYABLE_MAX_NUM / 10 )<<4) )
            {
                return( FALSE );
            }

            if( ( pPageAddress->subcodeA &0xf) > 9 )
            {
                return( FALSE );
            }
    */
    return( TRUE );
}

bool_t TTX_STD_isDisplayablePage2
(const TTX_STD_Page_AddrWithSubCode_t*  pPageAddress)
/*****************************************************************************\
 * @brief		Check if this page number (ignore subpage) is displayable page
 * @param	<pPageAddress>	page address
\*****************************************************************************/
{
    TTX_REQUIRE( pPageAddress != NULL );
    if ( pPageAddress == NULL )
    {
        return( FALSE );
    }

    // page < 0x99
    {
        if ( (pPageAddress->pageA & 0xf0) > 0x90 )
        {
            return( FALSE );
        }

        if ( (pPageAddress->pageA & 0xf) > 0x9 )
        {
            return( FALSE );
        }
    }

    // subpage <= 0x3F7F
    {
        if ( (pPageAddress->subcodeA & 0xf000) > 0x3000 )
        {
            return( FALSE );
        }
        /*
        		if ( (pPageAddress->subcodeA & 0x0f00) > 0x0f00 )
        		{
        			return( FALSE );
        		}
        */
        if ( (pPageAddress->subcodeA & 0x00f0)> 0x0070 )
        {
            return( FALSE );
        }
        /*
        		if ( (pPageAddress->subcodeA & 0x000f)> 0x000f )
        		{
        			return( FALSE );
        		}
        */
    }

    return( TRUE );
}


/*---------------------------------------------------------------------------*/
static bool_t
TTX_SRC_GetCollectingPageByMagazine (const TTX_STD_MagazineNum_t mag,
                                     TTX_SRC_CollectingPage_t* pPage)
/*****************************************************************************\
 *  @brief   Get the page address of collecting mag.
 *  @param   mag  is the magazine number that you want to query.
 *  @ret     Whether mag is being collected or not.
 *  @retval  TRUE  if mag is     being collected.
 *  @retval  FALSE if mag is not being collected.
\*****************************************************************************/
{
    uint32_t val;

    val  = (0x00000001UL << (mag-1));

    // check if magazine is collecting
    if (rtd_isr_inl(VBIPPR_PFE_ACTIVE_REG_0x40dc) & val)    // magazine on collecting
    {
        *pPage = collectingPages[ mag -1 ];
        return( TRUE );
    }

    return( FALSE );
}

#if SPARROW_LEVEL_15_MEMORY_LAYOUT

void TTX_SRC_Fetch_PfeMode1(const uint8_t* magNum, const uint16_t* pfe_nproc_addr, uint8_t* fetched_line)
{
    TTX_Address_t addrReg;
    uint32_t dest;
    uint8_t TTX_MEM* pPFE_Mode0_Buf = NULL;
    uint8_t TTX_MEM* pPFE_Mode1_Buf = NULL;

    // magazine index and register
    uint8_t mag = (*magNum) - 1;
    addrReg = VBIPPR_MAG1_DES_BADDR_REG_0x40b8 + (mag)*4;

    // mode 0 address
    pPFE_Mode0_Buf = (uint8_t TTX_MEM*)ttxBuf.pageList.elements[collectingPages[mag].buf_index].addrMem;

    // mode 1 address
    pPFE_Mode1_Buf = pPFE_Mode0_Buf + 26 * 40;

    // change PPR address to mode 1
    dest = (uint32_t)pPFE_Mode1_Buf >> 3; // 8-byte basis
    rtd_isr_outl(addrReg, dest);

    // restore next process address
    rtd_isr_outl(VBIPPR_PFE_NPROC_ADDR_REG_0x40e0, (uint32_t)(*pfe_nproc_addr));

    // PFE mode 1
    PPR_PfeMove(PPR_PFE_MODE1, fetched_line);

    // restore PPR address to mode 0
    dest = (uint32_t)pPFE_Mode0_Buf >> 3; // 8-byte basis
    rtd_isr_outl(addrReg, dest);
}

#else

#define X_27_TEST 0

void TTX_SRC_Fetch_X_27_0(const uint8_t* magNum, const uint16_t* pfe_nproc_addr, uint8_t* fetched_line)
{
    TTX_Address_t addrReg;
    TTX_Address_t dest;
    uint8_t TTX_MEM* pPFE_Mode1_Buf = (uint8_t TTX_MEM*)TTX_FLOF_PFE_MODE1_PAGE_ADDRESS;
#if X_27_TEST
    uint8_t* pTest = NULL;
#endif

    uint8_t mag = (*magNum) - 1;
    addrReg = VBIPPR_MAG1_DES_BADDR_REG_0x40b8 + (mag)*4;

    // clear X/27/0
    memset(pPFE_Mode1_Buf+0x258, 0, 40);

    // change PPR address
    dest = (uint16_t)pPFE_Mode1_Buf >> 3; // 8-byte basis
    rtd_isr_outl(addrReg, dest);

    // restore next process address
    rtd_isr_outl(VBIPPR_PFE_NPROC_ADDR_REG_0x40e0, (uint32_t)(*pfe_nproc_addr));
    //rtd_isr_outl(VBIPPR_PFE_NPROC_ADDR_REG_0x40e0, (uint32_t)(0)); // test

    // PFE mode 1
    PPR_PfeMove(PPR_PFE_MODE1, fetched_line);

    // restore PPR address
    dest = ((uint16_t)ttxBuf.pageList.elements[collectingPages[mag].buf_index].addrMem) >> 3; // 8-byte basis
    rtd_isr_outl(addrReg, dest);

#if X_27_TEST
    pTest = pPFE_Mode1_Buf + 0x258;
    pTest[0] = 0x00;
    pTest[1] = 0x49;
    pTest[2] = 0x15;
    pTest[3] = 0xEA;
    pTest[4] = 0x2F;
    pTest[5] = 0xEA;
    pTest[6] = 0x5E;
    pTest[7] = 0x15;
    pTest[8] = 0x15;
    pTest[9] = 0xEA;
    pTest[10] = 0xEA;
    pTest[11] = 0xEA;
    pTest[12] = 0x2F;
    pTest[13] = 0x02;
    pTest[14] = 0x02;
    pTest[15] = 0xEA;
    pTest[16] = 0x2F;
    pTest[17] = 0xEA;
    pTest[18] = 0x5E;
    pTest[19] = 0x02;
    pTest[20] = 0x15;
    pTest[21] = 0xEA;
    pTest[22] = 0x2F;
    pTest[23] = 0xEA;
    pTest[24] = 0x5E;
    pTest[25] = 0xEA;
    pTest[26] = 0xEA;
    pTest[27] = 0xEA;
    pTest[28] = 0xEA;
    pTest[29] = 0xEA;
    pTest[30] = 0x5E;
    pTest[31] = 0x15;
    pTest[32] = 0x15;
    pTest[33] = 0xEA;
    pTest[34] = 0x2F;
    pTest[35] = 0xEA;
    pTest[36] = 0x5E;
    pTest[37] = 0xEA;
    pTest[38] = 0x57;
    pTest[39] = 0x7B;
#endif

    // save X/27/0
    if ( (*(pPFE_Mode1_Buf+0x280-1) != 0) || (*(pPFE_Mode1_Buf+0x280-2) != 0) )
    {
        memcpy(
            ttxBuf.pageList.elements[collectingPages[mag].buf_index].addrMem+25*40, // replace X/25
            pPFE_Mode1_Buf+0x258, 40 );
        //RTD_Log(LOGGER_INFO, "w 27\n"); // debug
    }
    else
    {
        //RTD_Log(LOGGER_INFO, "wo 27\n"); // debug
    }
}

#endif

// This is the ISR handler for PPR field data valid
void TTX_SRC_PprFetch(void)
{
    uint32_t pkt_x0_ind;
    uint8_t packet_index =0, magNum, tens, units;
    uint32_t X0_header0; //, X0_header1;
    uint16_t pfe_nproc_addr;    // @todo NPROC_ADDR is only uint8_t, so this variable can be shrink to uint16_t
    uint16_t pfe_nproc_addr_orig = 0;
    static uint8_t last_mag = 0;

    static uint8_t magNumPrev = 0xff, pageNumPrev = 0xff; // previous mag and page
    static uint16_t subPagePrev = 0xffff; // previous sub page
    static uint8_t fetched_line = 0; // previous fetched line number
    uint8_t tmp_fetched_line = 0;
    static bool_t mag_serial = FALSE; // previous serial mode

    uint32_t FieldBuff[2] = 0;
    uint16_t timecode = 0;

    static TTX_X27_STATUS_t currPFEStatus = TTX_X27_EMPTY;

    if ( m_hTtx == NULL )
    {
        return;
    }

//	rtd_isr_outl(0x285c, 0);
//	rtd_isr_outl(0x285c, _BIT31);

#if 0 // debug
    rtd_outl(VBIPPR_LVLD_BTM_REG_0x42e4, 0x0);
    rtd_outl(VBIPPR_LVLD_TOP_REG_0x42e0, 0x0);
#endif

    do
    {
        //RtdCtrl_isr_Mask(TCWTR_0x2874, 0xffffffff, (UINT32)_BIT0); Reset watch dog for print msg

        // read the PPR next process address
        pfe_nproc_addr = rtd_isr_inl( VBIPPR_PFE_NPROC_ADDR_REG_0x40e0) ; // PFE_NPROC_ADDR is 4-byte basis
        pfe_nproc_addr_orig = pfe_nproc_addr;

        // due to the pfe_nproc_addr is 4 bytes basis, so we / 10
        packet_index = pfe_nproc_addr / 10;  // 10*4 bytes = 40

        // check first position of X/0
        pkt_x0_ind = rtd_isr_inl(VBIPPR_FB_IND_REG_0x40b0) & 0x3ffff;

        //TT_Log(LOGGER_INFO, "pkt_x0_ind : %x,  packet_index : %x \n", (UINT32) pkt_x0_ind, (UINT32) packet_index);

        if ( pkt_x0_ind & (UINT32_C(1) << packet_index) )  // first packet is X/0
        {

            TTX_SRC_CollectingPage_t  nowPage, prePage;

            pfe_nproc_addr = pfe_nproc_addr <<2; //  pfe_nproc_addr is 4 bytes basis, recover it!
            X0_header0 = rtd_isr_inl( VBIFB_WORDX_REG_0_0x4400+ pfe_nproc_addr );
            X0_header1 = rtd_isr_inl( VBIFB_WORDX_REG_0_0x4400+ pfe_nproc_addr+4 );

            FieldBuff[0] =  rtd_isr_inl( VBIFB_WORDX_REG_0_0x4400+ pfe_nproc_addr+32);
            FieldBuff[1] =  rtd_isr_inl( VBIFB_WORDX_REG_0_0x4400+ pfe_nproc_addr+36);

            // get the magazine number
            magNum               = (*((UINT8 xdata*)&X0_header0))&0x7;


            //TT_Log(LOGGER_VERBOSE, "he %x\n", X0_header0);
            //TT_Log(LOGGER_VERBOSE, "he1 %x\n", X0_header1);

            // Convert magazine_num to valid ranges of TTX_STD_MagazineNum_t.
            if ( magNum == 0 )
                magNum = 8;

            nowPage.pageAddr.pageA = (magNum<<8) | (X0_header0&0xFF);
            nowPage.pageAddr.subcodeA = (X0_header1>>16) & 0x3f7f;

            //TT_Log(LOGGER_INFO, "magNum : %x,  pageA : %x, subcodeA : %x \n", (UINT32) magNum, (UINT32) nowPage.pageAddr.pageA,
            //	(UINT32) nowPage.pageAddr.subcodeA);

            // subcode = 0x3F7F
            if (nowPage.pageAddr.subcodeA == 0x3F7F)
                nowPage.pageAddr.subcodeA = 0;

            // Annex E.2 Page Time-Code
            if (nowPage.pageAddr.subcodeA != 0)
            {
                timecode = ((((*((uint8_t*) &FieldBuff[0])) - '0')<<12) | (((*((uint8_t*) &FieldBuff[0]+1)) - '0')<<8)
                            | (((*((uint8_t*) &FieldBuff[0]+3)) - '0')<<4)|((*((uint8_t*) &FieldBuff[1])) - '0'));

                if (nowPage.pageAddr.subcodeA== timecode)
                    nowPage.pageAddr.subcodeA = 0;
            }

            //For the check condition of TT signal is available or not.
            if (m_hTtx->Correct_X0_cnt < 3)
                m_hTtx->Correct_X0_cnt++;

            if ( fetched_line == 1 )   // move 1 line and then meet X/0 again
            {
                if ( (magNum == magNumPrev) && ( (X0_header0&0xFF) == pageNumPrev) && (nowPage.pageAddr.subcodeA == subPagePrev) )   // the same page, redundant X/0
                {
                    goto no_collect;
                }
            }

            // we need to record which displayable page we've ever received
            // It will be used for next, prev page key
            if (TTX_SRC_isDisplayablePage( &(nowPage.pageAddr)))
            {
                tens         = ((X0_header0&0xF0)>>4);
                units        = ( X0_header0&0x0F    );
                TTX_SRC_SetPageMap(magNum, tens, units);

                if (nowPage.pageAddr.subcodeA != 0 )   // sub page meet!
                {
//						TTX_Log(LOGGER_VERBOSE, "S:%x !\n", (uint32_t )nowPage.pageAddr.subcodeA );

                    if ((ttxBuf.pageList.pageToShow.pageA == nowPage.pageAddr.pageA))
                    {
                        //TTX_Log(LOGGER_VERBOSE, "nav! %x\n", (uint32_t)nowPage.pageAddr.subcodeA);

                        TTX_EVT_Fire( DISP, UPDATE_NAV_CTRL ); // Force to redraw screen
                        TTX_BUF_ISRSetSubPageMap(nowPage.pageAddr.subcodeA);
                    }
                }
            }

//			nowPage.pageAddr.subCode.s4           = (*((UINT8 xdata*)&X0_header1)&0x30)>>4;
//			nowPage.pageAddr.subCode.s3           = (*((UINT8 xdata*)&X0_header1)&0xf);
//			nowPage.pageAddr.subCode.s2           = (*((UINT8 xdata*)&X0_header1 + 1)&0x70)>>4; //((X0_header1&0x00700000)>>20);
//			nowPage.pageAddr.subCode.s1           = (*((UINT8 xdata*)&X0_header1 + 1)&0xf); //((X0_header1&0x000F0000)>>16);

            // if X0_header0 = 0x12345678, the bitwise order will be 0x78563412
            nowPage.ctrlBits.bErasePage = tmpBit14;
            nowPage.ctrlBits.bNewsFlash = tmpBit13; //          = ((X0_header1>>13)&0x1); /* C5  */
            nowPage.ctrlBits.bSubtitle   = tmpBit12; //        = ((X0_header1>>12)&0x1); /* C6  */
            // nowPage.ctrlBits.bSuppressHeader      = tmpBit11; //((X0_header1>>11)&0x1); /* C7  */
            nowPage.ctrlBits.bUpdate              = tmpBit10;//((X0_header1>>10)&0x1); /* C8  */
            nowPage.ctrlBits.bInterruptedSequence = tmpBit9; //((X0_header1>> 9)&0x1); /* C9  */
            nowPage.ctrlBits.bSerialMode = tmpBit3; //((X0_header1>> 3)&0x1); /* C11  */

            if ( (magNumPrev==0x03) && (pageNumPrev==0x0000) )   // dirty patch for page P300, it should be removed after we fix PFE hardware bug
            {
                if ( mag_serial )   // previous page is serial mode
                {
                    if ( TTX_SRC_GetCollectingPageByMagazine( magNumPrev, &prePage ) )
                    {
                        TTX_SRC_onStoredPage( &prePage );
                        collectingPages[magNumPrev-1].counter = 0;
                    }
                }
            }

            if ( TTX_SRC_GetCollectingPageByMagazine( magNum, &prePage ) )
            {
                if ((nowPage.pageAddr.pageA!=prePage.pageAddr.pageA)||(nowPage.pageAddr.subcodeA != prePage.pageAddr.subcodeA))
                {
                    // if the same magazine and not the same page,
                    // This X/0 terminates previously received page
                    // Finish prePage
                    TTX_SRC_onStoredPage( &prePage );
                    collectingPages[magNum-1].counter = 0;
                }
                else  					// the same mag & the same page again,
                {
                    // USER:Ben_Wang DATE:2010/3/24
                    // Special case for chroma pattern, there's only one page;
                    // If we don't force to storege it, it won't display normal.
                    collectingPages[ magNum -1 ].counter++;
                    //after 2 times, set stored anyway, for
                    // i. P300 can't display bug, (magazine 3 only have one page 300 in fluke)
                    // ii. Chroma pattern 1, pattern 3, pattern 5
                    if (collectingPages[ magNum -1 ].counter > 2)
                    {
                        TTX_SRC_onStoredPage( &prePage );
                        collectingPages[ magNum -1 ].counter=0;
                        goto no_collect;  // don't receive this time's page data, for decoder could start decode
                    }
                }
                /* Allocate memory from page buffer to collect incoming page: nowPageAddr. */
            }

            // try to receive new incoming page
            TTX_SRC_AllocateAndStartCollectingPage( &nowPage );
            magNumPrev = magNum; // record the collecting mag number
            pageNumPrev = nowPage.pageAddr.pageA & 0xff; // record the collecting page number
            subPagePrev = nowPage.pageAddr.subcodeA; // record the collecting subpage number
            mag_serial = nowPage.ctrlBits.bSerialMode; // record magazine serial mode

            // record subtitle page in subtitle mode
            if ( m_hTtx->infoSubtitle.bIntendToFind )
            {
                if ( nowPage.ctrlBits.bSubtitle )
                {
                    if ( TTX_SRC_isDisplayablePage( &(nowPage.pageAddr) ) )
                    {
                        m_hTtx->infoSubtitle.addr = nowPage.pageAddr;
                        m_hTtx->infoSubtitle.bFound = TRUE;
                    }
                }
            }

#ifdef CONFIG_TTX_PFE_BUG
            currPFEStatus = ttxBuf.pageList.elements[nowPage.buf_index].X27_status;
#endif

no_collect:
            // clear the X0 bit we handled this time
            pkt_x0_ind &= ~(UINT32_C(1) << packet_index);
            rtd_isr_outl(VBIPPR_FB_IND_REG_0x40b0, pkt_x0_ind );
            last_mag = magNum;

            // reset fetched line to zero when it meets X/0
            fetched_line = 0;
        }   // end of if( pkt_x0_ind & (UINT32_C(1) << packet_index) )

#ifdef CONFIG_TTX_PFE_BUG
        // enable DMA
        if ( currPFEStatus == TTX_X27_COLLECTING )
        {

            if ( magNumPrev <= 8 )
            {

#if SPARROW_LEVEL_15_MEMORY_LAYOUT
                // collect PFE Mode 1 (X/26/0~X/26/14, X/27/0, and X/28/0~X/28/1)
                TTX_SRC_Fetch_PfeMode1(&magNumPrev, &pfe_nproc_addr_orig, &tmp_fetched_line);
#else
                // collect X/27/0
                TTX_SRC_Fetch_X_27_0(&magNumPrev, &pfe_nproc_addr_orig, &tmp_fetched_line);
#endif
            }
        }
        else
        {

            PPR_PfeMove(PPR_PFE_MODE0, &tmp_fetched_line); // Start prefetch moving engine mode 0
        }
        fetched_line += tmp_fetched_line;
#else
        // enable DMA
        PPR_PfeMove(PPR_PFE_MODE0, &tmp_fetched_line); // Start prefetch moving engine mode 0
        fetched_line += tmp_fetched_line;
        if (	(magNumPrev<=8) &&
                (ttxBuf.pageList.elements[collectingPages[magNumPrev-1].buf_index].bstatus == TTX_BUFFER_COLLECTING) &&
                (rtd_isr_inl(VBIPPR_PFE_CTRL_REG_0x40e4)&_BIT5) )
        {
#if SPARROW_LEVEL_15_MEMORY_LAYOUT
            // collect PFE Mode 1 (X/26/0~X/26/14, X/27/0, and X/28/0~X/28/1)
            //TT_Log(LOGGER_INFO, "TTX_SRC_Fetch_X_27_0 \n");
            TTX_SRC_Fetch_PfeMode1(&magNumPrev, &pfe_nproc_addr_orig, NULL);
#else
        // collect X/27/0
        TTX_SRC_Fetch_X_27_0(&magNumPrev, &pfe_nproc_addr_orig, NULL);
#endif
        }
#endif

    }
    while ( pkt_x0_ind );

    RtdCtrl_isr_Mask(TCWTR_0x2874, 0xffffffff, (UINT32)_BIT0); //Reset watch dog

#if 0 // debug
    rtd_outl(VBIPPR_LVLD_BTM_REG_0x42e4, 0x0150013d);
    rtd_outl(VBIPPR_LVLD_TOP_REG_0x42e0, 0x00160005);
#endif

    // Clear Data Valid
    rtd_isr_outl( VBIPPR_DATA_ACQU_ST_REG_0x4014, 0x3 );

    // Clear PPR INT status
    rtd_isr_outl( VBIPPR_INT_ST_REG_0x4008, 0xffffffff );

    // Reset prefetch start address of field buffer
    rtd_isr_outl( VBIPPR_PFE_NPROC_ADDR_REG_0x40e0, 0);

    // Accept data from Slicer
    rtd_isr_outl( VBIPPR_CTRL_REG_0x4000, 0x1);

    // ERROR: Our software cannot process field buffer before next coming of field buffer.
    //        In other words, our software didn't meet real-time requirements.
    //        This will cause frame loss.

    if (rtd_isr_inl(VBIPPR_DATA_ACQU_ST_REG_0x4014) & 0x2)
    {

//->not used in isr	TTX_Log(LOGGER_VERBOSE, "FB_FULL!!!\n");
    }

//	myTimer2 = rtd_isr_inl( 0x2854 );
//	TTX_Log(LOGGER_VERBOSE, "t=%x %x\n", myTimer2-myTimer1, X0_header0);

}


/* -- Implementations of Public Functions -----------------------------------*/




#if 0
/*---------------------------------------------------------------------------*/
static bool_t
TTX_SRC_onReplacingPage   (TTX_Handle                            m_hTtx,
                           const TTX_STD_Page_AddrWithSubCode_t* pOldPage,
                           const TTX_STD_Page_AddrWithSubCode_t* pNewPage)
/*****************************************************************************\
 *  @brief   The broadcaster is sending *pPage. But owing to limited size of
 *             page-buffer, we decide to store *pNewPage by replacing the
 *             existing *pOldPage.
 *  @details *pOldPage shall be already in page-buffer.
 *           *pNewPage shall not be in page-buffer.
 *  @ret     This event has been handled or not.
\*****************************************************************************/
{
    if ( ( pOldPage != NULL )&&( pNewPage != NULL ) )
    {
        if ( m_hTtx->state == TTX_STATE_NORMAL )  /* SubPage nav control is only renderned in TTX_STATE_NORMAL */
        {
            /* If *pOldPage is a subpage of showing page */
            if ( TTX_STD_PageAddrWithSubCodeAreEqual( &(m_hTtx->pageShowing.addr), pOldPage, FALSE ) )
            {
                TTX_EVT_Fire( m_hTtx, DISP, UPDATE_NAV_CTRL );
            }
        }

        return( TRUE );
    }
    return( FALSE );
}
#endif

void TT_Log(_LOGGER_LEVEL level, char code *format, ...)
{
    int xdata  i;
    UINT8  xdata getVal;
    bit fFirstNotZero;
    long xdata argint = 0;
    long xdata  base;
    va_list xdata  arglist;

    va_start(arglist,format);

    while (*format)
    {
        if (*format == '%')
        {
            switch  (*(format+1))
            {
            case 'd':
                argint = va_arg(arglist,long);
                if (argint == 0)
                {
                    drv_isr_uart_Write(UART_MONITOR_SOURCE, '0');
                }
                else
                {
                    if (argint < 0)
                    {
                        drv_isr_uart_Write(UART_MONITOR_SOURCE, '-');
                        argint = -argint;
                    }
                    base = 1;
                    i = 0;
                    while (argint / base)
                    {
                        base *= 10;
                        i++;
                    }
                    base /= 10;
                    while (i > 0)
                    {
                        getVal = argint / base;
                        argint = argint - (base * getVal);
                        getVal += '0';
                        base /= 10;
                        drv_isr_uart_Write(UART_MONITOR_SOURCE, getVal);
                        i--;
                    }
                }
                break;

            case 'X':	// <Janus>2009/08/18: Add support to "%X"
            case 'x':
                argint = va_arg(arglist, long);
                drv_isr_uart_Write(UART_MONITOR_SOURCE, '0');
                drv_isr_uart_Write(UART_MONITOR_SOURCE, 'x');
                if (argint == 0)
                {
                    drv_isr_uart_Write(UART_MONITOR_SOURCE, '0');
                }
                else
                {
                    i = 8;
                    fFirstNotZero = 0;
                    while (i > 0)
                    {
                        if (!fFirstNotZero)
                        {
                            getVal = ((argint & ((UINT32)0x0f) << ((i - 1) << 2)) >> ((i - 1) << 2)) & 0x0f;
                            if (getVal != 0)
                                fFirstNotZero = 1;
                        }

                        if (fFirstNotZero)
                        {
                            getVal = ((argint & ((UINT32)0x0f) << ((i - 1) << 2)) >> ((i - 1) << 2)) & 0x0f;
                            if (getVal > 9)
                                getVal = (getVal - 0x0a) + 'a';
                            else
                                getVal += '0';

                            drv_isr_uart_Write(UART_MONITOR_SOURCE, getVal);
                        }
                        i--;

                    }
                }
                break;
            case 's':	// <Janus>2009/08/06: Add support to "%s"
            {
                char* szStr = va_arg( arglist, char* );
                while ( *szStr != '\0' )
                {
                    drv_isr_uart_Write(UART_MONITOR_SOURCE, *szStr);
                    szStr++;
                }
                break;
            }
            }
            format+=2;
        }
        if (*format == 0x0a)
        {
            drv_isr_uart_Write(UART_MONITOR_SOURCE, 0x0a);
            drv_isr_uart_Write(UART_MONITOR_SOURCE, 0x0d);
            format++;
        }
        else
        {
            if (*format != 0)
                drv_isr_uart_Write(UART_MONITOR_SOURCE, *format);
            format++;
        }
    }
}




/*---------------------------------------------------------------------------*\
 * end of ttx_source.c                                                       *
\*---------------------------------------------------------------------------*/
#endif
