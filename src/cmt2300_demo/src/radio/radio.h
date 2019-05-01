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
 * @file    radio.h
 * @brief   Generic radio handlers
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __RADIO_H
#define __RADIO_H

#include "typedefs.h"
#include "cmt2300a.h"

#ifdef __cplusplus 
extern "C" { 
#endif
    
/* RF state machine */
typedef enum {
    RF_STATE_IDLE = 0,
    RF_STATE_RX_START,
    RF_STATE_RX_WAIT,
    RF_STATE_RX_PAUSE,
    RF_STATE_RX_DONE,
    RF_STATE_TX_START,
    RF_STATE_TX_WAIT,
    RF_STATE_TX_DONE,
    RF_STATE_RSSI,
    RF_STATE_ERR,
    
} tRadioStatus;

/* RF process function results */
typedef enum {
    RF_SUCCESS = 0,
    RF_RX_OFF,
    RF_SYS_ERR,
    
} tRadioResult;

typedef enum {
    RF_EVENT_NONE       = 0,
    RF_EVENT_INIT_DONE  = 1<<0,
    RF_EVENT_TRX_EXIT   = 1<<1,
    RF_EVENT_TRX_DONE   = 1<<2,
    RF_EVENT_GPIO2_INT  = 1<<3,
    RF_EVENT_GPIO3_INT  = 1<<4,
    RF_EVENT_RX_START   = 1<<5,
    RF_EVENT_RX_READY   = 1<<6,
    RF_EVENT_TX_START   = 1<<7,
    RF_EVENT_TX_READY   = 1<<8,
    RF_EVENT_TX_DONE    = 1<<9,
    RF_EVENT_RSSI_START = 1<<10,
    RF_EVENT_RSSI_READY = 1<<11,
    RF_EVENT_RSSI_EXIT  = 1<<12,
    RF_EVENT_RX_PAUSE   = 1<<13,
    RF_EVENT_RX_RECOVER = 1<<14,
    RF_EVENT_ERR        = 1<<15,
    RF_EVENT_EXIT       = 1<<16,
    
} tRadioEvent;

#define RF_BUFFER_SIZE      128
#define RF_FIFO_TH          16
#define RF_FIFO_FULL        32
#define RF_CHANN_STEP       400


typedef struct {
    u8 index;
    u8 buffer[RF_BUFFER_SIZE];
    
    u8*       rxBuffer;
    const u8* txBuffer;
    short     txIndex;
    short     txAllLen;
    short     rxBufSize;
    short     rxOffset;
    short     rxIndex;
    short     rxCurLen;
    short     rxAllLen;
    
    u8           packetType;
    BOOL         needLenFiled;
    u32          count;
    short        txChannel;
    short        rxChannel;
    tRadioStatus currStatus;
    
    short txPower;
    short rssiDBm;
    u32   timestamp;
    
    u32 events;
    pthread_t        thread;
    pthread_cond_t   cond;
    pthread_mutex_t  mutex;
    
} tRadioData;

extern tRadioData g_radios[RADIO_SIZE];

void Radio_Init(tRadioData* pRadio, u8 index);
void Radio_Config(tRadioData* pRadio);

void Radio_SetEvent(tRadioData* pRadio, u32 events);
void Radio_ClearEvent(tRadioData* pRadio, u32 events);
u32 Radio_WaitEvent(tRadioData* pRadio, u32 events, u32 timeout);

void Radio_SetPayloadLength(tRadioData* pRadio, short nPayloadLen);
void Radio_WriteTxFifo(tRadioData* pRadio, BOOL bFirst);
void Radio_ReadRxFifo(tRadioData* pRadio, BOOL bPktOk);
short Radio_SetTxPower(tRadioData* pRadio, short dBm);

tRadioResult Radio_SendData(tRadioData* pRadio, const u8 buf[], short len);
tRadioResult Radio_StartRx(tRadioData* pRadio, BOOL waitFinish);
tRadioResult Radio_ExitTRx(tRadioData* pRadio);
tRadioResult Radio_ReadRssi(tRadioData* pRadio, short* rssiValue);
tRadioResult Radio_SetOtherRxPause(u8 channel, BOOL bEnable);

void* Radio_Process(void* pParam);

#ifdef __cplusplus 
} 
#endif

#endif
