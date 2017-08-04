/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_buff_pkg.h
 * @brief    Internal (package scope) Teletext Buffer Management interface.
 * @details  This header declares the platform-dependent interface for this ttx library.
 * @author   Janus Cheng
 * @date     Jan. 13, 2009
\*****************************************************************************/

/**
 * @addtogroup ttx
 * @{
 */
#ifndef __ENHANCED_TELETEXT_DECODER_LIBRARY_PAGE_BUFFER_MANAGER_INTERNAL_PKG_HEADER_INCLUDED__
#define __ENHANCED_TELETEXT_DECODER_LIBRARY_PAGE_BUFFER_MANAGER_INTERNAL_PKG_HEADER_INCLUDED__ (1)

#include <ttx_std.h>    /* Provides: TTX_STD_PRESENT_LEVEL() */
#include <ttx_assert.h> /* Provides: TTX_ASSERT_COMPILE() */

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __KEIL__  /* KeilC 8051 compiler */
#define TTX_BUF_SOFTWARE_EMULATOR           (0)     // Define (1) to use allocated memory for TTX; (0) to use hardware fixed-location memory addressing for TTX.
#else
#define TTX_BUF_SOFTWARE_EMULATOR           (1)     // Define (1) to use allocated memory for TTX; (0) to use hardware fixed-location memory addressing for TTX.
#endif

#define BUFFER_TOP_PAGE (1)

#ifdef BUFFER_TOP_PAGE
#define BUFFER_MAX_AIT_NUM	(2)
#endif

#define SPARROW_LEVEL_15_MEMORY_LAYOUT (1) // 1: level 1.5 memory layout, 0: level 1.0 memory layout

#ifdef __KEIL__ /* KeilC 8051 compiler */
#if SPARROW_LEVEL_15_MEMORY_LAYOUT
#define TTX_MAX_PAGE_NUM                      (6)        /* total number of pages can be stored in page queue */
    // here i modify the page keep num as 3
    // due to the total page (6) - BTT+AIT (2) = 4
    // we need reserve one page for replace buffer at least
    // USER:BEN_WANG DATE:2010/3/19
#define TTX_MAX_PAGE_KEEP_NUM                  (3)        /* max number of pages can be kept */
#else
#define TTX_MAX_PAGE_NUM                      (10)        /* total number of pages can be stored in page queue */
#define TTX_MAX_PAGE_KEEP_NUM                  (7)        /* max number of pages can be kept */
#endif
#else
#define TTX_MAX_PAGE_NUM                    (1000)        /* total number of pages can be stored in page queue */
#define TTX_MAX_PAGE_KEEP_NUM                (7)        /* max number of pages can be kept */
#endif  /* end of #ifdef __KEIL__ */


    /* Page storage buffer address */
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
#define TTX_DEFAULT_PAGE_SIZE               (55*40)     // bytes
#else
#if SPARROW_LEVEL_15_MEMORY_LAYOUT
#define TTX_DEFAULT_PAGE_SIZE               (44*40)     // bytes
#else
#define TTX_DEFAULT_PAGE_SIZE               (26*40)     // bytes
#endif
#endif  /* end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25) */

#if TTX_BUF_SOFTWARE_EMULATOR
    extern void*                                TTX_gpPageMemAligned;
#define TTX_PAGE_MEM_START_ADDRESS          ((TTX_Address_t)TTX_gpPageMemAligned)    // To allow VTE_Decode() to work on this memory region, it must be 4 and 40 aligned.
#else
#define TTX_PAGE_MEM_START_ADDRESS          (0x8020)    // To allow VTE_Decode() to work on this memory region, it must be 4 and 40 aligned.
#endif  /* end of #if TTX_BUF_SOFTWARE_EMULATOR */

    /* TTX auto-computed macros */

    /* Channel decoder related information */
#define TTX_DEFAULT_CD_PAGE_ADDRESS         TTX_PAGE_MEM_START_ADDRESS
//#define TTX_DEFAULT_CD_PAGE_X0_25_SIZE      TTX_DEFAULT_PAGE_SIZE
#if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25)
#define TTX_DEFAULT_CD_PAGE_TOTAL_SIZE      (55*40)
#else
    //#define TTX_DEFAULT_CD_PAGE_TOTAL_SIZE      TTX_DEFAULT_CD_PAGE_X0_25_SIZE
#define TTX_DEFAULT_CD_PAGE_TOTAL_SIZE      TTX_DEFAULT_PAGE_SIZE
#endif  /* end of #if TTX_CFG_PRESENT_LEVEL >= TTX_STD_PRESENT_LEVEL(25) */
#define TTX_DEFAULT_CD_PAGE_LAST_ADDR       (TTX_DEFAULT_CD_PAGE_ADDRESS+TTX_DEFAULT_CD_PAGE_TOTAL_SIZE)

    /* Page storage buffer address */
#define TTX_DEFAULT_PAGE_ADDRESS            TTX_DEFAULT_CD_PAGE_LAST_ADDR
#define TTX_DEFAULT_PAGEx_ADDRESS(pageX)    (TTX_DEFAULT_PAGE_ADDRESS+pageX*TTX_DEFAULT_PAGE_SIZE)  // 0 <= pageX < TTX_MAX_PAGE_NUM
#define TTX_DEFAULT_PAGE_LAST_ADDRESS       (TTX_DEFAULT_PAGEx_ADDRESS(TTX_MAX_PAGE_NUM))

#if SPARROW_LEVEL_15_MEMORY_LAYOUT
// use page buffer
#else
    /* Mode 1 buffer for FLOF */
#define TTX_FLOF_PFE_MODE1_PAGE_ADDRESS	TTX_DEFAULT_PAGE_LAST_ADDRESS
#define TTX_FLOF_PFE_MODE1_SIZE			(18*40)
#define TTX_FLOF_PFE_MODE1_LAST_ADDRESS	(TTX_FLOF_PFE_MODE1_PAGE_ADDRESS+TTX_FLOF_PFE_MODE1_SIZE)
#endif

    /* TOP storage buffer address */
#if SPARROW_LEVEL_15_MEMORY_LAYOUT
#define TTX_TOP_BTT_PAGE_ADDRESS            TTX_DEFAULT_PAGE_LAST_ADDRESS
#else
#define TTX_TOP_BTT_PAGE_ADDRESS            TTX_FLOF_PFE_MODE1_LAST_ADDRESS
#endif
#define TTX_TOP_BTT_SIZE                    TTX_STD_TOP_BASIC_TOP_PAGE_SIZE
#define TTX_TOP_BTT_LAST_ADDRESS            (TTX_TOP_BTT_PAGE_ADDRESS+TTX_TOP_BTT_SIZE)

#if !TTX_BUF_SOFTWARE_EMULATOR
    TTX_ASSERT_COMPILE( TTX_PAGE_MEM_START_ADDRESS      >= 0x8000 );    // ERROR: Sparrow allocates page mem starting from 0x8000.
    TTX_ASSERT_COMPILE( TTX_PAGE_MEM_START_ADDRESS %  4 == 0 );         // ERROR: TTX_PAGE_MEM_START_ADDRESS is mis-aligned to work in VTE_Decode().
    TTX_ASSERT_COMPILE( TTX_PAGE_MEM_START_ADDRESS % 40 == 0 );         // ERROR: TTX_PAGE_MEM_START_ADDRESS is mis-aligned to work in VTE_Decode().
#endif  /* end of #if !TTX_BUF_SOFTWARE_EMULATOR */

#define TTX_DEFAULT_PAGE_LAST_ADDR          TTX_TOP_BTT_LAST_ADDRESS




    /* @brief   Use TTX_BUF_PageListIdx_t to index TTX_BUF_PageList_t::elements[].
     * @details To save memory space, use TTX_BUF_PageListIdx_t as small as possible.
     */
#if   TTX_MAX_PAGE_NUM <= UINT_LEAST8_MAX
    typedef uint_least8_t  TTX_BUF_PageListIdx_t;
#elif TTX_MAX_PAGE_NUM <= UINT_LEAST16_MAX
    typedef uint_least16_t TTX_BUF_PageListIdx_t;
#elif TTX_MAX_PAGE_NUM <= UINT_LEAST32_MAX
    typedef uint_least32_t TTX_BUF_PageListIdx_t;
#else
#error "TTX_MAX_PAGE_NUM is too large to find a suitable size for TTX_BUF_PageListIdx_t"
#endif

    /* @brief   Use TTX_BUF_KeepQueIdx_t to index TTX_BUF_KeepQue_t::elements[].
     * @details To save memory space, use TTX_BUF_KeepQueIdx_t as small as possible.
     */
#if   TTX_MAX_PAGE_KEEP_NUM <= UINT_LEAST8_MAX
    typedef uint_least8_t  TTX_BUF_KeepQueIdx_t;
#elif TTX_MAX_PAGE_KEEP_NUM <= UINT_LEAST16_MAX
    typedef uint_least16_t TTX_BUF_KeepQueIdx_t;
#elif TTX_MAX_PAGE_KEEP_NUM <= UINT_LEAST32_MAX
    typedef uint_least32_t TTX_BUF_KeepQueIdx_t;
#else
#error "TTX_MAX_PAGE_KEEP_NUM is too large to find a suitable size for TTX_BUF_KeepQueIdx_t"
#endif


    typedef enum
    {
        TTX_X27_EMPTY = 0,
        TTX_X27_COLLECTING,
        TTX_X27_COLLECTED,
    } TTX_X27_STATUS_t;


    typedef struct TTX_BUF_PageListElement_t
    {
        /* Implements the double-linked-list */
        uint16_t  pageA;
        uint16_t  subcodeA;
        uint8_t 	keep;
        TTX_BUFFER_STATUS_t 	bstatus;
        TTX_X27_STATUS_t X27_status;
        uint8_t collecting_count;
        void TTX_MEM*                       addrMem;
    } TTX_BUF_PageListElement_t;

    typedef struct TTX_BUF_PageListPageInfo_t
    {
        uint16_t  pageA;
        uint16_t  subcodeA;
        TTX_BUF_PageListIdx_t           idx;             /* idx is valid if( bIsBuffered == TRUE )*/
        uint8_t                         bIsBuffered;   /* TRUE if pageAddr has been stored in pageList */
    } TTX_BUF_PageListPageInfo_t;

    typedef struct TTX_BUF_PageList_t
    {
        TTX_BUF_PageListPageInfo_t           pageToShow;
        TTX_BUF_PageListPageInfo_t           pageShowing;    /* this is the base of TTX_BUF_FindNextDisplayableDifferentMainPage(), TTX_BUF_FindPrevDisplayableDifferentMainPage(), TTX_BUF_FindNextSubPage() and TTX_BUF_FindPrevSubPage(). */
        TTX_BUF_PageListElement_t            elements[TTX_MAX_PAGE_NUM]; /* use TTX_BUF_PageListIdx_t to index elements[] */
        uint8_t subPageIndicator[10];
    } TTX_BUF_PageList_t;

    typedef struct TTX_BUF_KeepQueElement_t
    {
        uint16_t  pageA;
        uint16_t  subcodeA;
        uint8_t  bindex;
    } TTX_BUF_KeepQueElement_t ;

    typedef struct TTX_BUF_KeepQue_t
    {
        TTX_BUF_KeepQueElement_t            elements[TTX_MAX_PAGE_KEEP_NUM];
        TTX_BUF_KeepQueIdx_t                cnt;
    } TTX_BUF_KeepQue_t;

#ifdef BUFFER_TOP_PAGE
    typedef struct TTX_BUF_KeepPage_t
    {
        uint16_t  pageA;
        uint16_t  subcodeA;
    } TTX_BUF_KeepPage_t;
#endif

    /* private object, not to be used by application code */
    typedef struct
    {
        TTX_BUF_PageList_t                  pageList;    /* actually not a queue, but a sorted double-linked list */
        TTX_BUF_KeepQue_t                 keepQue;
#ifdef BUFFER_TOP_PAGE
        TTX_BUF_KeepPage_t		  BttPage;
        TTX_BUF_KeepPage_t		  Ait_Mpe_Page[BUFFER_MAX_AIT_NUM];
#endif
#if TTX_BUF_SOFTWARE_EMULATOR
        size_t                              cntUsage;
        void TTX_MEM*                       pAddrBase;
#endif  /* end of #if TTX_BUF_SOFTWARE_EMULATOR */
    } TTX_BUF_Obj;

    extern TTX_BUF_Obj ttxBuf;



    /* -- Global Macro Definitions ----------------------------------------------*/

    /* -- Function Prototypes ---------------------------------------------------*/
    uint8_t               TTX_BUF_PageListGetIdx                ( const TTX_STD_Page_AddrWithSubCode_t* pPageAddr, TTX_BUF_PageListIdx_t* pIdxFound);
    uint8_t               TTX_BUF_PageListDelPage               (       const TTX_BUF_PageListIdx_t idxToBeDel) TTX_REENTRANT;
    uint8_t               TTX_BUF_GetNextDiffMainPageIdx        ( const TTX_BUF_PageListIdx_t idxStart  , const TTX_BUF_PageListIdx_t* pIdxStop, TTX_BUF_PageListIdx_t* pIdxFound) TTX_REENTRANT;
    uint8_t               TTX_BUF_PageListGetPrevAndNextIdx     ( const TTX_STD_Page_AddrWithSubCode_t* pPageToFind, TTX_BUF_PageListIdx_t* pIdxPrev, TTX_BUF_PageListIdx_t* pIdxNext, const TTX_BUF_PageListIdx_t* pIdxStart) TTX_REENTRANT;
    uint8_t               TTX_BUF_GetPrevMostFarSubPageIdx      ( const TTX_BUF_PageListIdx_t idxStart, const TTX_BUF_PageListIdx_t* pIdxStop, TTX_BUF_PageListIdx_t* pIdxFound, TTX_BUF_PageListIdx_t* pNumOfSubPages) TTX_REENTRANT;
    TTX_BUF_PageListIdx_t TTX_BUF_GetNumOfDiffMainPages        () TTX_REENTRANT;
    uint8_t               TTX_BUF_GetNextMostFarSubPageIdx      ( const TTX_BUF_PageListIdx_t idxStart, const TTX_BUF_PageListIdx_t* pIdxStop, TTX_BUF_PageListIdx_t* pIdxFound, TTX_BUF_PageListIdx_t* pNumOfSubPages) TTX_REENTRANT;



#ifdef __cplusplus
}   /* extern "C" */
#endif



#endif /* !__ENHANCED_TELETEXT_DECODER_LIBRARY_PAGE_BUFFER_MANAGER_INTERNAL_PKG_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of ttx_buff_pkg.h                                                     *
\*---------------------------------------------------------------------------*/

