/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     slr.c
 * @brief    Sparrow imlementation of VBI slicer driver.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Apr. 17, 2009
 * @version  0.1
\*****************************************************************************/

#include "vbi/slr.h"            /* Provides: functions to be implemented */
#include "sparrow.h"
#include "rtd_types.h"
#include "autoconf.h"
#include "isr_utility.h"
#include <stdlib.h>         /* Provides: NULL */

#define NUMPORTS (1) /* number of devices supported by this driver */


static SLR_Obj ports [NUMPORTS];

#if defined(CONFIG_WSS_SUPPORT)
static UINT16 vbi_wss;
static UINT16 vbi_preWss;
static UINT8 vbi_wss_stableCnt;
#endif

/* -- Prototypes of Private Functions ---------------------------------------*/

/* -- Implementations of Public Functions -----------------------------------*/

#if 0
/*---------------------------------------------------------------------------*/
SLR_Handle
drvif_SLR_Open (const SLR_DevId_t idDev, const SLR_OpenFlag_t flags)
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
    if ( idDev < NUMPORTS )
    {
        SLR_Handle hSlr = &( ports[idDev] );
        if ( flags & SLR_OPEN_RESET )
        {
            drvif_SLR_Reset( hSlr );
        }

        return( hSlr );
    }

    return( NULL );
}

/*---------------------------------------------------------------------------*/
void
drvif_SLR_Close (SLR_Handle hSlr)
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
    if ( hSlr != NULL )
    {
    }
}

/*---------------------------------------------------------------------------*/
void
drvif_SLR_Reset (SLR_Handle hSlr)
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
    if ( hSlr != NULL )
    {
    }
}
#endif

#if defined(CONFIG_WSS_SUPPORT)
void drvif_vbi_init_wss(void)
{
    vbi_wss = 0;
    vbi_preWss = 0;
    vbi_wss_stableCnt = 0;

    // decode setting
    rtdf_outl(TT_VBI_DATA_TYPE_CONFIG_REG_FOR_LINE_21_240X1803_0A14_0x5a14,0xcc666666); // line 21,22,23,24
    rtdf_outl(TT_VBI_DATA_TYPE_CONFIG_REG_FOR_LINE_25_260X1803_0A18_0x5a18,0x0000cc00); // line 26
    rtdf_outl(TT_VBI_CONTROL_REGISTER0X1803_0A00_0x5a00,0x4b4b2713);
    rtdf_outl(TT_VBI_DATA_STATUS0X1803_0A30_0x5a30,0x00004600);
    rtdf_outl(TT_VBI_LOOP_FILTER_I_GAIN_REGISTER0X1803_0A1C_0x5a1c,0x00008888);
    rtdf_outl(TT_VBI_DTO_REGISTER20X1803_0A24_0x5a24,0x15991599);
    rtdf_outl(TT_VBI_DTO_FRAME_START0X1803_0A28_0x5a28,0xbe78b4a8);
    rtdf_outl(TT_VBI_NOISY_CTRL50X1803_0A8C_0x5a8c,0);



    // enable WSS interrupt
    rtdf_setBits(TT_VBI_DATA_STATUS0X1803_0A30_0x5a30,_BIT30);

    drv_isr_SelectRoute( ROUTE_SEL_VBI_SLICER, IRQ_ROUTE_IRQ1 );
    //drv_isr_ClearPending( SYS_ISR_VBI_SLICER );
    drv_isr_Enable( SYS_ISR_VBI_SLICER, _TRUE );

}

/**
 * Handle WSS in ISR.
 *
 * @param	<void>	{ void }
 * @return 			{ void }
 */
void vbi_handle_wss(void)
{
    volatile UINT32 regData;
    UINT16 wssData = 0;

    regData = rtd_isr_inl(TT_VBI_CCWSS_DATA0X1803_0A2C_0x5a2c);
    wssData = (regData>>24&0xf);

    if (wssData != vbi_preWss)
    {
        vbi_preWss = wssData;
        vbi_wss_stableCnt = 0;
    }
    else
    {
        vbi_wss_stableCnt = (vbi_wss_stableCnt > 30) ? 30 : vbi_wss_stableCnt+1;

        if (vbi_wss_stableCnt >= 10)
            vbi_wss = vbi_preWss;
    }
}

/**
 * Read WSS.
 *
 * @param	<wss>		{ WSS }
 * @return 				{ void }
 */
void drvif_vbi_read_wss(UINT16* wss)
{
    *wss = vbi_wss;
}

//linmc20100105, for clear wssData
#if 0
void drvif_vbi_clear_wss(void)
{
    vbi_wss = 0;
}
#endif
bit drvif_vbi_wss_isr(void)
{
    UINT32 status;
    if ( rtd_isr_inb( SYS_GIP_1_0x2c39 ) & _BIT5 ) // check if is vbi pending (VBI Slicer Interrupt Pending Status)
    {
        status = rtd_isr_inl(TT_VBI_DATA_STATUS0X1803_0A30_0x5a30);
        if ( status& _BIT18) // Is WSS data ready?
        {
            // handle wss data
            vbi_handle_wss();

            rtd_isr_outl(TT_VBI_DATA_STATUS0X1803_0A30_0x5a30,status); // write clr status
            return( RT_ISR_HANDLED );
        }
    }
    return( RT_ISR_UNHANDLED );
}

#endif

#if 0 // open it when need
/**
 * read vbi data error status
 *
 * @param	<void>		{ void }
 * @return 				{ TRUE: error, FALSE: no error }
 */
BOOL drvif_SLR_read_error_status(void)
{
    if ( rtd_inl(VBI_DEBUG_MODE0X1803_0BC0_0x5bc0) & _BIT12 )   // error
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * reset vbi data error status
 *
 * @param	<void>		{ void }
 * @return 				{ void }
 */
void drvif_SLR_reset_error_status(void)
{
    if ( rtd_inl(VBI_DEBUG_MODE0X1803_0BC0_0x5bc0) & _BIT12 ) // error
        rtdf_setBits(VBI_DEBUG_MODE0X1803_0BC0_0x5bc0, _BIT12); // write clear
}
#endif

/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*\
 * end of slr.c                                                              *
\*---------------------------------------------------------------------------*/

