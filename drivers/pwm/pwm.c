/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2006
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is a main fuction and entrance for firmwae.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	System
 */

#include "pcb\pwm.h"
#include "pcb\pcb_conf_def.h"
//#include "rosprintf.h"
//  The real PWM clock frequency is crystal clock(27MHz) / PWM clock source divisor / PWM output clock divisor / 2
//
extern STRUCT_PWM_PARAMETER code stPWMParameter[PWM_TOTAL];
void drv_pwm_Init(void)
{
    UINT8 i;
    UINT8 remain;
    UINT32 regVal;
    UINT16 getOffset;
    STRUCT_PWM_PARAMETER* ptPWMParameter;

    //-- Release reset
    rtdf_setBitsb(SYS_SRST_3_0x2ca3, _BIT7);

    //-- Clock Enable
    rtdf_setBitsb(SYS_CLKEN_2_0x2cc2, _BIT7);

    for (i = 0; i < 	PWM_TOTAL; i++
        )
    {
        ptPWMParameter = &stPWMParameter[i];
        if (ptPWMParameter->enable == _ENABLE)
        {
            // Setup PWM & GPIO configuration
            if (ptPWMParameter->gpio_no < GPIO_B0)  	// use port A
            {
                // Get register address
                //getOffset = 3 - ((ptPWMParameter->gpio_no - GPIO_A1) >> 1);
                getOffset = 3 - (((ptPWMParameter->gpio_no+1) - GPIO_A1) >> 1);
                getOffset += PAS0_0x2d60;
            }
            else if (ptPWMParameter->gpio_no < GPIO_C0)  	// use port B
            {
                // Get register address
                getOffset = 3 - ((ptPWMParameter->gpio_no - GPIO_B0) >> 1);
                getOffset += PBS0_0x2d64;
            }
            else if (ptPWMParameter->gpio_no < GPIO_D0)   // use port C
            {
                // Get register address
                getOffset = 3 - ((ptPWMParameter->gpio_no - GPIO_C0) >> 1);
                getOffset += PCS0_0x2d68;
            }
            else
                continue;

            regVal = rtd_inb(getOffset);

            if (ptPWMParameter->gpio_no < GPIO_D0)
                remain = (ptPWMParameter->gpio_no+1)% 2;
            else
                remain = ptPWMParameter->gpio_no % 2;


            if (remain)
                regVal = 	(regVal & 0x0f) | (ptPWMParameter->no << 4);
            else
                regVal = 	(regVal & 0xf0) | ptPWMParameter->no;

            rtd_outb(getOffset, regVal);

            // Set clock source divisor for PWM
            remain = ptPWMParameter->no << 2;
            regVal = rtd_inl(PWM_CSD_0x2810);
            regVal &= ~((UINT32)0xf << remain);
            regVal |= (ptPWMParameter->SourceDivisor & 0x0f) << remain;
            rtd_outl(PWM_CSD_0x2810, regVal);

            // Set output clock divisor for PWM
            getOffset = ptPWMParameter->no >> 2;
            //getOffset += PWM03_OCD_0x2800;
            getOffset = getOffset*4;
            getOffset += PWM03_OCD_0x2800;

            regVal = rtd_inl(getOffset);
            remain = ptPWMParameter->no % 4;
            remain = remain << 3;
            regVal &= ~((UINT32)0xff << remain);
            regVal |= (ptPWMParameter->OutputDivisor & 0xff) << remain;
            rtd_outl(getOffset, regVal);
        }
    }
    //rtd_outb(PAS1_0x2d61, 0x90);
}

void drv_pwm_PowerOn(bit on_off)
{
    if (on_off)
    {
    }
    else
    {
        rtd_outb(PAS0_0x2d60, 0x99);
        rtd_outb(PAS1_0x2d61, 0x99);
        rtd_outb(PAS2_0x2d62, 0x99);
        rtd_outb(PAS3_0x2d63, 0x99);
        rtd_outb(PBS0_0x2d64, 0x99);
        rtd_outb(PBS1_0x2d65, 0x99);
        rtd_outb(PBS2_0x2d66, 0x99);
        rtd_outb(PBS3_0x2d67, 0x99);
//	rtd_outb(PCS0_0x2d68 0x99);
        rtd_outb(PCS1_0x2d69, 0x09);
        rtd_outb(PCS2_0x2d6a, 0x99);
        rtd_outb(PCS3_0x2d6b, 0x99);
    }
}

void drv_pwm_SetClockDuty(UINT8 no, UINT8 duty)
{
    UINT8 remain;
    UINT16 getOffset;
    UINT32 regVal;

    if (no > PWM_5)
        return;

    getOffset = no >> 2;
    getOffset += PWM03_CD_0x2808;
    regVal = rtd_inl(getOffset);
    remain = no % 4;
    remain = remain << 3;

    regVal &= ~(0xff << remain);
    regVal |= ((UINT32)duty << remain);
    //RTD_Log(LOGGER_INFO, "\nSetClockDuty:duty=%d, getOffset=%x, regVal=%d\n", (UINT32)duty, (UINT32)getOffset, (UINT32)regVal);

    rtd_outl(getOffset, regVal);
}

