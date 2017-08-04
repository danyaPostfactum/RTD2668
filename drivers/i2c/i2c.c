/*=============================================================
  * Copyright (c)      Realtek Semiconductor Corporation, 2004
  *
  * All rights reserved.
  *
  *============================================================*/
#include "i2c\i2c.h"
#include "rosprintf.h"
#include "reg52.h"
#define    udelay(x)
#include "timer_event.h"

extern STRUCT_I2C_PARAMETER code stI2CParameter[CHIP_I2C_TOTAL];

void drv_i2c_Clear_all(UINT8 i2c_master)
{
    UINT16 io_base;

    if (i2c_master >= I2C_NUM)
        return;

    io_base = I2C_BASE_ADDRESS(i2c_master);;

    rtd_inl(io_base+I2C_IC_CLR_INTR);
    rtd_inl(io_base+I2C_IC_CLR_RX_UNDER);
    rtd_inl(io_base+I2C_IC_CLR_RX_OVER);
    rtd_inl(io_base+I2C_IC_CLR_TX_OVER);
    rtd_inl(io_base+I2C_IC_CLR_RD_REQ);
    rtd_inl(io_base+I2C_IC_CLR_TX_ABRT);
    rtd_inl(io_base+I2C_IC_CLR_RX_DONE);
    rtd_inl(io_base+I2C_IC_CLR_ACTIVITY);
    rtd_inl(io_base+I2C_IC_CLR_STOP_DET);
    rtd_inl(io_base+I2C_IC_CLR_START_DET);
    rtd_inl(io_base+I2C_IC_CLR_GEN_CALL);
}

bit drv_i2c_Set_rx_tx_tl(UINT8 i2c_master, UINT8 rx_tl, UINT8 tx_tl)
{
    UINT16 io_base;
    UINT8 fEnable;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = I2C_BASE_ADDRESS(i2c_master);
    fEnable = 0;
    /* First disable IC  */
    if (rtdf_inl(io_base + I2C_IC_ENABLE) & IC_ENABLE_ENABLE)
    {
        rtdf_clearBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        udelay(20);
        fEnable = 1;
    }

    rtd_outl(io_base+I2C_IC_RX_TL, rx_tl);
    rtd_outl(io_base+I2C_IC_TX_TL, tx_tl);

    /* last enable IC */
    if (fEnable)
        rtdf_setBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);

    return _TRUE;
}

bit drv_i2c_Set_speed(UINT8 i2c_master, I2C_SPEED_T speed)
{
    UINT16 io_base;
    UINT8 fEnable;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = I2C_BASE_ADDRESS(i2c_master);
    fEnable = 0;

    /* First disable IC  */
    if (rtdf_inl(io_base + I2C_IC_ENABLE) & IC_ENABLE_ENABLE)
    {
        rtdf_clearBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        fEnable = 1;
        udelay(20);
    }

    rtd_maskl(io_base + I2C_IC_CON, ~IC_CON_SPEED_MASK, IC_CON_SPEED(speed));

    switch (speed)
    {
    case I2C_SPEED_STD:
        rtd_outl(io_base + I2C_IC_SS_SCL_HCNT,I2C_STD_SCL_HCNT);
        rtd_outl(io_base + I2C_IC_SS_SCL_LCNT,I2C_STD_SCL_LCNT);
        break;

    case I2C_SPEED_FAST:
        rtd_outl(io_base + I2C_IC_FS_SCL_HCNT,I2C_FAST_SCL_HCNT);
        rtd_outl(io_base + I2C_IC_FS_SCL_LCNT,I2C_FAST_SCL_LCNT);
        break;

    default:
        break;
    }

    /* last enable IC */
    if (fEnable)
        rtdf_setBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);

    return _TRUE;
}

bit drv_i2c_Set_LocalAddr(UINT8 i2c_master, UINT16 local_addr)
{
    UINT16 io_base;
    UINT8	fEnable;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = I2C_BASE_ADDRESS(i2c_master);
    fEnable = 0;

    /* First disable IC  */
    if (rtdf_inl(io_base + I2C_IC_ENABLE) & IC_ENABLE_ENABLE)
    {
        rtdf_clearBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        fEnable = 1;
    }

    rtd_maskl(io_base + I2C_IC_SAR, ~MIS_IC_SAR_MASK, MIS_IC_SAR(local_addr));

    /* last enable IC */
    if (fEnable)
        rtdf_setBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);

    return _TRUE;
}

#if 0 // Temporily not use
UINT16 drv_i2c_Get_LocalAddr(UINT8 i2c_master)
{
    UINT16 io_base;

    if (i2c_master >= I2C_NUM)
        return 0;

    io_base = I2C_BASE_ADDRESS(i2c_master);

    return (rtd_inl(io_base + I2C_IC_SAR) & MIS_IC_SAR_MASK);
}
#endif

bit drv_i2c_Set_TargetAddr(UINT8 i2c_master, UINT16 Target_addr)
{
    UINT16 io_base;
    UINT8 fEnable;
    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = I2C_BASE_ADDRESS(i2c_master);
    fEnable = 0;

    /* First disable IC  */
    if (rtdf_inl(io_base + I2C_IC_ENABLE) & IC_ENABLE_ENABLE)
    {
        rtdf_clearBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        fEnable = 1;
    }

    rtd_maskl(io_base + I2C_IC_TAR, ~MIS_IC_TAR_MASK, MIS_IC_TAR(Target_addr));

    /* last enable IC */
    if (fEnable)
        rtdf_setBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);

    return _TRUE;
}

UINT16 drv_i2c_Get_TargetAddr(UINT8 i2c_master)
{
    UINT16 io_base;

    if (i2c_master >= I2C_NUM)
        return 0;

    io_base = I2C_BASE_ADDRESS(i2c_master);

    return (rtd_inl(io_base + I2C_IC_TAR) & MIS_IC_TAR_MASK);

}

bit drv_i2c_Set_AddressType(UINT8 i2c_master, UINT8 addr_type)
{
    UINT16 io_base;
    UINT8 fEnable;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = I2C_BASE_ADDRESS(i2c_master);
    fEnable = 0;
    /* First disable IC  */
    if (rtdf_inl(io_base + I2C_IC_ENABLE) & IC_ENABLE_ENABLE)
    {
        rtdf_clearBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        fEnable = 1;
    }

    if (addr_type == I2C_ADDR_10)
    {
        rtdf_setBits(io_base + I2C_IC_CON, IC_CON_10BITADDR_MASTER);
        rtdf_setBits(io_base+I2C_IC_CON, IC_CON_10BITADDR_SLAVE);
    }
    else
    {
        rtdf_clearBits(io_base + I2C_IC_CON, IC_CON_10BITADDR_MASTER);
        rtdf_clearBits(io_base + I2C_IC_CON, IC_CON_10BITADDR_SLAVE);
    }

    /* last enable IC */
    if (fEnable)
        rtdf_setBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
}

bit drv_i2c_Set_MasterMode(UINT8 i2c_master, UINT8 master_type)
{
    UINT16 io_base;
    UINT8 fEnable;
    if (i2c_master >= I2C_NUM)
        return _FALSE;

    if ((master_type != MASTER) && (master_type != SLAVE))
        return _FALSE;

    io_base = I2C_BASE_ADDRESS(i2c_master);
    fEnable = 0;
    /* First disable IC  */
    if (rtdf_inl(io_base + I2C_IC_ENABLE) & IC_ENABLE_ENABLE)
    {
        rtdf_clearBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        fEnable = 1;
    }

    if (master_type == MASTER)
        rtdf_setBits(io_base + I2C_IC_CON , IC_CON_MASTER_MODE_ENABLE);
    else
        rtdf_clearBits(io_base + I2C_IC_CON , IC_CON_MASTER_MODE_ENABLE);

    /* First Enable IC  */
    if (fEnable)
        rtdf_setBits(io_base + I2C_IC_ENABLE, IC_ENABLE_ENABLE);

    return _TRUE;
}

bit drv_i2c_Write(UINT8 i2c_master, UINT8 w_len, UINT8* write_buf)
{
#if 1
    UINT16 i;
    UINT32 /*value, */value1;//For Keil Building
    UINT16 /*i2c_addr,*/ i2c_addr1;//For Keil Building
    //UINT16 count;//For Keil Building

    if (i2c_master >= I2C_NUM)
        return _FALSE;
    EA = 0;
    i2c_addr1 = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr1 += I2C_IC_DATA_CMD;
    value1 = rtd_inl(i2c_addr1);
    *((UINT8*)&value1 + 2) &= 0xfe;	// Bit8: CMD 0:Write, 1:Read
    for (i = 0 ; i < w_len ; i++)
    {
        /* wait while queue is full */
        // Check whether Tx buffer full or not
//		count = 600;
//		while(count-- > 0) {
//			value = rtd_inl(i2c_addr);
//			if(*((UINT8*)&value + 3) & 0x02)	// BIT1: Transmit FIFO Not Full
//				break;

//			udelay(20); // 10 ^ -4 seconds (enough for 1 byte to transmit)
//		}

        *((UINT8*)&value1 + 3) = write_buf[i];

        rtd_outl(i2c_addr1, value1);
    }
    EA = 1;
    return _TRUE;
}

bit drv_i2c_randomRead(UINT32 i2c_master,UINT8 w_len, UINT8 r_len, UINT8* write_buf, UINT8* read_buf)
{
#if 1
    UINT16 i2c_addr, i2c_addr1, i2c_addr2, i2c_addr3;
    UINT32 value, value1, value2, value3;
    UINT16 count,count1, count2;
    int i;
    UINT16 timercnt;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    /* wait while queue is empty */
    // Check whether Tx buffer empty or not
    i2c_addr = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr += I2C_IC_STATUS;
    count = 600;
    while (count-- > 0)
    {
        value = rtd_inl(i2c_addr);
        if (*((UINT8*)&value + 3) & 0x04)	// BIT2: Transmit FIFO Completely Empty
            break;

//		udelay(40); // 10 ^ -4 seconds (enough for 1 byte to transmit)
//		fw_timer_event_DelayXms(3);
        fw_timer_event_DelayXus(20);
    }

    if (!count)
    {
        drv_i2c_Clear_all(i2c_master);
        RTD_Log(LOGGER_INFO, "drv_i2c_randomRead1 read error %x\n", (UINT32)i);
        return _FALSE;
    }

    // Send Address Command/
    i2c_addr1 = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr1 += I2C_IC_DATA_CMD;
    value1 = rtd_inl(i2c_addr1);
    *((UINT8*)&value1 + 2) &= 0xfe;	// Bit8: CMD 0:Write, 1:Read
    EA = 0;
    for (i = 0 ; i < w_len ; i++)
    {
        /* wait while queue is full */
        // Check whether Tx buffer full or not
//		count = 600;
//		while(count-- > 0) {
//			value = rtd_inl(i2c_addr);
//			if(*((UINT8*)&value + 3) & 0x02)	// BIT1: Transmit FIFO Not Full
//				break;

//			udelay(20); // 10 ^ -4 seconds (enough for 1 byte to transmit)
//		}

        *((UINT8*)&value1 + 3) = write_buf[i];
        rtd_outl(i2c_addr1, value1);
    }
    EA = 1;
    // Send Read command & accept data from i2c
    i = 0;
    count = 600;
    i2c_addr2 = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr2 += I2C_IC_RXFLR;

    i2c_addr3 = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr3 += I2C_IC_RAW_INTR_STAT;
    value1 = rtd_inl(i2c_addr1);
    EA = 0;
    while ((i < r_len) && (count-- > 0))
    {
        count1 = 1000;
        while (count1-- > 0)
        {
            value = rtd_inl(i2c_addr);
            if (*((UINT8*)&value + 3) & 0x02) // BIT1: Transmit FIFO Not Full
                break;

//			udelay(40); // 10 ^ -4 seconds (enough for 1 byte to transmit)
//			fw_timer_event_DelayXus(10);
            // Fix possible crash in hdmi source when ac on/off. modified by weihao 03312010
            // Modify to use delay by software because of fw_timer_event_DelayXus using Interrupt.
            timercnt = 500;
            while (timercnt--);
        }

        count2 = 1000;
        // Send Read command
        *((UINT8*)&value1 + 2) |= 0x01;
        *((UINT8*)&value1 + 3) = 0;
        rtd_outl(i2c_addr1, value1);
        do
        {
            value3 = rtd_inl(i2c_addr3);	// BIT6: RAW_INTR_STAT_TX_ABRT
            if (*((UINT8*)&value3 + 3) & 0x40)
            {
                drv_i2c_Clear_all(i2c_master);
                RTD_Log(LOGGER_INFO, "drv_i2c_randomRead TX_ABRT\n");
                return _FALSE;
            }
            value2 = rtd_inl(i2c_addr2);
            if (*((UINT8*)&value2 + 3)  != 0)  		// RXFLR
            {
                value1 = rtd_inl(i2c_addr1);
                *read_buf = *((UINT8*)&value1 + 3);
                count = 1000;
                read_buf++;
                i++;
                break;
            }
        }
        while (count2-- > 0);

//		value3 = rtd_inl(i2c_addr3);	// BIT6: RAW_INTR_STAT_TX_ABRT
//		if(*((UINT8*)&value3 + 3) & 0x40) {
//			drv_i2c_Clear_all(i2c_master);
//			RTD_Log(LOGGER_INFO, "drv_i2c_randomRead TX_ABRT\n");
//			return _FALSE;
//		}

//		value2 = rtd_inl(i2c_addr2);
//		if(*((UINT8*)&value2 + 3)  != 0) {		// RXFLR
//			value1 = rtd_inl(i2c_addr1);
//			*read_buf = *((UINT8*)&value1 + 3);
//			count = 1000;
//			read_buf++;
//			i++;
// 		}
    }
    EA = 1;
    if (!count)
    {
        drv_i2c_Clear_all(i2c_master);
        RTD_Log(LOGGER_INFO, "drv_i2c_randomRead read error %x\n", (UINT32)i);
        return _FALSE;
    }
#else
    UINT32 io_base;
    UINT32 value, value1;
    int count,count1;
    int i;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = I2C_BASE_ADDRESS(i2c_master);

    /* wait while queue is full */
    // Check whether Tx buffer full or not
    count = 600;
    while (!(rtd_inl(io_base + I2C_IC_STATUS) & IC_STAT_TX_EMPTY) && (count-- > 0))
    {
        udelay(40); // 10 ^ -4 seconds (enough for 1 byte to transmit)
    }

    // Send Address Command/
    EA = 0;
    if (i2c_master == 0)
    {
        for (i = 0 ; i < w_len ; i++)
            rtd_outl(IC_DATA_CMD_0x2910, write_buf[i]);
    }
    else
    {
        for (i = 0 ; i < w_len ; i++)
            rtd_outl(IC1_DATA_CMD_0x2a10, write_buf[i]);
    }

    //
    // Send Read command & accept data from i2c
    i = 0;
    count = 600;
    value = rtd_inl(io_base + I2C_IC_DATA_CMD);
    value &= ~IC_DATA_CMD_DATA_MASK;
    value |= IC_DATA_CMD_READ_ENABLE;
    while (i < r_len && count)
    {
        if (i2c_master == 0)
        {
            count1 = 600;
            while (!(rtd_inl(IC_STATUS_0x2970) & IC_STAT_TX_FULL) && (count1-- > 0))
            {
                udelay(40); // 10 ^ -4 seconds (enough for 1 byte to transmit)
            }

            rtd_outl(IC_DATA_CMD_0x2910,value);
            if (rtd_inl(IC_RXFLR_0x2978) != 0)
            {
                value1 = rtd_inl(IC_DATA_CMD_0x2910);
                *read_buf = *((UINT8*)&value1 + 3);
                count = 600;
                read_buf++;
                i++;
            }
        }
        else
        {
            count1= 600;
            while (!(rtd_inl(IC1_STATUS_0x2a70) & IC_STAT_TX_FULL) && (count1-- > 0))
            {
                udelay(40); // 10 ^ -4 seconds (enough for 1 byte to transmit)
            }

            if (rtd_inl(IC1_RXFLR_0x2a78) != 0)
            {
                value1 = rtd_inl(IC1_DATA_CMD_0x2a10);
                *read_buf = *((UINT8*)&value1 + 3);
                count = 600;
                read_buf++;
                i++;
            }
        }
        count--;
    }
    EA = 1;
    if (!count)
    {
        drv_i2c_Clear_all(i2c_master);
        RTD_Log(LOGGER_INFO, "drv_i2c_randomRead read error %x\n", (UINT32)i);
        return _FALSE;
    }
#endif
    return _TRUE;
}

bit drv_i2c_currRead(UINT32 i2c_master,UINT8 r_len,UINT8* read_buf)
{
    UINT32 value=0;
    int i,j;
    UINT32 io_base;
    UINT8 count;

    if (i2c_master >= I2C_NUM)
        return 0;

    io_base = I2C_BASE_ADDRESS(i2c_master);

    j = 0;
    for (i=0; i<r_len; i++)
    {
        // wait while queue is full
        count = 20;
        while (!(rtd_inl(io_base+I2C_IC_STATUS) & IC_STAT_TX_FULL) && (count-- > 0))
        {
            udelay(20); // 10 ^ -4 seconds (enough for 1 byte to transmit)
        }

        value &= ~IC_DATA_CMD_DATA_MASK;
        value |= IC_DATA_CMD_READ_ENABLE;
        rtd_outl(io_base+I2C_IC_DATA_CMD,value);

        if (rtd_inl(io_base+I2C_IC_RAW_INTR_STAT)& RAW_INTR_STAT_TX_ABRT)
        {
            drv_i2c_Clear_all(i2c_master);
            return _FALSE;
        }

        if (rtd_inl(io_base+I2C_IC_RXFLR) != 0)
        {
            //udelay(20);
            *(read_buf+j) = (UINT8)(rtd_inl(io_base+I2C_IC_DATA_CMD) & 0xff);
            j++;
        }
    }

    i = 0;
    while (1)
    {
        if (j == r_len)
            break;

        udelay(20);
        if (rtd_inl(io_base+I2C_IC_RXFLR) != 0)
        {
            *(read_buf+j) = (UINT8)(rtd_inl(io_base+I2C_IC_DATA_CMD) & 0xff);
            j++;
        }

        // Time out
        i++;
        if (i>(r_len*200))
            break;
    }

    if (j != r_len)
    {
        drv_i2c_Clear_all(i2c_master);
        return _FALSE;
    }

    return _TRUE;
}

#ifdef CONFIG_TUNER_SILICON
bit drv_i2c_ContRead(UINT32 i2c_master,UINT8 r_len,UINT8* read_buf)
{

    UINT16 i2c_addr, i2c_addr1, i2c_addr2, i2c_addr3;
    UINT32 value, value1, value2, value3;
    UINT16 count,/*count1, */count2;//For Keil Building

    int i;
//	UINT8 j;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    /* wait while queue is empty */
    // Check whether Tx buffer empty or not
    i2c_addr = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr += I2C_IC_STATUS;
    count = 600;
    while (count-- > 0)
    {
        value = rtd_inl(i2c_addr);
        if (*((UINT8*)&value + 3) & 0x04) // BIT2: Transmit FIFO Completely Empty
            break;

        //  udelay(40); // 10 ^ -4 seconds (enough for 1 byte to transmit)
        //  fw_timer_event_DelayXms(3);
        fw_timer_event_DelayXus(20);
    }

    if (!count)
    {
        drv_i2c_Clear_all(i2c_master);
        RTD_Log(LOGGER_INFO, "drv_i2c_randomRead1 read error %x\n", (UINT32)i);
        return _FALSE;
    }

    // Send Read command & accept data from i2c
    i = 0;
    count = 600;

    // Send Address Command/
    i2c_addr1 = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr1 += I2C_IC_DATA_CMD;

    i2c_addr2 = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr2 += I2C_IC_RXFLR;

    i2c_addr3 = I2C_BASE_ADDRESS(i2c_master);
    i2c_addr3 += I2C_IC_RAW_INTR_STAT;
    value1 = rtd_inl(i2c_addr1);

//	i2c_addr4 = I2C_BASE_ADDRESS(i2c_master);
//	i2c_addr4 += I2C_IC_TX_TL;
    EA = 0;
// 	j = 0;
    *((UINT8*)&value1 + 2) |= 0x01;
    *((UINT8*)&value1 + 3) = 0;

    while (i < r_len)
    {
//  		count2 = 1000;
        // Send Read command
        rtd_outl(i2c_addr1, value1);
        i++;
    }
//		value4 = rtd_inl(i2c_addr4);
    i = 0;
    while (i < r_len && count)
    {
//		if(*((UINT8*)&value4 + 3) > 5) {
        do
        {
            value3 = rtd_inl(i2c_addr3); // BIT6: RAW_INTR_STAT_TX_ABRT
            if (*((UINT8*)&value3 + 3) & 0x40)
            {
                drv_i2c_Clear_all(i2c_master);
                RTD_Log(LOGGER_INFO, "drv_i2c_randomRead TX_ABRT\n");
                return _FALSE;
            }
            value2 = rtd_inl(i2c_addr2);
            if (*((UINT8*)&value2 + 3)  != 0)   // RXFLR
            {
                value1 = rtd_inl(i2c_addr1);
                *read_buf = *((UINT8*)&value1 + 3);
                count = 1000;
                read_buf++;
                i++;
//					 j--;
                break;
            }
        }
        while (count2-- > 0);
        count--;
//		}
//  value3 = rtd_inl(i2c_addr3); // BIT6: RAW_INTR_STAT_TX_ABRT
//  if(*((UINT8*)&value3 + 3) & 0x40) {
//   drv_i2c_Clear_all(i2c_master);
//   RTD_Log(LOGGER_INFO, "drv_i2c_randomRead TX_ABRT\n");
//   return _FALSE;
//  }

//  value2 = rtd_inl(i2c_addr2);
//  if(*((UINT8*)&value2 + 3)  != 0) {  // RXFLR
//   value1 = rtd_inl(i2c_addr1);
//   *read_buf = *((UINT8*)&value1 + 3);
//   count = 1000;
//   read_buf++;
//   i++;
//   }
    }
    EA = 1;
    if (!count)
    {
        drv_i2c_Clear_all(i2c_master);
        RTD_Log(LOGGER_INFO, "drv_i2c_randomRead read error %x\n", (UINT32)i);
        return _FALSE;
    }

    return _TRUE;
}
#endif

// STRUCT_I2C_PARAMETER code stI2CParameter[CHIP_I2C_TOTAL] =
void drv_i2c_Init(void)
{
    int i;
    STRUCT_I2C_PARAMETER* ptI2CParameter;
    UINT16 offset;

    for (i = 0; i < CHIP_I2C_TOTAL; i++)
    {
        ptI2CParameter = &stI2CParameter[i];

        if (ptI2CParameter->enable)
        {
            offset = I2C_BASE_ADDRESS(i);
            // Disable all mask interrupt
            rtd_outl(offset + I2C_IC_INTR_MASK, RAW_INTR_MASK_CLEAR_ALL);
            drv_i2c_Set_speed(i, ptI2CParameter->speed);
            drv_i2c_Set_LocalAddr(i, ptI2CParameter->addr);
            drv_i2c_Set_AddressType(i, ptI2CParameter->addrtype);
            drv_i2c_Set_MasterMode(i, ptI2CParameter->mode);
            drv_i2c_Set_rx_tx_tl(i, ptI2CParameter->rx_tl, ptI2CParameter->tx_tl);
            // Enable i2c
            rtdf_setBits(offset + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        }
    }
    RTD_Log(LOGGER_INFO, "Initialize I2c\n");
}

void drv_i2c_PowerOn(bit on_off)
{
    int i;
    UINT16 offset;
    if (on_off)
    {
    }
    else
    {
        for (i = 0; i < CHIP_I2C_TOTAL; i++)
        {
            offset = I2C_BASE_ADDRESS(i);

            // Disable i2c
            rtdf_clearBits(offset + I2C_IC_ENABLE, IC_ENABLE_ENABLE);
        }
    }
}


#ifdef CONFIG_IFD_TUNER_AUTO

bit drv_i2c_isr_Set_TargetAddr(UINT8 i2c_master, UINT16 Target_addr)
{
    UINT16 io_base;
    UINT8 fEnable;
    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = (IC_CON_0x2900 + ((UINT16)i2c_master << 8));
    fEnable = 0;

    /* First disable IC  */
    if (rtd_isr_inl(io_base + I2C_IC_ENABLE) & IC_ENABLE_ENABLE)
    {
        rtd_isr_maskl(io_base + I2C_IC_ENABLE, ~((UINT32)IC_ENABLE_ENABLE), (UINT32)0);
        fEnable = 1;
    }

    rtd_isr_maskl(io_base + I2C_IC_TAR, ~MIS_IC_TAR_MASK, Target_addr);

    /* last enable IC */
    if (fEnable)
        rtd_isr_maskl(io_base + I2C_IC_ENABLE, ~((UINT32)IC_ENABLE_ENABLE), (UINT32)IC_ENABLE_ENABLE);

    return _TRUE;
}

bit drv_i2c_isr_Write(UINT8 i2c_master, UINT8 w_len, UINT8* write_buf)
{
    int i;
    UINT32 value=0;
    UINT16 io_base;
    UINT8 count;
//        UINT8 getVal;

    if (i2c_master >= I2C_NUM)
        return _FALSE;

    io_base = (IC_CON_0x2900 + ((UINT16)i2c_master << 8));

    value = rtd_isr_inl(io_base + I2C_IC_DATA_CMD);
    value &= ~IC_DATA_CMD_DATA_MASK;
    value &= IC_DATA_CMD_WRITE_ENABLE;

    /* wait while queue is full */
    // Check whether Tx buffer full or not
    count = 200;
    while (!(rtd_isr_inl(io_base+I2C_IC_STATUS) & IC_STAT_TX_FULL) && (count-- > 0))
    {
        for (i=0; i<540; i++) ;//udelay(20); // 10 ^ -4 seconds (enough for 1 byte to transmit)
    }

    if (i2c_master == 0)
    {
        EA = 0;
        for (i = 0 ; i < w_len ; i++)
        {
            rtd_isr_outl(IC_DATA_CMD_0x2910, write_buf[i]);
        }
        EA = 1;
    }
    else
    {
        EA = 0;
        for (i = 0 ; i < w_len ; i++)
        {
            rtd_isr_outl(IC1_DATA_CMD_0x2a10, write_buf[i]);

        }
        EA = 1;
    }

    // Check whether the command be able to run or not
    if (rtd_isr_inl(io_base + I2C_IC_RAW_INTR_STAT)& RAW_INTR_STAT_TX_ABRT)
    {
        rtd_isr_inl(io_base+I2C_IC_CLR_INTR);
        rtd_isr_inl(io_base+I2C_IC_CLR_RX_UNDER);
        rtd_isr_inl(io_base+I2C_IC_CLR_RX_OVER);
        rtd_isr_inl(io_base+I2C_IC_CLR_TX_OVER);
        rtd_isr_inl(io_base+I2C_IC_CLR_RD_REQ);
        rtd_isr_inl(io_base+I2C_IC_CLR_TX_ABRT);
        rtd_isr_inl(io_base+I2C_IC_CLR_RX_DONE);
        rtd_isr_inl(io_base+I2C_IC_CLR_ACTIVITY);
        rtd_isr_inl(io_base+I2C_IC_CLR_STOP_DET);
        rtd_isr_inl(io_base+I2C_IC_CLR_START_DET);
        rtd_isr_inl(io_base+I2C_IC_CLR_GEN_CALL);
        return _FALSE;
    }

    for (i = 0; i < w_len + 5; i++)
    {
        for (i=0; i<2700; i++) ;//udelay(100);
        value = rtd_isr_inl(io_base + I2C_IC_RAW_INTR_STAT);
        if (value & RAW_INTR_STAT_TX_ABRT)
        {
            rtd_isr_inl(io_base+I2C_IC_CLR_INTR);
            rtd_isr_inl(io_base+I2C_IC_CLR_RX_UNDER);
            rtd_isr_inl(io_base+I2C_IC_CLR_RX_OVER);
            rtd_isr_inl(io_base+I2C_IC_CLR_TX_OVER);
            rtd_isr_inl(io_base+I2C_IC_CLR_RD_REQ);
            rtd_isr_inl(io_base+I2C_IC_CLR_TX_ABRT);
            rtd_isr_inl(io_base+I2C_IC_CLR_RX_DONE);
            rtd_isr_inl(io_base+I2C_IC_CLR_ACTIVITY);
            rtd_isr_inl(io_base+I2C_IC_CLR_STOP_DET);
            rtd_isr_inl(io_base+I2C_IC_CLR_START_DET);
            rtd_isr_inl(io_base+I2C_IC_CLR_GEN_CALL);
            return _FALSE;
        }

        if (value & RAW_INTR_STAT_TX_EMPTY)
            break;
    }

    return _TRUE;
}
#endif



void Delay5ms(UINT16 time)												  
{
	UINT16 i,j;
	for(j = 0; i < time; j++)
		for(i = 0; i < 1700; i++)
			;
}

#if CONFIG_ENABLE_FM
#define I2C_DEV0_ADDRESS 0x58
UINT8   qnd_i2c;

void QND_I2C_WRITE(UINT8 ucSubAdr,UINT8 ucVal)
{
	UINT8 writedata[3];

	drv_i2c_Set_TargetAddr(CHIP_I2C1, (I2C_DEV0_ADDRESS >> 1));

	writedata[0] = ucSubAdr;
	writedata[1] = ucVal;
    drv_i2c_Write(CHIP_I2C1, 2, &writedata);        
}

UINT8 QND_I2C_READ(UINT8 ucSubAdr)
{
    UINT8 ucBuf; // byte buffer
    
    drv_i2c_Set_TargetAddr(CHIP_I2C1, (I2C_DEV0_ADDRESS >> 1) );
    ucBuf = ucSubAdr;
    drv_i2c_Write(CHIP_I2C1, 1,&ucBuf);

    
    drv_i2c_currRead(CHIP_I2C1, 1, &ucBuf);
	
    return ucBuf;
}

void QND_WriteReg(UINT8 Regis_Addr,UINT8 Data)
{
     QND_I2C_WRITE(Regis_Addr,Data);
}


UINT8 QND_ReadReg(UINT8 Regis_Addr)
{
    UINT8  Data;
	
    Data = QND_I2C_READ(Regis_Addr);

    return Data;
}
#if 1//FM_CONTROL
#if 1
#define FREQ2CHREG(freq)   ((freq-7600)/5)
#define CHREG2FREQ(ch)     (ch*5+7600)
#define BAND_FM        0
  
#define CH            0x01
#define CH_STEP        0x00
#define CH_CH       0x03
UINT8    qnd_Band;




/**********************************************************************
void QNF_SetRegBit(UINT8 reg, UINT8 bitMask, UINT8 data_val)
**********************************************************************
Description: set register specified bit

Parameters:
reg:        register that will be set
bitMask:    mask specified bit of register
data_val:    data will be set for specified bit
Return Value:
None
**********************************************************************/
void QNF_SetRegBit(UINT8 reg, UINT8 bitMask, UINT8 data_val) 
{
    UINT8  temp;
    temp = QND_ReadReg(reg);
    temp &= (UINT8)(~bitMask);
    temp |= data_val;
    QND_WriteReg(reg, temp);
}


/**********************************************************************
void QNF_SetCh(UINT16 freq)
**********************************************************************
Description: set specified frequency 

Parameters:
        freq:  specified frequency that is set
Return Value:
         1: set success
**********************************************************************/
void QNF_SetCh(UINT16 freq) 
{
    // calculate ch para
    UINT8  tStep;
    UINT8  tCh;
    UINT16  f; 
    f = FREQ2CHREG(freq); 
    // set to reg: CH
    tCh = (UINT8) f;
    QND_WriteReg(CH, tCh);
    // set to reg: CH_STEP
    tStep = QND_ReadReg(CH_STEP);
    tStep &= ~CH_CH;
    tStep |= ((UINT8) (f >> 8) & CH_CH);
    QND_WriteReg(CH_STEP, tStep);
}

/**********************************************************************
UINT8 QND_GetRSSI(UINT16 ch)
**********************************************************************
Description:    Get the RSSI value, call QND_TuneToCH  to set specify CH
Parameters:
Return Value:
RSSI value  of the channel setted
**********************************************************************/
void QN_ChipInitialization()
{
    QND_WriteReg(0x00,0x80);// reset all registers to the default value
    Delay5ms(7);//Delay(30);
     //to be customized: change the crystal setting(register 0x03 and 0x04) according to HW design
    QNF_SetRegBit(0x00,0x40,0x40);//recalibration
    QNF_SetRegBit(0x00,0x40,0x00);//recalibration
    Delay5ms(7);//Delay(30);                //delay 20 ms
    QND_WriteReg(0x18,0x64);        //SNR improve
    QND_WriteReg(0x1b,0xf0);    //Increase RF power output
     //QND_WriteReg(0x02,0xb9);    //close PA AUTO off
    QND_WriteReg(0x00,0x21);    //SET TX mode
    //QNF_SetRegBit(0x02,0x10,0x10);//recalibration   
    //QNF_SetRegBit(0x02,0x20,0x20);//recalibration
    //Done chip recalibration
}

/**********************************************************************
int QND_Init()
**********************************************************************
Description: Initialize device to make it ready to have all functionality ready for use.

Parameters:
None
Return Value:
1: Device is ready to use.
0: Device is not ready to serve function.
**********************************************************************/
void QND_Init() 
{
    QN_ChipInitialization();
    //QND_WriteReg(0x00,  0x01); //resume original status of chip /* 2008 06 13 */

    qnd_Band = BAND_FM;
}

/**********************************************************************
void QND_TuneToCH(UINT16 ch)
**********************************************************************
Description:    Tune to the specified channel. Before use this function
make sure the QND_SetSysMode() has been called. If system is working 
on FM&TX mode,it will turn FM to ch, and start Transmit; If system is
working on FM&RX mode,it will turn FM to ch, and start FM play.
Parameters:
ch
Set the frequency (10kHz) to be tuned,
eg: 101.30MHz will be set to 10130.
Return Value:
    None
**********************************************************************/
void QND_TuneToCH(UINT16 ch) 
{
    QNF_SetCh(ch);
    Delay5ms(7);//Delay(20);
    QNF_SetRegBit(0x00,0x20,0x20);
}
void QND_Tuneoff(void)
{
	QNF_SetRegBit(0x00,0x20,0x00);
}
#endif

#if (!ENABLE_FM_FREQSET)
UINT16 code FM_FREQ[9]={0,8750,8790,8810,8830,8850,8870,8890,8910};
#endif


BOOL AdjustFMfun(UINT16 action)
{
#if ENABLE_FM_FREQSET
	if(action)
	{
		QND_TuneToCH(7600+(action-1)*10);
	}
	else
	{
		QND_Tuneoff();
	}
	return TRUE;

#else
	if(action)
	{
	   QND_TuneToCH(FM_FREQ[action]);
	   
	}
	else
	{
		QND_Tuneoff();
	}
	return TRUE;
#endif
}

#endif

#endif


#endif
