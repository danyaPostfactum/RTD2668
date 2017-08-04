/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     slr.h
 * @brief    Vertical Blanking Interval (VBI) Slicer driver.
 * @author   Janus Cheng
 * @date     Apr. 17, 2009
 * @version  0.1
 * @ingroup  vbi
\*****************************************************************************/

/**
 * @addtogroup vbi
 * @{
 */
#ifndef __REALTEK_VERTICAL_BLANKING_INTERVAL_SLICER_HEADER_INCLUDED__
#define __REALTEK_VERTICAL_BLANKING_INTERVAL_SLICER_HEADER_INCLUDED__ (1)

/* -- headers ---------------------------------------------------------------*/
#include <stdint.h>     /* Provides: uint_least8_t */

#ifdef __cplusplus
extern "C"
{
#endif

    /* -- Global Macro Definitions ----------------------------------------------*/
    typedef enum SLR_VideoSys_t
    {
        SLR_VIDEO_SYS_UNKNOWN,
        SLR_VIDEO_SYS_NTSC,
        SLR_VIDEO_SYS_PAL
    } SLR_VideoSys_t;

    typedef enum SLR_AdcSrc_t
    {
        SLR_ADC_SRC_VDC,
        SLR_ADC_SRC_YPbPr
    } SLR_AdcSrc_t;


    /* SLR_open() flags */
    typedef enum
    {
        SLR_OPEN_NOP   = 0x00000000u,
        SLR_OPEN_RESET = 0x00000001u
    } SLR_OpenFlag_t;

    /* private object, not to be used by application code */
    typedef struct
    {
        uint_least8_t dummy;
    } SLR_Obj, *SLR_Handle;


    /* device ID */
    typedef uint_fast8_t SLR_DevId_t;

    /* -- Global typedef Declarations -------------------------------------------*/




    /* -- Function Prototypes ---------------------------------------------------*/

    SLR_Handle              drvif_SLR_Open                      (const SLR_DevId_t idDev, const SLR_OpenFlag_t flags);
    void                    drvif_SLR_Close                     (SLR_Handle hSlr);
    void                    drvif_SLR_Reset                     (SLR_Handle hSlr);

#if defined(CONFIG_WSS_SUPPORT)
    void drvif_vbi_init_wss(void);
    void drvif_vbi_read_wss(UINT16* wss);
    void vbi_handle_wss(void);
    void drvif_vbi_clear_wss(void);
    bit drvif_vbi_wss_isr(void);
#endif

#if 0 // open it when need
    BOOL drvif_SLR_read_error_status(void);
    void drvif_SLR_reset_error_status(void);
#endif

#ifdef CONFIG_VBI_CC_SUPPORT
    void drvif_vbi_cc_verify_init(void);
    void drvif_vbi_cc_verify_play(void);
    void drvif_vbi_cc_verify_play_ypbpr(void);
    bit drvif_vbi_cc_verify_isr(void);
#endif


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif /* !__REALTEK_VERTICAL_BLANKING_INTERVAL_SLICER_HEADER_INCLUDED__ */

/**
 * @}
 */

/*---------------------------------------------------------------------------*\
 * end of slr.h                                                              *
\*---------------------------------------------------------------------------*/
