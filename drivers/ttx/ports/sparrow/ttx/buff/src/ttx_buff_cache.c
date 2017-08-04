/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_buff_cache.c
 * @brief    Teletext page-buffer management (time-critical routines).
 * @author   Janus Cheng
 * @date     Aug. 25, 2009
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include <ttx_buff.h>       /* Provides: functions to be implemented */
#include <ttx_buff_pkg.h>   /* Provides: TTX_DEFAULT_CD_PAGE_ADDRESS */

#include <ttx_log.h>        /* Provides: OSD_Log( LOGGER_WARN,  () */
#include <ttx_assert.h>     /* Provides: TTX_REQUIRE() and TTX_ENSURE() */
#include <ttx_std.h>        /* Provides: TTX_STD_Page_AddrWithSubCode_Hex_t */


#include <stdint.h>         /* Provides: UINT_LEAST8_MAX */
#include <string.h>         /* Provides: memset() */
#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */



/* -- Prototypes of Private Functions ---------------------------------------*/
/*---------------------------------------------------------------------------*/
static bool_t
TTX_BUF_ISRKeepQueGetIdx  (
    const TTX_STD_Page_AddrWithSubCode_t*  pPageAddr,
    TTX_BUF_KeepQueIdx_t*                  pIndex)
/*****************************************************************************\
 *  @brief   Returns the index of ttxBuf.records[] whose page address is the
 *              the same as *pPageAddr and the records[] is currently been used.
 *  @param   [in]  *pPageAddr is the page address in page buffer.
\*****************************************************************************/
{
    TTX_BUF_KeepQueIdx_t i;
    uint16_t tmp = pPageAddr->subcodeA;

    if (((tmp) & 0x3f00) == 0x3f00)
        tmp =  tmp & 0xff; // skip S3, S4

    if ( pPageAddr != NULL )
    {
        for ( i=0; i<TTX_MAX_PAGE_KEEP_NUM; i++ )
        {
#if 0
            if (( ttxBuf.keepQue.elements[i].pageA == pPageAddr->pageA) &&
                    ( ttxBuf.keepQue.elements[i].subcodeA== tmp ))    /* ttxBuf.keepQue.elements[i].addrPage == *pPageAddr */
            {
                *pIndex = i;
                return( TRUE ); /* found. We stop iterating here because we assume each records has unique page-address */
            }
#else
            if (ttxBuf.keepQue.elements[i].pageA == pPageAddr->pageA)
            {
                if ((ttxBuf.keepQue.elements[i].subcodeA == pPageAddr->subcodeA)||(ttxBuf.keepQue.elements[i].subcodeA == tmp)||(pPageAddr->pageA == 0x1F0))
                {
                    *pIndex = i;
                    return( TRUE ); /* found. We stop iterating here because we assume each records has unique page-address */
                }
            }

#endif
        }
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*/
static uint8_t TTX_BUF_ISRFindFarPage(uint32_t * far_dist)
{
    uint8_t i, far_index=0;
    uint16_t dist=0, tmp_dist=0;
#ifdef BUFFER_TOP_PAGE
    uint8_t   count_1 = 0, count_2 = 0, count_3 = 0;
#endif

    // find a buffer for it
    for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
    {

        if (ttxBuf.pageList.elements[i].keep) // skip the keep buffer
            continue;

#ifdef BUFFER_TOP_PAGE
        //force to buffer BTT table
        if ((ttxBuf.BttPage.pageA != 0) && (count_1<1) &&
                (ttxBuf.pageList.elements[i].pageA == ttxBuf.BttPage.pageA))
        {
            count_1++;
            continue;
        }
        //force to buffer AIT / MPE table
        if ( (count_2 < 1) && (ttxBuf.Ait_Mpe_Page[0].pageA != 0) &&
                (ttxBuf.pageList.elements[i].pageA == ttxBuf.Ait_Mpe_Page[0].pageA) &&
                (ttxBuf.pageList.elements[i].subcodeA== ttxBuf.Ait_Mpe_Page[0].subcodeA))
        {
            count_2++;
            continue;
        }
        if ( (count_3 < 1) && (ttxBuf.Ait_Mpe_Page[1].pageA != 0) &&
                (ttxBuf.pageList.elements[i].pageA == ttxBuf.Ait_Mpe_Page[1].pageA) &&
                (ttxBuf.pageList.elements[i].subcodeA== ttxBuf.Ait_Mpe_Page[1].subcodeA))
        {
            count_3++;
            continue;
        }
#endif

        if (ttxBuf.pageList.elements[i].pageA > ttxBuf.pageList.pageToShow.pageA)
            dist = ttxBuf.pageList.elements[i].pageA - ttxBuf.pageList.pageToShow.pageA;
        else
            dist = ttxBuf.pageList.pageToShow.pageA - ttxBuf.pageList.elements[i].pageA;

        if (dist > tmp_dist)
        {
            tmp_dist = dist;
            far_index = i;
        }
    }
    //TTX_Log(LOGGER_VERBOSE, "\n");

    *far_dist = tmp_dist;

    return far_index;
}

/*---------------------------------------------------------------------------*/
static uint8_t TTX_BUF_ISRFindFarSubPage(uint32_t * far_dist)
{
    uint8_t i, far_index=0;
    uint16_t dist=0, tmp_dist=0;
    uint8_t first_page=0, last_page=0;
#ifdef BUFFER_TOP_PAGE
    uint8_t   count_1 = 0, count_2 = 0, count_3 = 0;
#endif

    // find a buffer for it
    for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
    {

        if (ttxBuf.pageList.elements[i].keep) // skip the keep buffer
            continue;

#ifdef BUFFER_TOP_PAGE
        //force to buffer BTT table
        if ((ttxBuf.BttPage.pageA != 0) && (count_1<1) &&
                (ttxBuf.pageList.elements[i].pageA == ttxBuf.BttPage.pageA))
        {
            count_1++;
            continue;
        }
        //force to buffer AIT / MPE table
        if ( (count_2 < 1) && (ttxBuf.Ait_Mpe_Page[0].pageA != 0) &&
                (ttxBuf.pageList.elements[i].pageA == ttxBuf.Ait_Mpe_Page[0].pageA) &&
                (ttxBuf.pageList.elements[i].subcodeA== ttxBuf.Ait_Mpe_Page[0].subcodeA))
        {
            count_2++;
            continue;
        }
        if ( (count_3 < 1) && (ttxBuf.Ait_Mpe_Page[1].pageA != 0) &&
                (ttxBuf.pageList.elements[i].pageA == ttxBuf.Ait_Mpe_Page[1].pageA) &&
                (ttxBuf.pageList.elements[i].subcodeA== ttxBuf.Ait_Mpe_Page[1].subcodeA))
        {
            count_3++;
            continue;
        }
#endif

        if (ttxBuf.pageList.elements[i].pageA == ttxBuf.pageList.pageToShow.pageA)
        {

            if (ttxBuf.pageList.elements[i].subcodeA > ttxBuf.pageList.pageToShow.subcodeA)
            {
                dist = ttxBuf.pageList.elements[i].subcodeA - ttxBuf.pageList.pageToShow.subcodeA;
            }
            else
            {
                dist = ttxBuf.pageList.pageToShow.subcodeA - ttxBuf.pageList.elements[i].subcodeA;
            }
        }
        if (dist > tmp_dist)
        {
            tmp_dist = dist;
            far_index = i;
        }
    }

    *far_dist = tmp_dist;

    return far_index;
}
/* -- Implementations of Public Functions -----------------------------------*/

/*---------------------------------------------------------------------------*/
bool_t
TTX_BUF_ISRFindPage (const TTX_STD_Page_AddrWithSubCode_t * pPageAddr,
                     uint8_t * page_index)
/*****************************************************************************\
 *  @brief   find a page in buffer and returns its memory address
 *  @ret     the page whose page address is *pPageAddr is in page buffer or not
\*****************************************************************************/
{
    uint8_t TTX_MEM* pX27 = NULL;

    /*
    check if in-comming page is belong to keep buffer
    if yes
    	check if it is already in buffer
    	if yes
    		find it & receive it
    	else
    		allocate buffer for it
    else
    	compare the distance with other not-keep buffer pages
    	if less
    		allocate buffer for it
    	else
    		if it is displaying page's sub-page
    			issue the sub-page's display
    		else
    			do nothing (don't receive it)
    */

    uint8_t i, far_index=0, keep_index;
    uint32_t dist=0, far_dist=0;

    // check if it is already in buffer
    for ( i=0; i<TTX_MAX_PAGE_NUM; i++ )
    {
        if ((ttxBuf.pageList.elements[i].pageA == pPageAddr->pageA) &&
                (ttxBuf.pageList.elements[i].subcodeA == pPageAddr->subcodeA))    // already in buffer
        {
            *page_index = i;
            if ( ttxBuf.pageList.elements[i].bstatus != TTX_BUFFER_COLLECTING )    // if main page is still on collecting, don't interrupt it (for X0 repeat case)
            {
                if ( ttxBuf.pageList.elements[i].X27_status == TTX_X27_EMPTY )
                {
                    ttxBuf.pageList.elements[i].X27_status = TTX_X27_COLLECTING;
                    ttxBuf.pageList.elements[i].collecting_count = 0;
                }
                else if ( ttxBuf.pageList.elements[i].X27_status == TTX_X27_COLLECTING )
                {
                    pX27 = (uint8_t TTX_MEM*)ttxBuf.pageList.elements[i].addrMem + 41*40;

                    if ( (pX27[37] == 0x00) && (pX27[38] == 0x00) && (pX27[39] == 0x00) )
                    {
                        if ( ttxBuf.pageList.elements[i].collecting_count < 2 )
                        {
                            ttxBuf.pageList.elements[i].collecting_count++;
                        }
                        else
                        {
                            ttxBuf.pageList.elements[i].X27_status = TTX_X27_COLLECTED;
                        }
                    }
                    else
                    {
                        ttxBuf.pageList.elements[i].X27_status = TTX_X27_COLLECTED;
                    }
                }
            }
            return TRUE;
        }
    }

    if (TTX_BUF_ISRKeepQueGetIdx( pPageAddr, &keep_index ))
    {
        far_index = TTX_BUF_ISRFindFarPage(&far_dist);
        // Subpage condtition, use TTX_BUF_ISRFindFarSubPage to compare again.
        if (far_dist == 0)
            far_index = TTX_BUF_ISRFindFarSubPage(&far_dist);
        *page_index = far_index;
        ttxBuf.pageList.elements[far_index].keep = TRUE;
        ttxBuf.keepQue.elements[keep_index].bindex = far_index;
        ttxBuf.pageList.elements[far_index].X27_status = TTX_X27_EMPTY;
//		TTX_Log(LOGGER_VERBOSE, "keep %d\n", (uint32_t) far_index);
        return TRUE;
    }
    else
    {
        // find a buffer for it
        far_index = TTX_BUF_ISRFindFarPage(&far_dist);

        if (pPageAddr->pageA > ttxBuf.pageList.pageToShow.pageA)
            dist = pPageAddr->pageA - ttxBuf.pageList.pageToShow.pageA;
        else
            dist = ttxBuf.pageList.pageToShow.pageA - pPageAddr->pageA;

        // if it is not the most far, record it
        if (dist<far_dist)
        {
            *page_index = far_index;
            ttxBuf.pageList.elements[far_index].X27_status = TTX_X27_EMPTY;
//			TTX_Log(LOGGER_VERBOSE, "far %d\n",(uint32_t) far_index);
            return TRUE;
        }
        else
        {
            /*if it is displaying page's sub-page
            issue the sub-page's display*/
            // need to add code here! ghyu
            if (pPageAddr->pageA == ttxBuf.pageList.pageToShow.pageA)
            {
                far_index = TTX_BUF_ISRFindFarSubPage(&far_dist);

                if (pPageAddr->subcodeA > ttxBuf.pageList.pageToShow.subcodeA)
                    dist = pPageAddr->subcodeA - ttxBuf.pageList.pageToShow.subcodeA;
                else
                    dist = ttxBuf.pageList.pageToShow.subcodeA - pPageAddr->subcodeA;

                //TT_Log(LOGGER_INFO, "far_index : %d,  far_dist : %d, dist : %d \n", (uint32_t)  far_index, (uint32_t)  far_dist, (uint32_t) dist);

                if (dist<far_dist)
                {
                    *page_index = far_index;
                    ttxBuf.pageList.elements[far_index].X27_status = TTX_X27_EMPTY;
                    return TRUE;
                }
            }

            return FALSE;
        }
    }
}

void TTX_BUF_ISRSetSubPageMap(uint16_t subCode)
{
    uint8_t num;
    uint8_t index, offset;

    if ( (subCode & 0x3f00) != 0 )
        return;

//	if( (subCode & 0xf0) > (0x70 ))
//		return;

    if ( ( subCode &0xf) > 9 )
        return;

    num = 10*((subCode & 0x70) >>4)+ (subCode & 0xf);
    //TTX_Log(LOGGER_VERBOSE, "sub %d %x\n",(uint32_t) num,(uint32_t) subCode);

    index = num/8;
    offset =  num%8;

    ttxBuf.pageList.subPageIndicator[index] |=  (0x1<<offset);

}


/*---------------------------------------------------------------------------*\
 * end of ttx_buff_cache.c                                                   *
\*---------------------------------------------------------------------------*/
#endif
