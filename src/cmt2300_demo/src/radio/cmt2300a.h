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
 * @file    cmt2300a.h
 * @brief   CMT2300A transceiver RF chip driver
 *
 * @version 1.3
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */

#ifndef __CMT2300A_H
#define __CMT2300A_H

#include "typedefs.h"
#include "cmt2300a_defs.h"
#include "cmt2300a_hal.h"

#ifdef __cplusplus 
extern "C" { 
#endif

#define ENABLE_AUTO_SWITCH_CHIP_STATUS   /* Enable the auto switch chip status */

/* ************************************************************************
   The following are for chip status controls.
*  ************************************************************************ */
void CMT2300A_SoftReset(u8 index);
u8 CMT2300A_GetChipStatus(u8 index);
BOOL CMT2300A_AutoSwitchStatus(u8 index, u8 nGoCmd);
BOOL CMT2300A_GoSleep(u8 index);
BOOL CMT2300A_GoStby(u8 index);
BOOL CMT2300A_GoTFS(u8 index);
BOOL CMT2300A_GoRFS(u8 index);
BOOL CMT2300A_GoTx(u8 index);
BOOL CMT2300A_GoRx(u8 index);


/* ************************************************************************
*  The following are for chip interrupts, GPIO, FIFO operations.
*  ************************************************************************ */
void CMT2300A_ConfigGpio(u8 index, u8 nGpioSel);
void CMT2300A_ConfigInterrupt(u8 index, u8 nInt1Sel, u8 nInt2Sel);
void CMT2300A_SetInterruptPolar(u8 index, BOOL bActiveHigh);
void CMT2300A_SetFifoThreshold(u8 index, u8 nFifoThreshold);
void CMT2300A_EnableAntennaSwitch(u8 index, u8 nMode);
void CMT2300A_EnableInterrupt(u8 index, u8 nEnable);
void CMT2300A_EnableRxFifoAutoClear(u8 index, BOOL bEnable);
void CMT2300A_EnableFifoMerge(u8 index, BOOL bEnable);
void CMT2300A_EnableReadFifo(u8 index);
void CMT2300A_EnableWriteFifo(u8 index);
void CMT2300A_RestoreFifo(u8 index);
void CMT2300A_ClearTxFifo(u8 index);
void CMT2300A_ClearRxFifo(u8 index);
void CMT2300A_ClearInterruptFlags(u8 index);


/* ************************************************************************
*  The following are for Tx DIN operations in direct mode.
*  ************************************************************************ */
void CMT2300A_ConfigTxDin(u8 index, u8 nDinSel);
void CMT2300A_EnableTxDin(u8 index, BOOL bEnable);
void CMT2300A_EnableTxDinInvert(u8 index, BOOL bEnable);


/* ************************************************************************
*  The following are general operations.
*  ************************************************************************ */
BOOL CMT2300A_IsExist(u8 index);
u8 CMT2300A_GetRssiCode(u8 index);
short CMT2300A_GetRssiDBm(u8 index);
void CMT2300A_SetFrequencyChannel(u8 index, u8 nChann);
void CMT2300A_SetFrequencyStep(u8 index, u8 nOffset);
void CMT2300A_SetPayloadLength(u8 index, u16 nLength);
void CMT2300A_EnableLfosc(u8 index, BOOL bEnable);
void CMT2300A_EnableLfoscOutput(u8 index, BOOL bEnable);
void CMT2300A_EnableErrorStop(u8 index, BOOL bEnable);
void CMT2300A_SetTxPower(u8 index, u8 nPowerCode, u8 nPCodeAdj);
void CMT2300A_SetPacketMode(u8 index, u8 nPacketMode);
void CMT2300A_SetPacketType(u8 index, u8 nPacketType);
void CMT2300A_EnableRxAutoExit(u8 index, BOOL bEnable);
void CMT2300A_SetRssiDetectMode(u8 index, u8 nRssiDetectMode);

/* ************************************************************************
*  The following are for chip initializes.
*  ************************************************************************ */
void CMT2300A_Init(u8 index);
BOOL CMT2300A_ConfigRegBank(u8 index, u8 base_addr, const u8 bank[], u8 len);

#ifdef __cplusplus
} 
#endif

#endif
