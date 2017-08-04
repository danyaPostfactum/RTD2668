/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_nav.c
 * @brief    Teletext Table of Pages software decoder routines.
 * @author   Janus Cheng
 * @date     Jan. 23, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_nav.h>        /* Provides functions to be implemented */
#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */
#include <ttx_buff.h>       /* Provides: TTX_BUF_FindPage() */
#include <ttx_log.h>        /* Provides: TTX_Log(LOGGER_VERBOSE, ) */

TTX_DEFINE_THIS_MODULE( ttx_nav.c )

/** Returns the number of entries of a C array. */
//#define ENTRIES(carray) (sizeof(carray)/sizeof(carray[0]))


/* -- Prototypes of Private Functions ---------------------------------------*/

/* -- Implementations of Public Functions -----------------------------------*/



/*---------------------------------------------------------------------------*/
bool_t TTX_NAV_GetNextPage ( TTX_STD_Page_AddrWithSubCode_t* pNextPage)
/*****************************************************************************\
 *  @brief   Get the next page of m_hTtx->pageShowing.
 *  @note    Must run this function BEFORE calling any other TTX_TOP_xxxx()
 *             functions.
\*****************************************************************************/
{
    uint16_t page_num;
    uint8_t index, offset;
    int8_t i, j, start, end, found=0;

    page_num = TTX_STD_PageAddrHexToInt(m_hTtx->pageShowing.addr.pageA);

    if (page_num<799)
        page_num +=1;
    else
        page_num = 0;

    index = page_num/8;
    offset =  page_num%8;

    TTX_Log(LOGGER_VERBOSE, "n1 %x\n",(uint32_t) page_num);

    for (i=index;  i<100; i++)
    {
        if (m_hTtx->pageMap[i]==0)
            continue;

        if (i==index)
        {
//			TTX_Log(LOGGER_VERBOSE, "map %x\n",(uint32_t)m_hTtx->pageMap[i]);
            start = offset;
        }
        else
            start = 0;

        for (j=start; j<8; j++)
        {
            if (m_hTtx->pageMap[i] & (0x1<<j))
            {
                found = 1;
                TTX_Log(LOGGER_VERBOSE, "f %x %x\n",(uint32_t) i, (uint32_t) j);
                break;
            }
        }
        if (found)
            break;
    }

    if (!found)   // search from head
    {

        TTX_Log(LOGGER_VERBOSE, "nf\n");

        for (i=0;  i<=index; i++)
        {
            if (m_hTtx->pageMap[i]==0)
                continue;

            if (i==index)
                end = offset;
            else
                end = 8;

            for (j=0; j<end; j++)
            {
                if (m_hTtx->pageMap[i] & (0x1<<j))
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
        pNextPage->pageA = TTX_STD_PageAddrIntToHex(100+ i*8 + j);
        pNextPage->subcodeA = 0;
        TTX_Log(LOGGER_VERBOSE, "next %x\n",(uint32_t) pNextPage->pageA);
#ifdef	CONFIG_FAST_PAGENUM_UPDATE
		{
			TTX_CmdArg_t arg;

			arg.pageNum.magNum = (pNextPage->pageA)>>8;
			arg.pageNum.pageNumTens = (((pNextPage->pageA) &0xF0)>>4);
			arg.pageNum.pageNumUnits = ( (pNextPage->pageA) &0x0F );

//			TTX_Log(LOGGER_VERBOSE, "set mag %x, page ten %x, page uints %x\n",(uint32_t) arg.pageNum.magNum, (uint32_t) arg.pageNum.pageNumTens
//				, (uint32_t) arg.pageNum.pageNumUnits);

			TTX_Control1(TTX_CMD_SET_PAGE_NUM_INDICATOR, &arg);
		}
#endif
        return( TRUE );
    }
    TTX_Log(LOGGER_VERBOSE, "not found\n");

    return( FALSE);


#if 0
    if ( m_hTtx != NULL )
    {
        /*  <Janus>2009/07/30: To avoid missing pages, we currently ignore info provided by TOP.
                if( m_hTtx->pageShowing.navInfo.top.bAvailable )
                {
                    if( m_hTtx->pageShowing.navInfo.top.bNextPageReady )
                    {
                        if( pNextPage != NULL )
                        {
                            *pNextPage = m_hTtx->pageShowing.navInfo.top.flofKeys[TTX_FLOF_KEY_CYAN];
                        }
                        return( TRUE );
                    }
                }
                else
        */
        {
            TTX_STD_Page_AddrWithSubCode_t nextPage;
            if ( TTX_BUF_FindNextDisplayableDifferentMainPage( NULL, &nextPage ) )
            {
                if ( pNextPage != NULL )
                {
                    *pNextPage = nextPage;
                }
                return( TRUE );
            }
        }
    }
#endif
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_NAV_GetPrevPage (
    TTX_STD_Page_AddrWithSubCode_t* pPrevPage)
/*****************************************************************************\
 *  @brief   Get the next page of m_hTtx->pageShowing.
 *  @note    Must run this function BEFORE calling any other TTX_TOP_xxxx()
 *             functions.
\*****************************************************************************/
{
    uint16_t page_num;
    uint8_t index, offset;
    int8_t i, j, start, end, found=0;

    page_num = TTX_STD_PageAddrHexToInt(m_hTtx->pageShowing.addr.pageA);

    if (page_num>0)
        page_num -=1;
    else
        page_num = 799;

    TTX_Log(LOGGER_VERBOSE, "p1 %x\n",(uint32_t) page_num);

    index = page_num/8;
    offset =  page_num%8;

    for (i=index;  i>=0; i--)
    {
        if (m_hTtx->pageMap[i]==0)
            continue;

        if (i==index)
        {
            start = offset;
            TTX_Log(LOGGER_VERBOSE, "map %x\n",(uint32_t)m_hTtx->pageMap[i]);
        }
        else
            start = 7;

        for (j=start; j>=0; j--)
        {
            if (m_hTtx->pageMap[i] & (0x1<<j))
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
        for (i=99;  i>=index; i--)
        {
            if (m_hTtx->pageMap[i]==0)
                continue;

            if (i==index)
                end = offset;
            else
                end = 0;

            for (j=7; j>=end; j--)
            {
                if (m_hTtx->pageMap[i] & (0x1<<j))
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
        pPrevPage->pageA = TTX_STD_PageAddrIntToHex(100+ i*8 + j);
        pPrevPage->subcodeA = 0;
        TTX_Log(LOGGER_VERBOSE, "prev %x\n",(uint32_t) pPrevPage->pageA);

#ifdef	CONFIG_FAST_PAGENUM_UPDATE
		{
			TTX_CmdArg_t arg;

			arg.pageNum.magNum = (pPrevPage->pageA)>>8;
			arg.pageNum.pageNumTens = (((pPrevPage->pageA) &0xF0)>>4);
			arg.pageNum.pageNumUnits = ( (pPrevPage->pageA) &0x0F );

//			TTX_Log(LOGGER_VERBOSE, "set mag %x, page ten %x, page uints %x\n",(uint32_t) arg.pageNum.magNum, (uint32_t) arg.pageNum.pageNumTens
//				, (uint32_t) arg.pageNum.pageNumUnits);

			TTX_Control1(TTX_CMD_SET_PAGE_NUM_INDICATOR, &arg);
		}
#endif
        return( TRUE );
    }

    TTX_Log(LOGGER_VERBOSE, "not found\n");

    return( FALSE);

#if 0
    if ( m_hTtx != NULL )
    {
        /*  <Janus>2009/07/30: To avoid missing pages, we currently ignore info provided by TOP.
                if( m_hTtx->pageShowing.navInfo.top.bAvailable )
                {
                    if( m_hTtx->pageShowing.navInfo.top.bPrevPageReady )
                    {
                        if( pPrevPage != NULL )
                        {
                            *pPrevPage = m_hTtx->pageShowing.navInfo.top.flofKeys[TTX_FLOF_KEY_RED];
                        }
                        return( TRUE );
                    }
                }
                else
        */
        {
            TTX_STD_Page_AddrWithSubCode_t prevPage;
            if ( TTX_BUF_FindPrevDisplayableDifferentMainPage( NULL, &prevPage ) )
            {
                if ( pPrevPage != NULL )
                {
                    *pPrevPage = prevPage;
                }
                return( TRUE );
            }
        }
    }
    return( FALSE );
#endif

}

/*---------------------------------------------------------------------------*/
bool_t
TTX_NAV_GetNextSubPage  ( TTX_STD_Page_AddrWithSubCode_t* pNextPage)
/*****************************************************************************\
 *  @brief   Get the next adjacent sub page of m_hTtx->pageShowing.
 *  @param   [out] *pnextSubPage will be the next sub-page.
 *  @param   [in]  *pShowingPage is the current showing page.
 *  @param   [in]  *pSubPageInfo is the sub-page info of *pShowingPage.
 *  @ret     There's any next sub-page or not.
 *  @pre     TTX_BUF_SetShowingPage() is invoked before this function.
 *  @see     TTX_NAV_GetPrevSubPage().
\*****************************************************************************/
{
    TTX_STD_Page_AddrWithSubCode_t nextSubPage;
    uint16_t subCode = m_hTtx->pageShowing.addr.subcodeA;
    if (subCode==0x0)
    {
        return FALSE; // It won't have sub pages
    }

    //RTD_Log(LOGGER_INFO, "Before GetNextSubPage : subCode(%x), PageA (%x), SubCodeA (%x) \n", (UINT32) subCode, (UINT32)pNextPage->pageA, (UINT32)pNextPage->subcodeA);
    if ( TTX_BUF_FindNextSubPage(subCode & 0x7F,  &nextSubPage ) )
    {
        *pNextPage = nextSubPage;
        //RTD_Log(LOGGER_INFO, "GetNextSubPage : PageA (%x), SubCodeA (%x) \n", (UINT32)nextSubPage.pageA, (UINT32)nextSubPage.subcodeA);
        return( TRUE );
    }

    return( FALSE );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_NAV_GetPrevSubPage  ( TTX_STD_Page_AddrWithSubCode_t* pPrevPage)
/*****************************************************************************\
 *  @brief   Get the next adjacent sub page of m_hTtx->pageShowing.
 *  @param   [out] *pnextSubPage will be the next sub-page.
 *  @param   [in]  *pShowingPage is the current showing page.
 *  @param   [in]  *pSubPageInfo is the sub-page info of *pShowingPage.
 *  @ret     There's any next sub-page or not.
 *  @pre     TTX_BUF_SetShowingPage() is invoked before this function.
 *  @see     TTX_NAV_GetPrevSubPage().
\*****************************************************************************/
{
    TTX_STD_Page_AddrWithSubCode_t prevSubPage;

    if (m_hTtx->pageShowing.addr.subcodeA==0x0)
    {
        return FALSE; // It won't have sub pages
    }

    if ( TTX_BUF_FindPrevSubPage( &prevSubPage ) )
    {
        *pPrevPage = prevSubPage;
        return( TRUE );
    }

    return( FALSE );
#if 0
    if ( m_hTtx != NULL ) /* to avoid compiler warning of unused m_hTtx */
    {
        TTX_STD_Page_AddrWithSubCode_t prevSubPage;

        if ( TTX_BUF_FindPrevSubPage( NULL, &prevSubPage ) )
        {
            if ( pPrevPage != NULL )
            {
                *pPrevPage = prevSubPage;
            }
            return( TRUE );
        }
    }
    return( FALSE );

#endif

}

/*---------------------------------------------------------------------------*/
void
TTX_NAV_SetPageToShow  (
    const TTX_STD_Page_AddrWithSubCode_t* pPageToShow)
/*****************************************************************************\
 *  @brief   Integrated routine to update showing page.
 *  @note    Don't modify m_hTtx->pageShowing.addr nor calling TTX_BUF_SetShowingPage()
 *             directly. Use this __FUNCTION__ instead.
 *  @param   [in] *pShowingPage is the page that currently showing on screen.
 *  @see     TTX_BUF_SetShowingPage().
\*****************************************************************************/
{
//            TTX_REQUIRE( TTX_STD_isDisplayablePage( pPageToShow ) );

    /* Start searching... */
    TTX_DISP_EnableRolling( TRUE );

    // Update m_hTtx->usrConfig.pageToShow
    m_hTtx->usrConfig.pageToShow.pageAddr = *pPageToShow;
    //	TTX_Log(LOGGER_VERBOSE, "sps %x\n", (uint32_t) pPageToShow->pageA);

    m_hTtx->usrConfig.pageToShow.bIsSet   = TRUE;
}

/*---------------------------------------------------------------------------*/
void
TTX_NAV_SetShowingPage  (
    const TTX_STD_Page_AddrWithSubCode_t* pShowingPage)
/*****************************************************************************\
 *  @brief   Integrated routine to update showing page.
 *  @note    Don't modify m_hTtx->pageShowing.addr nor calling TTX_BUF_SetShowingPage()
 *             directly. Use this __FUNCTION__ instead.
 *  @param   [in] *pShowingPage is the page that currently showing on screen.
 *  @see     TTX_BUF_SetShowingPage().
\*****************************************************************************/
{
    if ( pShowingPage != NULL )
    {

        // Inform buffer manager to allow accurate TTX_BUF_FindNextDisplayableDifferentMainPage()... etc.
        TTX_BUF_SetShowingPage( pShowingPage );
    }
}


/* -- Implementations of Private Functions ----------------------------------*/




/*---------------------------------------------------------------------------*\
 * end of ttx_nav.c                                                          *
\*---------------------------------------------------------------------------*/


#endif
