/*==========================================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2009
  * All rights reserved.
  * ========================================================================*/

/*================= File Description =======================================*/
/**
 * @file
 * 	This file is for rtdaccess related functions.
 *
 * @author 	$Author$
 * @date	$Date$
 * @version	$Revision$
 * @ingroup	rtdaccess
 */

/**
 * @addtogroup rtdaccess
 * @{
 */
#include "autoconf.h"
#include "pcb\pcb_pin_struct.h"
#include "pcb\pcb_conf_def.h"
#include "irrc\irrc.h"
#include "pcb_customer.h"
#include "pcb\pwm.h"
#include "char\uart.h"
#include "i2c\i2c.h"
#include "char\lsadc.h"
#include "timer\timer.h"
#include "irrc\irrc_parameter.h"

_STRUCT_PCB_PARAMETER code stPcbData =
{
// Pad configure
    //Port A
    (((PAD_PORTA_7_SLEW_RATE << 3) |
    (PAD_PORTA_7_DRIVING_STRENGTH << 2) |
    (PAD_PORTA_7_PULL_UP_CONTROL << 1) |
    PAD_PORTA_7_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTA_6_SLEW_RATE << 3) |
    (PAD_PORTA_6_DRIVING_STRENGTH << 2) |
    (PAD_PORTA_6_PULL_UP_CONTROL << 1) |
    PAD_PORTA_6_PULL_DOWN_CONTROL),

    (((PAD_PORTA_5_SLEW_RATE << 3) |
    (PAD_PORTA_5_DRIVING_STRENGTH  << 2)|
    (PAD_PORTA_5_PULL_UP_CONTROL << 1) |
    PAD_PORTA_5_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTA_4_SLEW_RATE << 3) |
    (PAD_PORTA_4_DRIVING_STRENGTH << 2) |
    (PAD_PORTA_4_PULL_UP_CONTROL << 1) |
    PAD_PORTA_4_PULL_DOWN_CONTROL),

    (((PAD_PORTA_3_SLEW_RATE << 3) |
    (PAD_PORTA_3_DRIVING_STRENGTH << 2) |
    (PAD_PORTA_3_PULL_UP_CONTROL << 1) |
    PAD_PORTA_3_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTA_2_SLEW_RATE << 3) |
    (PAD_PORTA_2_DRIVING_STRENGTH << 2) |
    (PAD_PORTA_2_PULL_UP_CONTROL << 1) |
    PAD_PORTA_2_PULL_DOWN_CONTROL),

    (((PAD_PORTA_1_SLEW_RATE << 3) |
    (PAD_PORTA_1_DRIVING_STRENGTH << 2)|
    (PAD_PORTA_1_PULL_UP_CONTROL << 1) |
    PAD_PORTA_1_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTA_0_SLEW_RATE << 3) |
    (PAD_PORTA_0_DRIVING_STRENGTH << 2) |
    (PAD_PORTA_0_PULL_UP_CONTROL << 1) |
    PAD_PORTA_0_PULL_DOWN_CONTROL),

    //Port B
    (((PAD_PORTB_7_SLEW_RATE << 3) |
    (PAD_PORTB_7_DRIVING_STRENGTH  << 2) |
    (PAD_PORTB_7_PULL_UP_CONTROL << 1) |
    PAD_PORTB_7_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTB_6_SLEW_RATE << 3) |
    (PAD_PORTB_6_DRIVING_STRENGTH << 2) |
    (PAD_PORTB_6_PULL_UP_CONTROL << 1) |
    PAD_PORTB_6_PULL_DOWN_CONTROL),

    (((PAD_PORTB_5_SLEW_RATE << 3) |
    (PAD_PORTB_5_DRIVING_STRENGTH  << 2)|
    (PAD_PORTB_5_PULL_UP_CONTROL << 1) |
    PAD_PORTB_5_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTB_4_SLEW_RATE << 3) |
    (PAD_PORTB_4_DRIVING_STRENGTH << 2) |
    (PAD_PORTB_4_PULL_UP_CONTROL << 1) |
    PAD_PORTB_4_PULL_DOWN_CONTROL),

    (((PAD_PORTB_3_SLEW_RATE << 3) |
    (PAD_PORTB_3_DRIVING_STRENGTH << 2) |
    (PAD_PORTB_3_PULL_UP_CONTROL << 1) |
    PAD_PORTB_3_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTB_2_SLEW_RATE << 3) |
    (PAD_PORTB_2_DRIVING_STRENGTH << 2) |
    (PAD_PORTB_2_PULL_UP_CONTROL << 1) |
    PAD_PORTB_2_PULL_DOWN_CONTROL),

    (((PAD_PORTB_1_SLEW_RATE << 3) |
    (PAD_PORTB_1_DRIVING_STRENGTH << 2) |
    (PAD_PORTB_1_PULL_UP_CONTROL << 1) |
    PAD_PORTB_1_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTB_0_SLEW_RATE << 3) |
    (PAD_PORTB_0_DRIVING_STRENGTH << 2) |
    (PAD_PORTB_0_PULL_UP_CONTROL << 1) |
    PAD_PORTB_0_PULL_DOWN_CONTROL),

    //Port C
    (((PAD_PORTC_7_SLEW_RATE << 3) |
    (PAD_PORTC_7_DRIVING_STRENGTH  << 2) |
    (PAD_PORTC_7_PULL_UP_CONTROL << 1) |
    PAD_PORTC_7_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTC_6_SLEW_RATE << 3) |
    (PAD_PORTC_6_DRIVING_STRENGTH << 2) |
    (PAD_PORTC_6_PULL_UP_CONTROL << 1) |
    PAD_PORTC_6_PULL_DOWN_CONTROL),

    (((PAD_PORTC_5_SLEW_RATE << 3) |
    (PAD_PORTC_5_DRIVING_STRENGTH  << 2)|
    (PAD_PORTC_5_PULL_UP_CONTROL << 1) |
    PAD_PORTC_5_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTC_4_SLEW_RATE << 3) |
    (PAD_PORTC_4_DRIVING_STRENGTH  << 2)|
    (PAD_PORTC_4_PULL_UP_CONTROL << 1) |
    PAD_PORTC_4_PULL_DOWN_CONTROL),

    (((PAD_PORTC_3_SLEW_RATE << 3) |
    (PAD_PORTC_3_DRIVING_STRENGTH  << 2) |
    (PAD_PORTC_3_PULL_UP_CONTROL << 1)|
    PAD_PORTC_3_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTC_2_SLEW_RATE << 3) |
    (PAD_PORTC_2_DRIVING_STRENGTH << 2) |
    (PAD_PORTC_2_PULL_UP_CONTROL << 1) |
    PAD_PORTC_2_PULL_DOWN_CONTROL),

    (((PAD_PORTC_1_SLEW_RATE << 3) |
    (PAD_PORTC_1_DRIVING_STRENGTH  << 2) |
    (PAD_PORTC_1_PULL_UP_CONTROL << 1) |
    PAD_PORTC_1_PULL_DOWN_CONTROL) << 4) |
    ((PAD_PORTC_0_SLEW_RATE << 3) |
    (PAD_PORTC_0_DRIVING_STRENGTH  << 2) |
    (PAD_PORTC_0_PULL_UP_CONTROL << 1) |
    PAD_PORTC_0_PULL_DOWN_CONTROL),

//Misc
    // PAD_SETTING_AHS_SMT_CTRL,
    0x65,	// default

    // PAD_SETTING_LVDS_RSDS_TTL_CFG
    0x4a,	// default

    // PortA Smith Trigger control
    ((PAD_PORTA_7_SMITH_TRIGGER_CONTROL << 7) |
    (PAD_PORTA_6_SMITH_TRIGGER_CONTROL << 6) |
    (PAD_PORTA_5_SMITH_TRIGGER_CONTROL << 5) |
    (PAD_PORTA_4_SMITH_TRIGGER_CONTROL << 4) |
    (PAD_PORTA_3_SMITH_TRIGGER_CONTROL << 3) |
    (PAD_PORTA_2_SMITH_TRIGGER_CONTROL << 2) |
    (PAD_PORTA_1_SMITH_TRIGGER_CONTROL << 1) |
    (PAD_PORTA_0_SMITH_TRIGGER_CONTROL)),

    // PortB Smith Trigger control
    ((PAD_PORTB_7_SMITH_TRIGGER_CONTROL << 7) |
    (PAD_PORTB_6_SMITH_TRIGGER_CONTROL << 6) |
    (PAD_PORTB_5_SMITH_TRIGGER_CONTROL << 5) |
    (PAD_PORTB_4_SMITH_TRIGGER_CONTROL << 4) |
    (PAD_PORTB_3_SMITH_TRIGGER_CONTROL << 3) |
    (PAD_PORTB_2_SMITH_TRIGGER_CONTROL << 2) |
    (PAD_PORTB_1_SMITH_TRIGGER_CONTROL << 1) |
    (PAD_PORTB_0_SMITH_TRIGGER_CONTROL)),

    // PortC Smith Trigger control
    ((PAD_PORTC_7_SMITH_TRIGGER_CONTROL << 7) |
    (PAD_PORTC_6_SMITH_TRIGGER_CONTROL << 6) |
    (PAD_PORTC_5_SMITH_TRIGGER_CONTROL << 5) |
    (PAD_PORTC_4_SMITH_TRIGGER_CONTROL << 4) |
    (PAD_PORTC_3_SMITH_TRIGGER_CONTROL << 3) |
    (PAD_PORTC_2_SMITH_TRIGGER_CONTROL << 2) |
    (PAD_PORTC_1_SMITH_TRIGGER_CONTROL << 1) |
    (PAD_PORTC_0_SMITH_TRIGGER_CONTROL)),

// Pin Share
    //Port A
    ((PIN_SHARE_PORTA_7 & 0x7) << 4) | (PIN_SHARE_PORTA_6 & 0x7),
    ((PIN_SHARE_PORTA_5 & 0x7) << 4) | (PIN_SHARE_PORTA_4 & 0x7),
    ((PIN_SHARE_PORTA_3 & 0x7) << 4) | (PIN_SHARE_PORTA_2 & 0x7),
    ((PIN_SHARE_PORTA_1 & 0x7) << 4) | (PIN_SHARE_PORTA_0 & 0x07),

    //Port B
    ((PIN_SHARE_PORTB_7 & 0x7)) << 4 | (PIN_SHARE_PORTB_6 & 0x7),
    ((PIN_SHARE_PORTB_5 & 0x7)) << 4 | (PIN_SHARE_PORTB_4 & 0x7),
    ((PIN_SHARE_PORTB_3 & 0x7)) << 4 | (PIN_SHARE_PORTB_2 & 0x7),
    ((PIN_SHARE_PORTB_1 & 0x7)) << 4 | (PIN_SHARE_PORTB_0 & 0x7),

    //Port C
    0,
    ((PIN_SHARE_PORTC_5 & 0x7)) << 4 | (PIN_SHARE_PORTC_4 & 0x7),
    ((PIN_SHARE_PORTC_3 & 0x7)) << 4 | (PIN_SHARE_PORTC_2 & 0x7),
    ((PIN_SHARE_PORTC_1 & 0x7)) << 4 | (PIN_SHARE_PORTC_0 & 0x7),

    //Port D
    ((PIN_SHARE_PORTD_1 & 0x7)) << 4 | (PIN_SHARE_PORTD_0 & 0x7),
    (PIN_SHARE_PORTD_2 & 0x7),

    //Port G
    ((PIN_SHARE_PORTG_7 & 0x7)) << 4 | (PIN_SHARE_PORTG_6 & 0x7),
    ((PIN_SHARE_PORTG_5 & 0x7)) << 4 | (PIN_SHARE_PORTG_4 & 0x7),
    ((PIN_SHARE_PORTG_3 & 0x7)) << 4 | (PIN_SHARE_PORTG_2 & 0x7),
    ((PIN_SHARE_PORTG_1 & 0x7)) << 4 | (PIN_SHARE_PORTG_0 & 0x7),

    //Port H
    ((PIN_SHARE_PORTH_7 & 0x7)) << 4 | (PIN_SHARE_PORTH_6 & 0x7),
    ((PIN_SHARE_PORTH_5 & 0x7)) << 4 | (PIN_SHARE_PORTH_4 & 0x7),
    ((PIN_SHARE_PORTH_3 & 0x7)) << 4 | (PIN_SHARE_PORTH_2 & 0x7),
    ((PIN_SHARE_PORTH_1 & 0x7)) << 4 | (PIN_SHARE_PORTH_0 & 0x7),

    //Port I
    ((PIN_SHARE_PORTI_7 & 0x7)) << 4 | (PIN_SHARE_PORTI_6 & 0x7),
    ((PIN_SHARE_PORTI_5 & 0x7)) << 4 | (PIN_SHARE_PORTI_4 & 0x7),
};

// GPIO parameter table
STRUCT_GPIO_CONFIGURATION code stGPIOData =
{
    {	// Directory
        ((GPIO0_DIRECTION & _BIT0)  |
        ((GPIO1_DIRECTION & _BIT0) << 1)  |
        ((GPIO2_DIRECTION & _BIT0)  << 2) |
        ((GPIO3_DIRECTION & _BIT0) << 3)  |
        ((GPIO4_DIRECTION & _BIT0)  << 4) |
        ((GPIO5_DIRECTION & _BIT0) << 5)  |
        ((GPIO6_DIRECTION & _BIT0) << 6)  |
        ((GPIO7_DIRECTION & _BIT0) << 7)),
        ((GPIO8_DIRECTION & _BIT0)  |
        ((GPIO9_DIRECTION & _BIT0) << 1)  |
        ((GPIO10_DIRECTION & _BIT0)  << 2) |
        ((GPIO11_DIRECTION & _BIT0) << 3)  |
        ((GPIO12_DIRECTION & _BIT0)  << 4) |
        ((GPIO13_DIRECTION & _BIT0) << 5)  |
        ((GPIO14_DIRECTION & _BIT0) << 6)  |
        ((GPIO15_DIRECTION & _BIT0) << 7)),
        ((GPIO16_DIRECTION & _BIT0)  |
        ((GPIO17_DIRECTION & _BIT0) << 1)  |
        ((GPIO18_DIRECTION & _BIT0)  << 2) |
        ((GPIO19_DIRECTION & _BIT0) << 3)  |
        ((GPIO20_DIRECTION & _BIT0)  << 4) |
        ((GPIO21_DIRECTION & _BIT0) << 5)  |
        ((GPIO22_DIRECTION & _BIT0) << 6)  |
        ((GPIO23_DIRECTION & _BIT0) << 7)),
        ((GPIO24_DIRECTION & _BIT0)  |
        ((GPIO25_DIRECTION & _BIT0) << 1)  |
        ((GPIO26_DIRECTION & _BIT0)  << 2) |
        ((GPIO27_DIRECTION & _BIT0) << 3)  |
        ((GPIO28_DIRECTION & _BIT0)  << 4) |
        ((GPIO29_DIRECTION & _BIT0) << 5)  |
        ((GPIO30_DIRECTION & _BIT0) << 6)  |
        ((GPIO31_DIRECTION & _BIT0) << 7)),
        ((GPIO32_DIRECTION & _BIT0)  |
        ((GPIO33_DIRECTION & _BIT0) << 1)  |
        ((GPIO34_DIRECTION & _BIT0)  << 2) |
        ((GPIO35_DIRECTION & _BIT0) << 3)  |
        ((GPIO36_DIRECTION & _BIT0)  << 4) |
        ((GPIO37_DIRECTION & _BIT0) << 5)  |
        ((GPIO38_DIRECTION & _BIT0) << 6))
    },
    {	// Active Level
        ((GPIO0_ACTICE_LEVEL & _BIT0)  |
        ((GPIO1_ACTICE_LEVEL & _BIT0) << 1)  |
        ((GPIO2_ACTICE_LEVEL & _BIT0)  << 2) |
        ((GPIO3_ACTICE_LEVEL & _BIT0) << 3)  |
        ((GPIO4_ACTICE_LEVEL & _BIT0)  << 4) |
        ((GPIO5_ACTICE_LEVEL & _BIT0) << 5)  |
        ((GPIO6_ACTICE_LEVEL & _BIT0) << 6)  |
        ((GPIO7_ACTICE_LEVEL & _BIT0) << 7)),
        ((GPIO8_ACTICE_LEVEL & _BIT0)  |
        ((GPIO9_ACTICE_LEVEL & _BIT0) << 1)  |
        ((GPIO10_ACTICE_LEVEL & _BIT0)  << 2) |
        ((GPIO11_ACTICE_LEVEL & _BIT0) << 3)  |
        ((GPIO12_ACTICE_LEVEL & _BIT0)  << 4) |
        ((GPIO13_ACTICE_LEVEL & _BIT0) << 5)  |
        ((GPIO14_ACTICE_LEVEL & _BIT0) << 6)  |
        ((GPIO15_ACTICE_LEVEL & _BIT0) << 7)),
        ((GPIO16_ACTICE_LEVEL & _BIT0)  |
        ((GPIO17_ACTICE_LEVEL & _BIT0) << 1)  |
        ((GPIO18_ACTICE_LEVEL & _BIT0)  << 2) |
        ((GPIO19_ACTICE_LEVEL & _BIT0) << 3)  |
        ((GPIO20_ACTICE_LEVEL & _BIT0)  << 4) |
        ((GPIO21_ACTICE_LEVEL & _BIT0) << 5)  |
        ((GPIO22_ACTICE_LEVEL & _BIT0) << 6)  |
        ((GPIO23_ACTICE_LEVEL & _BIT0) << 7)),
        ((GPIO24_ACTICE_LEVEL & _BIT0)  |
        ((GPIO25_ACTICE_LEVEL & _BIT0) << 1)  |
        ((GPIO26_ACTICE_LEVEL & _BIT0)  << 2) |
        ((GPIO27_ACTICE_LEVEL & _BIT0) << 3)  |
        ((GPIO28_ACTICE_LEVEL & _BIT0)  << 4) |
        ((GPIO29_ACTICE_LEVEL & _BIT0) << 5)  |
        ((GPIO30_ACTICE_LEVEL & _BIT0) << 6)  |
        ((GPIO31_ACTICE_LEVEL & _BIT0) << 7)),
        ((GPIO32_ACTICE_LEVEL & _BIT0)  |
        ((GPIO33_ACTICE_LEVEL & _BIT0) << 1)  |
        ((GPIO34_ACTICE_LEVEL & _BIT0)  << 2) |
        ((GPIO35_ACTICE_LEVEL & _BIT0) << 3)  |
        ((GPIO36_ACTICE_LEVEL & _BIT0)  << 4) |
        ((GPIO37_ACTICE_LEVEL & _BIT0) << 5)  |
        ((GPIO38_ACTICE_LEVEL & _BIT0) << 6))
    },
    {	// De-bounce clock base.
        GPIO_0_7_DEBOUNCE,
        GPIO_8_15_DEBOUNCE,
        GPIO_16_23_DEBOUNCE,
        GPIO_24_31_DEBOUNCE,
        GPIO_32_38_DEBOUNCE
    }
};

STRUCT_AGPIO_CONFIGURATION code stAGPIOData =
{
    // AGPIO_JD_0	Enable Output
    ((AGPO_LSADC0_ENABLE_OUTPUT & _BIT0) |
    ((AGPO_LSADC1_ENABLE_OUTPUT & _BIT0) << 1) |
    ((AGPO_LSADC2_ENABLE_OUTPUT & _BIT0) << 2) |
    ((AGPO_LSADC3_ENABLE_OUTPUT & _BIT0) << 3)),

    // AGPIO_JD_2		Enable Input
    ((AGPO_LSADC0_ENABLE_INPUT & _BIT0) |
    ((AGPO_LSADC1_ENABLE_INPUT & _BIT0) << 1) |
    ((AGPO_LSADC2_ENABLE_INPUT & _BIT0) << 2) |
    ((AGPO_LSADC3_ENABLE_INPUT & _BIT0) << 3)),

    // AGPIO_AUDIO_0 	Enable Output
    ((AGPO_AIO_2L_ENABLE_OUTPUT & _BIT0) |
    ((AGPO_AIO_2R_ENABLE_OUTPUT & _BIT0) << 1)),

    // AGPIO_Audio_2		Enable Input
    ((AGPO_AIO_2L_ENABLE_INPUT & _BIT0) |
    ((AGPO_AIO_2R_ENABLE_INPUT & _BIT0) << 1)),

    // AGPIO_VD_0		Enable outupt
    ((AGPO_VIN00P_ENABLE_OUTPUT & _BIT0) |
    ((AGPO_VIN10P_ENABLE_OUTPUT & _BIT0) << 1) |
    ((AGPO_VIN01P_ENABLE_OUTPUT & _BIT0) << 2) |
    ((AGPO_VIN11P_ENABLE_OUTPUT & _BIT0) << 3) |
    ((AGPO_VIN0N_ENABLE_OUTPUT & _BIT0) << 4) |
    ((AGPO_VIN02P_ENABLE_OUTPUT & _BIT0) << 5) |
    ((AGPO_VIN12P_ENABLE_OUTPUT & _BIT0) << 6) |
    ((AGPO_VIN1N_ENABLE_OUTPUT & _BIT0) << 7)),

    // AGPIO_VD_2		 Enable input
    ((AGPO_VIN00P_ENABLE_INPUT & _BIT0) |
    ((AGPO_VIN10P_ENABLE_INPUT & _BIT0) << 1) |
    ((AGPO_VIN01P_ENABLE_INPUT & _BIT0) << 2) |
    ((AGPO_VIN11P_ENABLE_INPUT & _BIT0) << 3) |
    ((AGPO_VIN0N_ENABLE_INPUT & _BIT0) << 4) |
    ((AGPO_VIN02P_ENABLE_INPUT & _BIT0) << 5) |
    ((AGPO_VIN12P_ENABLE_INPUT & _BIT0) << 6) |
    ((AGPO_VIN1N_ENABLE_INPUT & _BIT0) << 7)),

    // AGPIO_VD_4		Enable outupt
    ((AGPO_VIN03P_ENABLE_OUTPUT & _BIT0) |
    ((AGPO_RGBSW0_ENABLE_OUTPUT & _BIT0) << 1) |
    ((AGPO_AIN_2L_ENABLE_OUTPUT & _BIT0) << 2) |
    ((AGPO_RGBSW1_ENABLE_OUTPUT & _BIT0) << 3) |
    ((AGPO_AIN_2R_ENABLE_OUTPUT & _BIT0) << 4) |
    ((AGPO_AVOUT2_ENABLE_OUTPUT & _BIT0) << 5)),

    // AGPIO_VD_6		 Enable input
    ((AGPO_VIN03P_ENABLE_INPUT & _BIT0) |
    ((AGPO_RGBSW0_ENABLE_INPUT & _BIT0) << 1) |
    ((AGPO_AIN_2L_ENABLE_INPUT & _BIT0) << 2) |
    ((AGPO_RGBSW1_ENABLE_INPUT & _BIT0) << 3) |
    ((AGPO_AIN_2R_ENABLE_INPUT & _BIT0) << 4) |
    ((AGPO_AVOUT2_ENABLE_INPUT & _BIT0) << 5)),

    // AGPIO_ADC_0		Enable Output
    ((AGPO_B2N_ENABLE_OUTPUT & _BIT0) |
    ((AGPO_B2P_ENABLE_OUTPUT & _BIT0) << 1) |
    ((AGPO_G2N_ENABLE_OUTPUT & _BIT0) << 2) |
    ((AGPO_G2P_ENABLE_OUTPUT & _BIT0) << 3) |
    ((AGPO_R2N_ENABLE_OUTPUT & _BIT0) << 4) |
    ((AGPO_R2P_ENABLE_OUTPUT & _BIT0) << 5) |
    ((AGPO_AIN_5L_ENABLE_OUTPUT & _BIT0) << 6) |
    ((AGPO_AIN_5R_ENABLE_OUTPUT & _BIT0) << 7)),

    // AGPIO_ADC_2		Enable Input
    ((AGPO_B2N_ENABLE_INPUT & _BIT0) |
    ((AGPO_B2P_ENABLE_INPUT & _BIT0) << 1) |
    ((AGPO_G2N_ENABLE_INPUT & _BIT0) << 2) |
    ((AGPO_G2P_ENABLE_INPUT & _BIT0) << 3) |
    ((AGPO_R2N_ENABLE_INPUT & _BIT0) << 4) |
    ((AGPO_R2P_ENABLE_INPUT & _BIT0) << 5) |
    ((AGPO_AIN_5L_ENABLE_INPUT & _BIT0) << 6) |
    ((AGPO_AIN_5R_ENABLE_INPUT & _BIT0) << 7))
};

// PWM parameter table
STRUCT_PWM_PARAMETER code stPWMParameter[PWM_TOTAL] =
{
    {PWM0_ENABLE, PWM_0, PWM0_GPIO, PWM0_SourceDivisor, PWM0_OutputDivisor},
    {PWM1_ENABLE, PWM_1, PWM1_GPIO, PWM1_SourceDivisor, PWM1_OutputDivisor},
    {PWM2_ENABLE, PWM_2, PWM2_GPIO, PWM2_SourceDivisor, PWM2_OutputDivisor},
    {PWM3_ENABLE, PWM_3, PWM3_GPIO, PWM3_SourceDivisor, PWM3_OutputDivisor},
    {PWM4_ENABLE, PWM_4, PWM4_GPIO, PWM4_SourceDivisor, PWM4_OutputDivisor},
    {PWM5_ENABLE, PWM_5, PWM5_GPIO, PWM5_SourceDivisor, PWM5_OutputDivisor}
};

// Uart parameter table
STRUCT_UART_PARAMETER code stUartParameter[UART_SOURCE_TOTAL] =
{
    // UART0
    {UART_SOURCE_0_BAUDRATE,
        UART_SOURCE_0_PARITY_ENABLE,
        UART_SOURCE_0_PARITY_SELECT,
        UART_SOURCE_0_STOP_BITS,
        UART_SOURCE_0_DATA_BITS},

    // UART1
    {UART_SOURCE_1_BAUDRATE,
     UART_SOURCE_1_PARITY_ENABLE,
     UART_SOURCE_1_PARITY_SELECT,
     UART_SOURCE_1_STOP_BITS,
     UART_SOURCE_1_DATA_BITS}
};

// I2C parameter Table
STRUCT_I2C_PARAMETER code stI2CParameter[CHIP_I2C_TOTAL] =
{
    // I2C
    { _ENABLE, MASTER, I2C_ADDR_7, I2C_SPEED_STD, I2C_RX_FIFO_TL, I2C_TX_FIFO_TL, I2C_1_Local_Addr},

    // I2C_1
    { _DISABLE, MASTER, I2C_ADDR_7, I2C_SPEED_STD, I2C_RX_FIFO_TL, I2C_TX_FIFO_TL, 0x52},

    // I2C_2
    { _DISABLE, MASTER, I2C_ADDR_7, I2C_SPEED_FAST, I2C_RX_FIFO_TL, I2C_TX_FIFO_TL, 0x54}
};

// IR parameter Table
STRUCT_IR_PARAMETER code stIrrcSettings =
{
//  IR_PSR Reg:0x2880
    DEFAULT_IRRBL,
    DEFAULT_IRRML,
    DEFAULT_IRRDZL,
    DEFAULT_IRRDOL,

// IR_PER Reg:0x2884
    DEFAULT_IRRDL,
    DEFAULT_IRRRL,
    DEFAULT_IRRSL,

// IR_SF Reg:0x2888
    DEFAULT_IRIT,
    DEFAULT_IROCT,
    0x021b,

// IR_CR  Reg:0x2980
    DEFAULT_IREDNM,
    DEFAULT_IREDN,
    DEFAULT_MLAE,
    DEFAULT_IRRES,
    DEFAULT_IRCM,
    DEFAULT_IRBME,
    DEFAULT_IRDPM,
    DEFAULT_IRDN,

// IR_SR	Reg:0x2898
    DEFAULT_IRRF
};

LSADC_CONFIG_PARAMETER code lsadcConfig =
{
    PAD_LSADC_OPAMP_SETTLING_TIME,	// selWait
    PAD_LSADC_ADC_CLOCK_RATE,	// selClk
    PAD_LSADC_DEBOUNCE_COUNT,	// debounce
    {
        {PAD_LSADC_ENABLE_0, PAD_LSADC_SWITCH_0, PAD_LSADC_MODE_CONTROL_0, PAD_LSADC_THRESHOLD_0, PAD_LSADC_CURRENT_0},
        {PAD_LSADC_ENABLE_1, PAD_LSADC_SWITCH_1, PAD_LSADC_MODE_CONTROL_1, PAD_LSADC_THRESHOLD_1, PAD_LSADC_CURRENT_1},
        {PAD_LSADC_ENABLE_2, PAD_LSADC_SWITCH_2, PAD_LSADC_MODE_CONTROL_2, PAD_LSADC_THRESHOLD_2, PAD_LSADC_CURRENT_2},
        {PAD_LSADC_ENABLE_3, PAD_LSADC_SWITCH_3, PAD_LSADC_MODE_CONTROL_3, PAD_LSADC_THRESHOLD_3, PAD_LSADC_CURRENT_3},
        {PAD_LSADC_ENABLE_4, PAD_LSADC_SWITCH_4, PAD_LSADC_MODE_CONTROL_4, PAD_LSADC_THRESHOLD_4, PAD_LSADC_CURRENT_4},
        {PAD_LSADC_ENABLE_5, PAD_LSADC_SWITCH_5, PAD_LSADC_MODE_CONTROL_5, PAD_LSADC_THRESHOLD_5, PAD_LSADC_CURRENT_5},
        {PAD_LSADC_ENABLE_6, PAD_LSADC_SWITCH_6, PAD_LSADC_MODE_CONTROL_6, PAD_LSADC_THRESHOLD_6, PAD_LSADC_CURRENT_6},
        {PAD_LSADC_ENABLE_7, PAD_LSADC_SWITCH_7, PAD_LSADC_MODE_CONTROL_7, PAD_LSADC_THRESHOLD_7, PAD_LSADC_CURRENT_7}
    }
};

// Timer
TIMER_PARAMETER_STRUCT_TABLE  code stTimerParameter[TIMER_TOTAL] =
{
    {TIMER_ENABLE_0, TIMER_TARGET_0, TIMER_DIVIDER_0},
    {TIMER_ENABLE_1, TIMER_TARGET_1, TIMER_DIVIDER_1},
    {TIMER_ENABLE_2, TIMER_TARGET_2, TIMER_DIVIDER_2}
};
