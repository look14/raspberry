/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    cmt2300a.c
 * @brief   CMT2300A transceiver RF chip driver
 *
 * @version 1.3
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */

#include "cmt2300a.h"

/*! ********************************************************
* @name    CMT2300A_SoftReset
* @desc    Soft reset.
* *********************************************************/
void CMT2300A_SoftReset(u8 index)
{
    CMT2300A_WriteReg(index, 0x7F, 0xFF);
}

/*! ********************************************************
* @name    CMT2300A_GetChipStatus
* @desc    Get the chip status.
* @return
*          CMT2300A_STA_PUP
*          CMT2300A_STA_SLEEP
*          CMT2300A_STA_STBY
*          CMT2300A_STA_RFS
*          CMT2300A_STA_TFS
*          CMT2300A_STA_RX
*          CMT2300A_STA_TX
*          CMT2300A_STA_EEPROM
*          CMT2300A_STA_ERROR
*          CMT2300A_STA_CAL
* *********************************************************/
u8 CMT2300A_GetChipStatus(u8 index)
{
    return  CMT2300A_ReadReg(index, CMT2300A_CUS_MODE_STA) & CMT2300A_MASK_CHIP_MODE_STA;
}

/*! ********************************************************
* @name    CMT2300A_AutoSwitchStatus
* @desc    Auto switch the chip status, and 10 ms as timeout.
* @param   nGoCmd: the chip next status
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_AutoSwitchStatus(u8 index, u8 nGoCmd)
{
#ifdef ENABLE_AUTO_SWITCH_CHIP_STATUS
    u32 nBegTick = CMT2300A_GetTickCount();
    u8  nWaitStatus;
    
    switch(nGoCmd)
    {
    case CMT2300A_GO_SLEEP: nWaitStatus = CMT2300A_STA_SLEEP; break;
    case CMT2300A_GO_STBY : nWaitStatus = CMT2300A_STA_STBY ; break;
    case CMT2300A_GO_TFS  : nWaitStatus = CMT2300A_STA_TFS  ; break;
    case CMT2300A_GO_TX   : nWaitStatus = CMT2300A_STA_TX   ; break;
    case CMT2300A_GO_RFS  : nWaitStatus = CMT2300A_STA_RFS  ; break;
    case CMT2300A_GO_RX   : nWaitStatus = CMT2300A_STA_RX   ; break;
    }
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_MODE_CTL, nGoCmd);

    while(CMT2300A_GetTickCount()-nBegTick < 10)
    {
        //CMT2300A_DelayUs(100);
        
        if(nWaitStatus==CMT2300A_GetChipStatus(index))
            return TRUE;
        
        if(CMT2300A_GO_TX==nGoCmd) {
            //CMT2300A_DelayUs(100);
            
            if(CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(index, CMT2300A_CUS_INT_CLR1))
                return TRUE;
        }
        
        if(CMT2300A_GO_RX==nGoCmd) {
            //CMT2300A_DelayUs(100);
            
            if(CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(index, CMT2300A_CUS_INT_FLAG))
                return TRUE;
        }
    }
    printf("CMT2300A_AutoSwitchStatus(%d, %X:FALSE)\n", index, nGoCmd);
    return FALSE;
    
#else
    CMT2300A_WriteReg(index, CMT2300A_CUS_MODE_CTL, nGoCmd);
    return TRUE;
#endif
}

/*! ********************************************************
* @name    CMT2300A_GoSleep
* @desc    Entry SLEEP mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoSleep(u8 index)
{
    return CMT2300A_AutoSwitchStatus(index, CMT2300A_GO_SLEEP);
}

/*! ********************************************************
* @name    CMT2300A_GoStby
* @desc    Entry Sleep mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoStby(u8 index)
{
    return CMT2300A_AutoSwitchStatus(index, CMT2300A_GO_STBY);
}

/*! ********************************************************
* @name    CMT2300A_GoTFS
* @desc    Entry TFS mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoTFS(u8 index)
{
    return CMT2300A_AutoSwitchStatus(index, CMT2300A_GO_TFS);
}

/*! ********************************************************
* @name    CMT2300A_GoRFS
* @desc    Entry RFS mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoRFS(u8 index)
{
    return CMT2300A_AutoSwitchStatus(index, CMT2300A_GO_RFS);
}

/*! ********************************************************
* @name    CMT2300A_GoTx
* @desc    Entry Tx mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoTx(u8 index)
{
    return CMT2300A_AutoSwitchStatus(index, CMT2300A_GO_TX);
}

/*! ********************************************************
* @name    CMT2300A_GoRx
* @desc    Entry Rx mode.
* @return  TRUE or FALSE
* *********************************************************/
BOOL CMT2300A_GoRx(u8 index)
{
    return CMT2300A_AutoSwitchStatus(index, CMT2300A_GO_RX);
}

/*! ********************************************************
* @name    CMT2300A_ConfigGpio
* @desc    Config GPIO pins mode.
* @param   nGpioSel: GPIO1_SEL | GPIO2_SEL | GPIO3_SEL | GPIO4_SEL
*          GPIO1_SEL:
*            CMT2300A_GPIO1_SEL_DOUT/DIN 
*            CMT2300A_GPIO1_SEL_INT1
*            CMT2300A_GPIO1_SEL_INT2 
*            CMT2300A_GPIO1_SEL_DCLK
*
*          GPIO2_SEL:
*            CMT2300A_GPIO2_SEL_INT1 
*            CMT2300A_GPIO2_SEL_INT2
*            CMT2300A_GPIO2_SEL_DOUT/DIN 
*            CMT2300A_GPIO2_SEL_DCLK
*
*          GPIO3_SEL:
*            CMT2300A_GPIO3_SEL_CLKO 
*            CMT2300A_GPIO3_SEL_DOUT/DIN
*            CMT2300A_GPIO3_SEL_INT2 
*            CMT2300A_GPIO3_SEL_DCLK
*
*          GPIO4_SEL:
*            CMT2300A_GPIO4_SEL_RSTIN 
*            CMT2300A_GPIO4_SEL_INT1
*            CMT2300A_GPIO4_SEL_DOUT 
*            CMT2300A_GPIO4_SEL_DCLK
* *********************************************************/
void CMT2300A_ConfigGpio(u8 index, u8 nGpioSel)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_IO_SEL, nGpioSel);
}

/*! ********************************************************
* @name    CMT2300A_ConfigInterrupt
* @desc    Config interrupt on INT1 and INT2.
* @param   nInt1Sel, nInt2Sel
*            CMT2300A_INT_SEL_RX_ACTIVE
*            CMT2300A_INT_SEL_TX_ACTIVE
*            CMT2300A_INT_SEL_RSSI_VLD
*            CMT2300A_INT_SEL_PREAM_OK
*            CMT2300A_INT_SEL_SYNC_OK
*            CMT2300A_INT_SEL_NODE_OK
*            CMT2300A_INT_SEL_CRC_OK
*            CMT2300A_INT_SEL_PKT_OK
*            CMT2300A_INT_SEL_SL_TMO
*            CMT2300A_INT_SEL_RX_TMO
*            CMT2300A_INT_SEL_TX_DONE
*            CMT2300A_INT_SEL_RX_FIFO_NMTY
*            CMT2300A_INT_SEL_RX_FIFO_TH
*            CMT2300A_INT_SEL_RX_FIFO_FULL
*            CMT2300A_INT_SEL_RX_FIFO_WBYTE
*            CMT2300A_INT_SEL_RX_FIFO_OVF
*            CMT2300A_INT_SEL_TX_FIFO_NMTY
*            CMT2300A_INT_SEL_TX_FIFO_TH
*            CMT2300A_INT_SEL_TX_FIFO_FULL
*            CMT2300A_INT_SEL_STATE_IS_STBY
*            CMT2300A_INT_SEL_STATE_IS_FS
*            CMT2300A_INT_SEL_STATE_IS_RX
*            CMT2300A_INT_SEL_STATE_IS_TX
*            CMT2300A_INT_SEL_LED
*            CMT2300A_INT_SEL_TRX_ACTIVE
*            CMT2300A_INT_SEL_PKT_DONE
* *********************************************************/
void CMT2300A_ConfigInterrupt(u8 index, u8 nInt1Sel, u8 nInt2Sel)
{
    u8 tmp;
    
    nInt1Sel &= CMT2300A_MASK_INT1_SEL;
    nInt2Sel &= CMT2300A_MASK_INT2_SEL;
    
    tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_INT1_CTL);
    if(nInt1Sel != (CMT2300A_MASK_INT1_SEL & tmp)) {
        nInt1Sel |= (~CMT2300A_MASK_INT1_SEL) & tmp;
        CMT2300A_WriteReg(index, CMT2300A_CUS_INT1_CTL, nInt1Sel);
    }

    tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_INT2_CTL);
    if(nInt2Sel != (CMT2300A_MASK_INT2_SEL & tmp)) {
        nInt2Sel |= (~CMT2300A_MASK_INT2_SEL) & tmp;
        CMT2300A_WriteReg(index, CMT2300A_CUS_INT2_CTL, nInt2Sel);
    }
}

/*! ********************************************************
* @name    CMT2300A_SetInterruptPolar
* @desc    Set the polarity of the interrupt.
* @param   bEnable(TRUE): active-high (default)
*          bEnable(FALSE): active-low
* *********************************************************/
void CMT2300A_SetInterruptPolar(u8 index, BOOL bActiveHigh)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_INT1_CTL);

    if(bActiveHigh)
        tmp &= ~CMT2300A_MASK_INT_POLAR;
    else
        tmp |= CMT2300A_MASK_INT_POLAR;

    CMT2300A_WriteReg(index, CMT2300A_CUS_INT1_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_SetFifoThreshold
* @desc    Set FIFO threshold.
* @param   nFifoThreshold
* *********************************************************/
void CMT2300A_SetFifoThreshold(u8 index, u8 nFifoThreshold)
{ 
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_PKT29);
    
    tmp &= ~CMT2300A_MASK_FIFO_TH;
    tmp |= nFifoThreshold & CMT2300A_MASK_FIFO_TH;
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_PKT29, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableAntennaSwitch
* @desc    Enable antenna switch, output TX_ACTIVE/RX_ACTIVE
*          via GPIO1/GPIO2.
* @param   nMode 
*            0: RF_SWT1_EN=1, RF_SWT2_EN=0
*               GPIO1: RX_ACTIVE, GPIO2: TX_ACTIVE
*            1: RF_SWT1_EN=0, RF_SWT2_EN=1
*               GPIO1: RX_ACTIVE, GPIO2: ~RX_ACTIVE
* *********************************************************/
void CMT2300A_EnableAntennaSwitch(u8 index, u8 nMode)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_INT1_CTL);

    if(0 == nMode) {
        tmp |= CMT2300A_MASK_RF_SWT1_EN;
        tmp &= ~CMT2300A_MASK_RF_SWT2_EN;
    }
    else if(1 == nMode) {
        tmp &= ~CMT2300A_MASK_RF_SWT1_EN;
        tmp |= CMT2300A_MASK_RF_SWT2_EN;
    }

    CMT2300A_WriteReg(index, CMT2300A_CUS_INT1_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableInterrupt
* @desc    Enable interrupt.
* @param   nEnable 
*            CMT2300A_MASK_SL_TMO_EN   |
*            CMT2300A_MASK_RX_TMO_EN   |
*            CMT2300A_MASK_TX_DONE_EN  |
*            CMT2300A_MASK_PREAM_OK_EN |
*            CMT2300A_MASK_SYNC_OK_EN  |
*            CMT2300A_MASK_NODE_OK_EN  |
*            CMT2300A_MASK_CRC_OK_EN   |
*            CMT2300A_MASK_PKT_DONE_EN
* *********************************************************/
void CMT2300A_EnableInterrupt(u8 index, u8 nEnable)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_INT_EN, nEnable);
}

/*! ********************************************************
* @name    CMT2300A_EnableRxFifoAutoClear
* @desc    Auto clear Rx FIFO before entry Rx mode.
* @param   bEnable(TRUE): Enable it(default)
*          bEnable(FALSE): Disable it
* *********************************************************/
void CMT2300A_EnableRxFifoAutoClear(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_FIFO_CTL);

    if(bEnable)
        tmp &= ~CMT2300A_MASK_FIFO_AUTO_CLR_DIS;
    else
        tmp |= CMT2300A_MASK_FIFO_AUTO_CLR_DIS;

    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableFifoMerge
* @desc    Enable FIFO merge.
* @param   bEnable(TRUE): use a single 64-byte FIFO for either Tx or Rx
*          bEnable(FALSE): use a 32-byte FIFO for Tx and another 32-byte FIFO for Rx(default)
* *********************************************************/
void CMT2300A_EnableFifoMerge(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_FIFO_CTL);

    if(bEnable)
        tmp |= CMT2300A_MASK_FIFO_MERGE_EN;
    else
        tmp &= ~CMT2300A_MASK_FIFO_MERGE_EN;

    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableReadFifo
* @desc    Enable SPI to read the FIFO.
* *********************************************************/
void CMT2300A_EnableReadFifo(u8 index)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_FIFO_CTL);
    u8 mask = CMT2300A_MASK_SPI_FIFO_RD_WR_SEL | CMT2300A_MASK_FIFO_RX_TX_SEL;
    
    if(mask & tmp) {
        tmp &= ~mask;
        CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CTL, tmp);
    }
}

/*! ********************************************************
* @name    CMT2300A_EnableWriteFifo
* @desc    Enable SPI to write the FIFO.
* *********************************************************/
void CMT2300A_EnableWriteFifo(u8 index)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_FIFO_CTL);
    tmp |= CMT2300A_MASK_SPI_FIFO_RD_WR_SEL;
    tmp |= CMT2300A_MASK_FIFO_RX_TX_SEL;
    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_RestoreFifo
* @desc    Restore the FIFO.
* *********************************************************/
void CMT2300A_RestoreFifo(u8 index)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CLR, CMT2300A_MASK_FIFO_RESTORE);
}

/*! ********************************************************
* @name    CMT2300A_ClearFifo
* @desc    Clear the Tx FIFO.
* @return  FIFO flags
*            CMT2300A_MASK_RX_FIFO_FULL_FLG |
*            CMT2300A_MASK_RX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_RX_FIFO_TH_FLG   |
*            CMT2300A_MASK_RX_FIFO_OVF_FLG  |
*            CMT2300A_MASK_TX_FIFO_FULL_FLG |
*            CMT2300A_MASK_TX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_TX_FIFO_TH_FLG
* *********************************************************/
void CMT2300A_ClearTxFifo(u8 index)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CLR, CMT2300A_MASK_FIFO_CLR_TX);
}

/*! ********************************************************
* @name    CMT2300A_ClearFifo
* @desc    Clear the Rx FIFO.
* @return  FIFO flags
*            CMT2300A_MASK_RX_FIFO_FULL_FLG |
*            CMT2300A_MASK_RX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_RX_FIFO_TH_FLG   |
*            CMT2300A_MASK_RX_FIFO_OVF_FLG  |
*            CMT2300A_MASK_TX_FIFO_FULL_FLG |
*            CMT2300A_MASK_TX_FIFO_NMTY_FLG |
*            CMT2300A_MASK_TX_FIFO_TH_FLG
* *********************************************************/
void CMT2300A_ClearRxFifo(u8 index)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CLR, CMT2300A_MASK_FIFO_CLR_RX);
}

/*! ********************************************************
* @name    CMT2300A_ClearInterruptFlags
* @desc    Clear all interrupt flags.
* *********************************************************/
void CMT2300A_ClearInterruptFlags(u8 index)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_INT_CLR1, 0xFF);
    CMT2300A_WriteReg(index, CMT2300A_CUS_INT_CLR2, 0xFF);
}

/*! ********************************************************
* @name    CMT2300A_ConfigTxDin
* @desc    Used to select whether to use GPIO1 or GPIO2 or GPIO3
*          as DIN in the direct mode. It only takes effect when 
*          call CMT2300A_EnableTxDin(TRUE) in the direct mode.
* @param   nDinSel
*            CMT2300A_TX_DIN_SEL_GPIO1
*            CMT2300A_TX_DIN_SEL_GPIO2
*            CMT2300A_TX_DIN_SEL_GPIO3
* *********************************************************/
void CMT2300A_ConfigTxDin(u8 index, u8 nDinSel)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_FIFO_CTL);
    tmp &= ~CMT2300A_MASK_TX_DIN_SEL;
    tmp |= nDinSel;
    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableTxDin
* @desc    Used to change GPIO1/GPIO2/GPIO3 between DOUT and DIN.
* @param   bEnable(TRUE): used as DIN
*          bEnable(FALSE): used as DOUT(default)
* *********************************************************/
void CMT2300A_EnableTxDin(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_FIFO_CTL);

    if(bEnable)
        tmp |= CMT2300A_MASK_TX_DIN_EN;
    else
        tmp &= ~CMT2300A_MASK_TX_DIN_EN;

    CMT2300A_WriteReg(index, CMT2300A_CUS_FIFO_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableTxDinInvert
* @desc    Used to invert DIN data in direct mode.
* @param   bEnable(TRUE): invert DIN
*          bEnable(FALSE): not invert DIN(default)
* *********************************************************/
void CMT2300A_EnableTxDinInvert(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_INT2_CTL);

    if(bEnable)
        tmp |= CMT2300A_MASK_TX_DIN_INV;
    else
        tmp &= ~CMT2300A_MASK_TX_DIN_INV;

    CMT2300A_WriteReg(index, CMT2300A_CUS_INT2_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_IsExist
* @desc    Chip indentify.
* @return  TRUE: chip is exist, FALSE: chip not found
* *********************************************************/
BOOL CMT2300A_IsExist(u8 index)
{
    u8 back, dat;

    back = CMT2300A_ReadReg(index, CMT2300A_CUS_PKT17);
    CMT2300A_WriteReg(index, CMT2300A_CUS_PKT17, 0xAA);

    dat = CMT2300A_ReadReg(index, CMT2300A_CUS_PKT17);
    CMT2300A_WriteReg(index, CMT2300A_CUS_PKT17, back);

    if(0xAA==dat)
        return TRUE;
    else
        return FALSE;
}

/*! ********************************************************
* @name    CMT2300A_GetRssiCode
* @desc    Get RSSI code.
* @return  RSSI code
* *********************************************************/
u8 CMT2300A_GetRssiCode(u8 index)
{
    return CMT2300A_ReadReg(index, CMT2300A_CUS_RSSI_CODE);
}

/*! ********************************************************
* @name    CMT2300A_GetRssiDBm
* @desc    Get RSSI dBm.
* @return  dBm
* *********************************************************/
short CMT2300A_GetRssiDBm(u8 index)
{
    return (short)CMT2300A_ReadReg(index, CMT2300A_CUS_RSSI_DBM) - 128;
}

/*! ********************************************************
* @name    CMT2300A_SetFrequencyChannel
* @desc    This defines up to 255 frequency channel
*          for fast frequency hopping operation.
* @param   nChann: the frequency channel
* *********************************************************/
void CMT2300A_SetFrequencyChannel(u8 index, u8 nChann)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_FREQ_CHNL, nChann);
}

/*! ********************************************************
* @name    CMT2300A_SetFrequencyStep
* @desc    This defines the frequency channel step size 
*          for fast frequency hopping operation. 
*          One step size is 2.5 kHz.
* @param   nOffset: the frequency step
* *********************************************************/
void CMT2300A_SetFrequencyStep(u8 index, u8 nOffset)
{
    CMT2300A_WriteReg(index, CMT2300A_CUS_FREQ_OFS, nOffset);
}

/*! ********************************************************
* @name    CMT2300A_SetPayloadLength
* @desc    Set payload length.
* @param   nLength
* *********************************************************/
void CMT2300A_SetPayloadLength(u8 index, u16 nLength)
{ 
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_PKT14);
    
    tmp &= ~CMT2300A_MASK_PAYLOAD_LENG_10_8;
    tmp |= (nLength >> 4) & CMT2300A_MASK_PAYLOAD_LENG_10_8;
    CMT2300A_WriteReg(index, CMT2300A_CUS_PKT14, tmp);
    
    tmp = nLength & CMT2300A_MASK_PAYLOAD_LENG_7_0;
    CMT2300A_WriteReg(index, CMT2300A_CUS_PKT15, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableLfosc
* @desc    If you need use sleep timer, you should enable LFOSC.
* @param   bEnable(TRUE): Enable it(default)
*          bEnable(FALSE): Disable it
* *********************************************************/
void CMT2300A_EnableLfosc(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_SYS2);
    
    if(bEnable) {
        tmp |= CMT2300A_MASK_LFOSC_RECAL_EN;
        tmp |= CMT2300A_MASK_LFOSC_CAL1_EN;
        tmp |= CMT2300A_MASK_LFOSC_CAL2_EN;
    }
    else {
        tmp &= ~CMT2300A_MASK_LFOSC_RECAL_EN;
        tmp &= ~CMT2300A_MASK_LFOSC_CAL1_EN;
        tmp &= ~CMT2300A_MASK_LFOSC_CAL2_EN;
    }
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_SYS2, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableLfoscOutput
* @desc    LFOSC clock is output via GPIO3.
* @param   bEnable(TRUE): Enable it
*          bEnable(FALSE): Disable it(default)
* *********************************************************/
void CMT2300A_EnableLfoscOutput(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_INT2_CTL);
    
    if(bEnable)
        tmp |= CMT2300A_MASK_LFOSC_OUT_EN;
    else
        tmp &= ~CMT2300A_MASK_LFOSC_OUT_EN;
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_INT2_CTL, tmp);
}

/*! ********************************************************
* @name    CMT2300A_EnableErrorStop
* @desc    If the CMT2300A fails, it stays in the ERROR state.
* @param   bEnable(TRUE): Enable it
*          bEnable(FALSE): Disable it(default)
* *********************************************************/
void CMT2300A_EnableErrorStop(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_EN_CTL);
    
    if(bEnable)
        tmp |= CMT2300A_MASK_ERROR_STOP_EN;
    else
        tmp &= ~CMT2300A_MASK_ERROR_STOP_EN;
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_EN_CTL, tmp);
}

void CMT2300A_SetTxPower(u8 index, u8 nPowerCode, u8 nPCodeAdj)
{
    u8 tmp;
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_TX8, nPowerCode);
    
    tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_TX9);
    tmp &= ~0x1F;
    tmp |= nPCodeAdj & 0x1F;
    CMT2300A_WriteReg(index, CMT2300A_CUS_TX9, tmp);
}

void CMT2300A_SetPacketMode(u8 index, u8 nPacketMode)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_PKT1);
    
    tmp &= ~CMT2300A_MASK_DATA_MODE;
    tmp |= nPacketMode & CMT2300A_MASK_DATA_MODE;
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_PKT1, tmp);
}

void CMT2300A_SetPacketType(u8 index, u8 nPacketType)
{
    u8 tmp;
    
    tmp  = CMT2300A_ReadReg(index, CMT2300A_CUS_PKT14);
    tmp &= ~CMT2300A_MASK_PKT_TYPE;
    tmp |= nPacketType & CMT2300A_MASK_PKT_TYPE;
    CMT2300A_WriteReg(index, CMT2300A_CUS_PKT14, tmp);
}

void CMT2300A_EnableRxAutoExit(u8 index, BOOL bEnable)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_SYS10);
    
    if(bEnable)
        tmp &= ~CMT2300A_MASK_RX_AUTO_EXIT_DIS;
    else
        tmp |= CMT2300A_MASK_RX_AUTO_EXIT_DIS;
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_SYS10, tmp);
}

void CMT2300A_SetRssiDetectMode(u8 index, u8 nRssiDetectMode)
{
    u8 tmp = CMT2300A_ReadReg(index, CMT2300A_CUS_SYS11);
    
    tmp &= ~CMT2300A_MASK_RSSI_DET_SEL;
    tmp |= nRssiDetectMode & CMT2300A_MASK_RSSI_DET_SEL;
    
    CMT2300A_WriteReg(index, CMT2300A_CUS_SYS11, tmp);
}

/*! ********************************************************
* @name    CMT2300A_Init
* @desc    Initialize chip status.
* *********************************************************/
void CMT2300A_Init(u8 index)
{
    u8 tmp;

    CMT2300A_SoftReset(index);
    CMT2300A_DelayMs(20);
    
    CMT2300A_GoStby(index);

    tmp  = CMT2300A_ReadReg(index, CMT2300A_CUS_MODE_STA);
    tmp |= CMT2300A_MASK_CFG_RETAIN;        /* Enable CFG_RETAIN */
    tmp &= ~CMT2300A_MASK_RSTN_IN_EN;       /* Disable RSTN_IN */
    CMT2300A_WriteReg(index, CMT2300A_CUS_MODE_STA, tmp);
    
    CMT2300A_EnableLfosc(index, FALSE);     /* Diable LFOSC */

    CMT2300A_ClearInterruptFlags(index);
}

/*! ********************************************************
* @name    CMT2300A_ConfigRegBank
* @desc    Config one register bank.
* *********************************************************/
BOOL CMT2300A_ConfigRegBank(u8 index, u8 base_addr, const u8 bank[], u8 len)
{
    u8 i;
    for(i=0; i<len; i++) {
        CMT2300A_WriteReg(index, i+base_addr, bank[i]);
        
        if(CMT2300A_ReadReg(index, i+base_addr) != bank[i]) {
            //printf("ERR\n");
            //return FALSE;
        }
    }

    return TRUE;
}
