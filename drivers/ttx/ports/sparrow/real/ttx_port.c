/*---------------------------------------------------------------------------*\
 * (c) 2009 Realtek Semiconductor Corporation (http://www.realtek.com/)      *
 * All rights reserved.                                                      *
\*---------------------------------------------------------------------------*/

/*****************************************************************************\
 * @file     ttx_port.c
 * @brief    Implements platform dependent functions.
 * @details  This header declares the public access application interface for this TtLib library.
 * @author   Janus Cheng
 * @date     Dec. 11, 2007
 * @version  0.1
 * @ingroup  ttx
\*****************************************************************************/
#ifdef CONFIG_TTX_SUPPORT

#include "ttx_port.h"               /* Provides: functions to be implemented */
#include <sparrow/isr_utility.h>    /* Provides: drv_isr_Enable() */


/* -- Prototypes of Private Functions ---------------------------------------*/
static bool_t   _onOsdEnablingInterrupt     (void);
static bool_t   _onOsdDisablingInterrupt    (void);


/* -- Implementations of Public Functions -----------------------------------*/



/*---------------------------------------------------------------------------*/
bool_t
TTX_onPprEnablingInterrupt  (void)
/*****************************************************************************\
 *  @brief   Platform dependent codes for enabling interrupts for PPR.
 *  @param   bEnable  TRUE to enable interrupts.
 *                    FALSE to disable interrupts.
 *  @note    This code is not required to be in bank "common".
\*****************************************************************************/
{
    return( _onOsdEnablingInterrupt() );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_onPprDisablingInterrupt  (void)
/*****************************************************************************\
 *  @brief   Platform dependent codes for disabling interrupts for PPR.
 *  @param   bEnable  TRUE to enable interrupts.
 *                    FALSE to disable interrupts.
 *  @note    This code is not required to be in bank "common".
\*****************************************************************************/
{
    return( _onOsdDisablingInterrupt() );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_onOsdEnablingInterrupt  (void)
/*****************************************************************************\
 *  @brief   Platform dependent codes for enabling interrupts for VBI OSD.
 *  @note    This code is not required to be in bank "common".
\*****************************************************************************/
{
    return( _onOsdEnablingInterrupt() );
}

/*---------------------------------------------------------------------------*/
bool_t
TTX_onOsdDisablingInterrupt  (void)
/*****************************************************************************\
 *  @brief   Platform dependent codes for disabling interrupts for VBI OSD.
 *  @note    This code is not required to be in bank "common".
\*****************************************************************************/
{
    return( _onOsdDisablingInterrupt() );
}

/* -- Implementations of Private Functions ----------------------------------*/

/*---------------------------------------------------------------------------*/
static bool_t
_onOsdEnablingInterrupt  (void)
/*****************************************************************************\
 *  @brief   Platform dependent codes for enabling interrupts for PPR.
 *  @param   bEnable  TRUE to enable interrupts.
 *                    FALSE to disable interrupts.
 *  @note    This code is not required to be in bank "common".
\*****************************************************************************/
{
#if 1   // <Janus> Use polling instead of interrupt
    /* Curently, VBI PPR, VTE and OSD share the same as VBI OSD*/
    const IRQ_ENABLE_TYPE irqType      = SYS_ISR_VBIOSD;
    const IRQ_ROUTE_TYPE  irqRoute     = ROUTE_SEL_VBIOSD;
    const IRQ_ROUTE_PATH  irqRoutePath = IRQ_ROUTE_IRQ1;

    /* @note Don't use IRQ_ROUTE_IRQ1, this will crash the ISR */
    drv_isr_SelectRoute( irqRoute, irqRoutePath );
    //drv_isr_ClearPending( irqType );
    drv_isr_Enable( irqType, _TRUE );
#endif
    return( TRUE ); /* OK */
}

/*---------------------------------------------------------------------------*/
static bool_t
_onOsdDisablingInterrupt  (void)
/*****************************************************************************\
 *  @brief   Platform dependent codes for disabling interrupts for PPR.
 *  @param   bEnable  TRUE to enable interrupts.
 *                    FALSE to disable interrupts.
 *  @note    This code is not required to be in bank "common".
\*****************************************************************************/
{
#if 1   // <Janus> Use polling instead of interrupt
    /* Curently, VBI PPR, VTE and OSD share the same as VBI OSD*/
    const IRQ_ENABLE_TYPE irqType = SYS_ISR_VBIOSD;

    drv_isr_Enable( irqType, _FALSE );
    //drv_isr_ClearPending( irqType );
#endif
    return( TRUE ); /* OK */
}

/*---------------------------------------------------------------------------*\
 * end of ttx_port.c                                                         *
\*---------------------------------------------------------------------------*/
#endif
