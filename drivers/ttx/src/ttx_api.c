/*---------------------------------------------------------------------------*\
 * (c) 2008 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/
/*****************************************************************************\
 * @file     ttx_api.c
 * @brief    Prototypes of public application interface.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT
#include <ttx.h>            /* Provides: functions to be implemented */
#include <ttx_assert.h>     /* Provides: TTX_DEFINE_THIS_MODULE() */
#include <ttx_source.h>     /* Provides: TTX_SRC_Open() */
#include <ttx_buff.h>       /* Provides: TTX_BUF_Open() and TTX_BUF_Close() */
#include <ttx_disp.h>       /* Provides: TTX_DISP_Open() */
#include <ttx_dec_top.h>    /* Provides: TTX_TOP_Init() */
#include <ttx_log.h>        /* Provides: TTX_Log(LOGGER_VERBOSE, ) */

#include <sys_hal.h>
#include <stdlib.h>         /* Provides: NULL */


#ifdef __KEIL__   /* KeilC 8051 compiler */
#include <sys.h>        /* Provides: System module */
#endif  /* end of #ifdef __KEIL__ */


#include <ttx_assert.h> /* Provides: TTX_DEFINE_THIS_MODULE() */

#include <flow_struct.h>


TTX_DEFINE_THIS_MODULE( ttx_api.c )

TTX_Obj ttx;   /* actual private device instance */


/* -- Prototypes of Private Functions ---------------------------------------*/

/* -- Implementations of Public Functions -----------------------------------*/



/*---------------------------------------------------------------------------*/
void
TTX_Open (void)
/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    m_hTtx = &ttx;

    TTX_OBJ_ResetAllMemberVariables( );    /* Construct m_hTtx */

#if !DV_SIM /* don't waste time in DV_SIM */
//        TTX_TestSoftware();
#endif

#ifdef __KEIL__   /* KeilC 8051 compiler */
    SYS_Open( SYS_OPEN_RESET );
#endif  /* end of #ifdef __KEIL__ */

    /* Force share-mem to mode-0 (no UIOSD).
     * @todo Take mode-2 (UIOSD+TT) into considerationi here.
     */
    _SYS_REG_WRITE( 0x2DA1, 0x00 );

    TTX_BUF_Open();
    TTX_SRC_Open( TTX_SRC_OPEN_RESET );
    TTX_DISP_Reset(); // clear VBI OSD memory
    TTX_DISP_Open( TTX_DISP_OPEN_RESET );
    TTX_TOP_Init(); // initialize TOP service module

    /* Since we have done required operations of TTX_STATE_INITIAL,
     *   we now can transit to TTX_STATE_READY.
     */
    TTX_STATE_TransitTo( READY );
    TTX_Log(LOGGER_INFO, "[TTX] state: INIT -> READY\n");

}

/*---------------------------------------------------------------------------*/
void
TTX_Close ()
/*****************************************************************************\
 *  @brief   The de-initialization routine.
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
    TTX_DISP_Close();
    TTX_SRC_Close();
    TTX_BUF_Close();
}

/*---------------------------------------------------------------------------*/
void
TTX_Reset ()
/*****************************************************************************\
 *  @brief   The initialization routine.
 *  @details Use this function to initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @return  the initialization status.
 *  @retval  TTX_TRUE  success
 *  @retval  TTX_FALSE failure
 *
 *  @ingroup ttx
 *  @see     TTX_Deinit()
\*****************************************************************************/
{
    TTX_DISP_Reset();
    TTX_SRC_Reset();
    TTX_BUF_Reset();
    TTX_OBJ_ResetAllMemberVariables( );
}

/*---------------------------------------------------------------------------*/
bool_t TTX_SetInitPage (const TTX_STD_Page_AddrWithSubCode_t* pageInit)
/*****************************************************************************\
 * @brief		set initial page
 * @param	<pageInit>	initial page
 * @ret		SUCCESS or FAIL
\*****************************************************************************/
{
    if ( (pageInit != NULL) && TTX_STD_Page_AddrIsValid(pageInit) )
    {
        ttx.pageInitial = *pageInit;
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

/*---------------------------------------------------------------------------*/
bool_t TTX_GetInitPage (TTX_STD_Page_AddrWithSubCode_t* pageInit)
/*****************************************************************************\
 * @brief		get initial page
 * @param	<pageInit>	initial page
 * @ret		SUCCESS or FAIL
\*****************************************************************************/
{
    if ( pageInit != NULL )
    {
        *pageInit = ttx.pageInitial;
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

#if !TTX_RELEASE
/*---------------------------------------------------------------------------*/
char const TTX_ROM * TTX_ROM_VAR
TTX_GetVersion  (void)
/*****************************************************************************\
 *  @brief   The assertion-failure handler callback.
 *  @details Use this function to de-initialize this library.
 *  @note    Must be called in advance of any other functions.
 *
 *  @ingroup ttx
 *  @see     TTApi_DeInit()
\*****************************************************************************/
{
    return( TTX_VER );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_GetStatus               (const  TTX_Status_t* pStatus)
/*****************************************************************************\
 *  @brief   A blocking call to wait for the previous TTX_Control() to be completed.
 *  @pre     1. Must be called immediately after TTX_Control().
 *  @retval  TRUE if success.
 *  @retval  FALSE if timeout or error.
\*****************************************************************************/
{
    if ( m_hTtx != NULL )
    {
        if ( pStatus != NULL )
        {
            pStatus->state      = m_hTtx->state;
            pStatus->statistics = m_hTtx->statistics;
            return( TRUE );
        }
    }

    return( FALSE );
}
#endif  /* end of #if !TTX_RELEASE */

void	 TTX_PowerOff()
{
    if ( m_hTtx != NULL )
    {
        TTX_Control( TTX_CMD_HIDE);	/* hide teletext */
        TTX_Control( TTX_CMD_STOP);
        TTX_WaitForCompletion();
        TTX_Close();

        // Disable global mapping of 4k-Imem
        rtdf_maskb(GLOBAL_MAPPING_0x2c08, ~_BIT1, 0);
        // Disable 4k-iMem map
        rtdf_clearBitsb(IMEM4K_REMAP_0x2c02, _BIT0);
        // Disable 8k-iMem map
        rtdf_outb(IMEM8K_REMAP_0x2c05, 0);
        // Disable global mapping of 8k-Imem
        rtdf_clearBitsb(GLOBAL_MAPPING_0x2c08, _BIT0);
        rtd_outb( SYS_SHARE_MEM_0x2da1, 0x05 );	// restore share-mem to UIOSD

        if (GET_INTERLACE_MODE())	// Enable Video compensate
            rtdf_setBits(DM_UZU_CTRL_0x1604, _BIT11);
    }
}


/* -- Implementations of Private Functions ----------------------------------*/


/*---------------------------------------------------------------------------*\
 * end of ttx_api.c                                                          *
\*---------------------------------------------------------------------------*/
#endif
