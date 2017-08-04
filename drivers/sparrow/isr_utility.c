/*=============================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2005
  * All rights reserved.
  * ============================================ */

/*================= File Description ================= */
/**
 * @file
 * The file is a declaration and definition header file
 *
 * @author 	$Author:  $
 * @date 	$Date:  $
 * @version 	$Revision:  $
 * @ingroup
 */

/**
* @addtogroup
* @
*/
#include "isr_utility.h"
#include "reg_def.h"
#include "scaler_idomain\measure.h"
#include "video\video.h"
#include "timer\timer.h"
#include "rosprintf.h"

bit drv_isr_SelectRoute(IRQ_ROUTE_TYPE type, IRQ_ROUTE_PATH irq_route)
{
    UINT8 offset;
    UINT8 offset1;
//	UINT32 val;

    if ((type >=  ROUTE_SEL_TOTAL) || (irq_route >= IRQ_ROUTE_TOTAL))
        return _FALSE;

    // Calculate register offest
    offset = type / 10;
    offset = (offset << 2);

//	val = irq_route;
//	RTD_Log(LOGGER_INFO, "irq_route: %x ...\n", val);

    // Clear route path & set route path
    offset1 = type % 10;
//	val = offset1;
//	RTD_Log(LOGGER_INFO, "drv_isr_SelectRoute: offset1 0 %x ...\n", val);
    switch (offset1)
    {
    case 0:
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , (0x07 << 2));
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << (offset1 + 2)));
        break;

    case 1:
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , (0x7 << 5));
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << (offset1 + 5)));
        break;

    case 2:
        offset += 1;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , 0x7);
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << offset1));
        break;

    case 3:
        offset += 1;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , (0x7 << 3));
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << (offset1 + 3)));
        break;

    case 4:
        offset += 1;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , (0x3 << 6));
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset + 1, _BIT0);
        offset1 = irq_route;
        if (offset1 > 1)
            rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset + 1, _BIT0);
        else
            rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset, (1 << (offset1 + 6)));
        break;

    case 5:
        offset += 2;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , 0x0e); //  (0x7 << 1)
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << (offset1 + 1)));
        break;

    case 6:
        offset += 2;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , (0x7 << 4));
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << (offset1 + 4)));
        break;

    case 7:
        offset += 2;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , _BIT7);
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset + 1 , 0x3);
        offset1 = irq_route;
        if (offset1 > 0)
            rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset + 1, (1 << (offset1 - 1)));
        else
            rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset, _BIT7);
        break;

    case 8:
        offset += 3;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , (0x7 << 2));
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << (offset1 + 2)));
        break;

    case 9:
        offset += 3;
        rtdf_clearBitsb(SYS_GIR_0_0x2c24 + offset , (0x7 << 5));
        offset1 = irq_route;
        rtdf_setBitsb(SYS_GIR_0_0x2c24 + offset,  (0x1 << (offset1 + 5)));
        break;
    };

    return _TRUE;
}

void drv_isr_Enable(IRQ_ENABLE_TYPE type, bit enable)
{
    UINT8 offset;
    UINT8 offset1;
//	UINT32 val;

    offset = type >> 3;
    offset1 = type % 8;
//	val = type;
//	RTD_Log(LOGGER_INFO, "drv_isr_enable type %x ...\n", val);
//	val = offset;
//	RTD_Log(LOGGER_INFO, "drv_isr_enable offset %x ...\n", val);
//	val = offset1;
//	RTD_Log(LOGGER_INFO, "drv_isr_enable offset1 %x ...\n", val);

    if (enable)
        rtdf_setBitsb(SYS_GIE_0_0x2c34 + offset, (1 << offset1));
    else
        rtdf_clearBitsb(SYS_GIE_0_0x2c34 + offset, (1 << offset1));

//	val = rtd_inb(SYS_GIE_0_0x2c34 + offset);
//	RTD_Log(LOGGER_INFO, "SYS_GIE_0_0x2c34:%x ...\n", val);

}

void drv_isr_isr_Enable(IRQ_ENABLE_TYPE type, bit enable)
{
    volatile UINT8 offset;
    volatile UINT8 offset1;
//	UINT32 val;

    offset = type >> 3;
    offset1 = type % 8;
//	val = type;
//	RTD_Log(LOGGER_INFO, "drv_isr_enable type %x ...\n", val);
//	val = offset;
//	RTD_Log(LOGGER_INFO, "drv_isr_enable offset %x ...\n", val);
//	val = offset1;
//	RTD_Log(LOGGER_INFO, "drv_isr_enable offset1 %x ...\n", val);

    if (enable)
        rtd_isr_maskb(SYS_GIE_0_0x2c34 + offset, 0xff, (1 << offset1));
//		rtdf_setBitsb(SYS_GIE_0_0x2c34 + offset, (1 << offset1));
    else
//		rtdf_clearBitsb(SYS_GIE_0_0x2c34 + offset, (1 << offset1));
        rtd_isr_maskb(SYS_GIE_0_0x2c34 + offset, ~(1 << offset1), 0x00);

//	val = rtd_inb(SYS_GIE_0_0x2c34 + offset);
//	RTD_Log(LOGGER_INFO, "SYS_GIE_0_0x2c34:%x ...\n", val);

}

bit drv_isr_GetPending(IRQ_ENABLE_TYPE type)// reentrant 
{
    volatile UINT8 offset;
    volatile UINT8 offset1;

    offset = type >> 3;
    offset1 = type % 8;
    return (rtd_isr_inb(SYS_GIP_0_0x2c38 + offset) & (1 << (offset1)));
}
#if 0
void drv_isr_ClearPending(IRQ_ENABLE_TYPE type)
{
    /* <Janus>2009/08/25: Currently, there're no global pending bits to clear. */
    type = 0;	// to eliminate compiler warning of "unused type".
}
#endif
