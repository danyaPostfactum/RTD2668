/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_buff.h
 * @brief    Teletext page-buffer management.
 * @author   Janus Cheng
 * @date     Jan. 12, 2009
 * @note     There's one and only one page of "cd mem" which stores the output of TTX_BUF_DecodePage().
 * @version  0.1
 * @ingroup  osd
\*****************************************************************************/

#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_BUFFER_MANAGEMENT_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_BUFFER_MANAGEMENT_HEADER_INCLUDED__ (1)

/* -- TTX headers -----------------------------------------------------------*/
#include <ttx_pkg.h>    /* Provides: uint32_t */
#include <ttx_std.h>    /* Provides: TTX_STD_Page_AddrWithSubCode_t */
#include <ttx.h>        /* Provides: TTX_DEFAULT_CD_PAGE_LAST_ADDR */
#include <ttx_disp.h>   /* Provides: TTX_DISP_PageNavCtrl_t */

#include <stdlib.h>     /* Provides: size_t */


/* -- BUF constants ---------------------------------------------------------*/



#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/
#define                 TTX_BUF_isFull()                  (TTX_BUF_GetFreeSpace()==0)


    /* -- Global typedef Declarations -------------------------------------------*/

    typedef enum
    {
        TTX_BUFFER_IDLE=0,
        TTX_BUFFER_COLLECTING,
        TTX_BUFFER_COLLECTED,
        TTX_BUFFER_ONDECODE,
        TTX_BUFFER_DISPLAYED,
//	TTX_BUFFER_REPLACING,
    } TTX_BUFFER_STATUS_t;


    /* -- Function Prototypes ---------------------------------------------------*/
    void                    TTX_BUF_Open                        (void);   // initialize all space to freely available
    void                    TTX_BUF_Close                       (void);   // reinitialize all space to freely available

    void                    TTX_BUF_Reset                       (void);   // reinitialize all space to freely available

    /* -- Functions will be called in ISR */
    bool_t
    TTX_BUF_ISRFindPage (const TTX_STD_Page_AddrWithSubCode_t * pPageAddr,
                         uint8_t * page_index);

    void TTX_BUF_ISRSetSubPageMap(uint16_t subCode);

// Since rule of page memory mangement is based on currently displaying page, so let TTX_BUF now which page is currently displaying is very important.
    void                    TTX_BUF_SetPageToShow               (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);
    void                    TTX_BUF_SetShowingPage              (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);

// Page Memory Management
    size_t                  TTX_BUF_GetFreeSpace                (void);   // get number of pages free
    bool_t                  TTX_BUF_isEmpty                     (void);
    bool_t                  TTX_BUF_InsPage                     (const TTX_STD_Page_AddrWithSubCode_t* pPageToIns, void TTX_MEM** pAllocatedMemAddr, uint8_t* pbRemoveOccurs, TTX_STD_Page_AddrWithSubCode_t* pRemovedPage);  // get the space for a new page. May delete the most far page address if buffer is full.
    void                    TTX_BUF_DelPage                     (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);    // remove specified page from buffer
    bool_t                  TTX_BUF_FindPage                    (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr, uint8_t * buff_idx, bool_t lock);  // find a page in buffer and returns its buf index
    void TTX_BUF_SetBufStatus(uint8_t idx, uint8_t status);
    void TTX_MEM* TTX_BUF_GetMemByIdx(uint8_t idx);

    bool_t                  TTX_BUF_FindNextDisplayableDifferentMainPage     (                                                    void TTX_MEM** pSaveAddr, TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound);
    void 			TTX_BUF_SetBufStatus(uint8_t idx, uint8_t status);
    bool_t                  TTX_BUF_FindPrevDisplayableDifferentMainPage     (                                                    void TTX_MEM** pSaveAddr, TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound);
    bool_t                  TTX_BUF_ZeroPage                    (void TTX_MEM* pSaveAddr);  /* Erase content of a whole page */
    bool_t                  TTX_BUF_FindNextSubPage             (uint8_t start, TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound);
    bool_t                  TTX_BUF_FindPrevSubPage             (TTX_STD_Page_AddrWithSubCode_t* pPageAddrFound);


    bool_t TTX_BUF_GetSubPageNavCtrlInfo(int* cursor_Index, TTX_DISP_SubPageNavCtrl_t*     pPageNavInfo);

    void TTX_BUF_ResetSubPageList();
    uint8_t TTX_BUF_Get_First_Subcode();
    uint8_t TTX_BUF_Get_Last_Subcode();
    void TTX_BUF_Sync_Cursor_Page(int cursor_idx);
    uint16_t TTX_BUF_Get_Index_By_Subcode(uint16_t subcode);



// Mark *pPageAddr can be removed or not when TTX_BUF_InsPage().
    void                  TTX_BUF_KeepPage                    (const TTX_STD_Page_AddrWithSubCode_t* pPageAddr);

// Channel Decode
    bool_t TTX_BUF_DecodeNormalPage (uint8_t src); // Overwrite the "cd mem" with channel decoded *src.
//bool_t TTX_BUF_DecodeX0 (uint8_t src); // Overwrite the X/0 of "cd mem" with channel decoded *src.
    bool_t TTX_BUF_DecodeX24 (uint8_t src); // Overwrite the X/24 of "cd mem" with channel decoded *src.
    bool_t TTX_BUF_DecodeAitPage (uint8_t  src); // Overwrite the "cd mem" with channel decoded *src.
    bool_t TTX_BUF_DecodeMpePage (uint8_t  src); // Overwrite the "cd mem" with channel decoded *src.
    uint8_t TTX_MEM* TTX_BUF_GetDecodedPacket (const TTX_STD_Packet_Num_t pktNum); // After calling TTX_BUF_DecodeXxxPage() once, and call this function multiple times to obtain various contents of decoded packets with pktNum.

    bool_t                  TTX_BUF_DecodeBttPage               ( uint8_t src);   // Overwrite the TTX_TOP_BTT_PAGE_ADDRESS with channel decoded *src
    uint8_t TTX_MEM*        TTX_BUF_GetDecodedBttPage           (void);


    bool_t                  TTX_BUF_Test                        (void);
    void                  TTX_BUF_Dump                        (void);

#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_BUFFER_MANAGEMENT_HEADER_INCLUDED__ */


/*---------------------------------------------------------------------------*\
 * End of ttx_buff.h                                                         *
\*---------------------------------------------------------------------------*/
