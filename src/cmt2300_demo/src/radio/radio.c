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
 * @file    radio.c
 * @brief   Generic radio handlers
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#include "radio.h"
#include "radio_list.h"
#include "cmt2300a_params.h"
//#include "interrupt_server.h"

tRadioData g_radios[RADIO_SIZE];

/*
//              -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
13Code_315[] = {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,  9, 11, 13, 14, 16, 18, 20, 23, 26, 31, 37, 45, 57, 73, 99, };
13Code_434[] = {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 17, 19, 20, 24, 27, 31, 36, 45, 55, 70, };
13Code_915[] = {  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 16, 19, 21, 24, 27, 30, 34, 37, 42, 48, 56, 66, 79, };
13Adj_315[]  = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  3,  4,  7, 10, 18, 21, };
13Adj_434[]  = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  3,  3,  4,  5,  7, 10, 13, };
13Adj_915[]  = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  5,  7,  9, 12, 13, };

//              -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20
20Code_315[] = {  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  9, 10, 11, 13, 14, 16, 18, 20, 23, 26, 29, 33, 37, 43, 49, 58, 68, 83,107,150,180, };
20Code_434[] = {  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 16, 18, 20, 23, 26, 29, 32, 36, 41, 46, 51, 60, 66, 75, 86, 98,114, 74, 88,107,138, };
20Code_915[] = {  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 16, 19, 22, 25, 28, 31, 36, 40, 45, 51, 57, 65, 73, 83, 94,108,125, 74, 88,107,138, };
20Adj_315[]  = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  5,  5,  8, 10, 16, 24, 29, 30, };
20Adj[]      = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  4,  4,  5,  6,  7,  8,  9, 11, 12, 12, 12, 15, 18, 24, };
*/

#if 0
//                             -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
const u8 g_powerCodeTable[] = {  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 16, 19, 21, 24, 27, 30, 34, 37, 42, 48, 56, 66, 79, };
const u8 g_pcodeAdjTable[]  = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  5,  7,  9, 12, 13, };
#else   
//                             -10  -9  -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20
const u8 g_powerCodeTable[] = {  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 16, 19, 22, 25, 28, 31, 36, 40, 45, 51, 57, 65, 73, 83, 94,108,125, 74, 88,107,138, };
const u8 g_pcodeAdjTable[]  = {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  4,  4,  5,  6,  7,  8,  9, 11, 12, 12, 12, 15, 18, 24, };
#endif

static void Radio_Gpio2Callback(void* pParam);
static void Radio_Gpio3Callback(void* pParam);

void Radio_Init(tRadioData* pRadio, u8 index)
{
    u8 tmp;
    
    pRadio->index        = index;
    pRadio->currStatus   = RF_STATE_IDLE;
    pRadio->needLenFiled = FALSE;
    pRadio->count        = 0;
    pRadio->txChannel    = 0;
    pRadio->rxChannel    = 0;
    pRadio->events       = RF_EVENT_NONE;
    
    /*if(NULL==pRadio->eventGroup) {
        printf("[%d] xEventGroupCreate failed\r\n", pRadio->index);
    }*/
    
    CMT2300A_InitGpio(pRadio->index);
	CMT2300A_Init(pRadio->index);
    
    if(FALSE==CMT2300A_IsExist(pRadio->index)) {
        printf("[%d] CMT2300A not found\r\n", pRadio->index);
    }
    
    /* Config registers */
    CMT2300A_ConfigRegBank(pRadio->index, CMT2300A_CMT_BANK_ADDR       , g_cmt2300aCmtBank       , CMT2300A_CMT_BANK_SIZE       );
    CMT2300A_ConfigRegBank(pRadio->index, CMT2300A_SYSTEM_BANK_ADDR    , g_cmt2300aSystemBank    , CMT2300A_SYSTEM_BANK_SIZE    );
    CMT2300A_ConfigRegBank(pRadio->index, CMT2300A_FREQUENCY_BANK_ADDR , g_cmt2300aFrequencyBank , CMT2300A_FREQUENCY_BANK_SIZE );
    CMT2300A_ConfigRegBank(pRadio->index, CMT2300A_DATA_RATE_BANK_ADDR , g_cmt2300aDataRateBank  , CMT2300A_DATA_RATE_BANK_SIZE );
    CMT2300A_ConfigRegBank(pRadio->index, CMT2300A_BASEBAND_BANK_ADDR  , g_cmt2300aBasebandBank  , CMT2300A_BASEBAND_BANK_SIZE  );
    CMT2300A_ConfigRegBank(pRadio->index, CMT2300A_TX_BANK_ADDR        , g_cmt2300aTxBank        , CMT2300A_TX_BANK_SIZE        );
    
    // xosc_aac_code[2:0] = 2
    tmp = (~0x07) & CMT2300A_ReadReg(pRadio->index, CMT2300A_CUS_CMT10);
    CMT2300A_WriteReg(pRadio->index, CMT2300A_CUS_CMT10, tmp|0x02);
    
    // afc_en[4:4] = 0
    //tmp = (~0x10) & CMT2300A_ReadReg(pRadio->index, CMT2300A_CUS_FSK5);
    //CMT2300A_WriteReg(pRadio->index, CMT2300A_CUS_FSK5, tmp);
    
	Radio_Config(pRadio);

    pthread_mutex_init(&pRadio->mutex, NULL);
    pthread_cond_init(&pRadio->cond, NULL);
    pthread_create(&pRadio->thread, NULL, Radio_Process, pRadio);
}

void Radio_Config(tRadioData* pRadio)
{
    cmt_spi3_type* pSpi3 = &g_spi3[pRadio->index];
    
    /* Config GPIOs */
    CMT2300A_ConfigGpio(pRadio->index, 
        CMT2300A_GPIO2_SEL_INT1 | /* INT1 > GPIO2 */
        CMT2300A_GPIO3_SEL_INT2 | /* INT2 > GPIO3 */
        CMT2300A_GPIO1_SEL_DOUT
        );
    
    /* Config interrupt */
    CMT2300A_ConfigInterrupt(pRadio->index, 
        CMT2300A_INT_SEL_TX_DONE, /* Config INT1 */
        CMT2300A_INT_SEL_PKT_OK   /* Config INT2 */
        );

    /* Enable interrupt */
    CMT2300A_EnableInterrupt(pRadio->index, 
        CMT2300A_MASK_TX_DONE_EN  |
        CMT2300A_MASK_PREAM_OK_EN |
        CMT2300A_MASK_SYNC_OK_EN  |
        CMT2300A_MASK_NODE_OK_EN  |
        CMT2300A_MASK_CRC_OK_EN   |
        CMT2300A_MASK_PKT_DONE_EN
        );
    
    /* Disable low frequency OSC calibration */
    CMT2300A_EnableLfosc(pRadio->index, FALSE);
    
    /* Check the ERROR state when GoTFS/GoTx/GoFS/GoRx */
    CMT2300A_EnableErrorStop(pRadio->index, TRUE);
    
    /* Use a single 64-byte FIFO for either Tx or Rx */
    //CMT2300A_EnableFifoMerge(pRadio->index, TRUE);
    
    //CMT2300A_SetFifoThreshold(pRadio->index, 16);
    
    CMT2300A_SetFrequencyStep(pRadio->index, RF_CHANN_STEP*2/5);  // 400k as step
    
    pRadio->packetType = CMT2300A_PKT_TYPE_VARIABLE; // CMT2300A_PKT_TYPE_FIXED  CMT2300A_PKT_TYPE_VARIABLE
    CMT2300A_SetPacketType(pRadio->index, pRadio->packetType);
    
    CMT2300A_SetFifoThreshold(pRadio->index, RF_FIFO_TH);
    
    //CMT2300A_EnableRxAutoExit(pRadio->index, ENABLE);
    
    Radio_SetTxPower(pRadio, 13);
    
    IntSrv_Register(pSpi3->gpio2_gpio, INT_EDGE_BOTH, Radio_Gpio2Callback, pRadio);
    IntSrv_Register(pSpi3->gpio3_gpio, INT_EDGE_BOTH, Radio_Gpio3Callback, pRadio);
    
    /* Go to sleep for configuration to take effect */
    CMT2300A_GoSleep(pRadio->index);
}

void Radio_SetEvent(tRadioData* pRadio, u32 events)
{
    pthread_mutex_lock(&pRadio->mutex);
    pRadio->events |= events;
    pthread_cond_signal(&pRadio->cond);
    pthread_mutex_unlock(&pRadio->mutex);
}

void Radio_ClearEvent(tRadioData* pRadio, u32 events)
{
    pthread_mutex_lock(&pRadio->mutex);
    pRadio->events &= ~events;
    pthread_mutex_unlock(&pRadio->mutex);
}

u32 Radio_WaitEvent(tRadioData* pRadio, u32 events, u32 timeout)
{
    u32 ret;
    struct timeval now;
    struct timespec end_time;

    pthread_mutex_lock(&pRadio->mutex);

    if(INFINITE != timeout)
    {
        gettimeofday(&now, NULL);

        now.tv_usec += timeout*1000;
        now.tv_sec  += now.tv_usec/1000000;
        now.tv_usec %= 1000000;

        end_time.tv_sec  = now.tv_sec;
        end_time.tv_nsec = now.tv_usec*1000;
    }

    while(1)
    {
        ret = events & pRadio->events;

        if(RF_EVENT_NONE != ret) {
            pRadio->events &= ~ret;
            break;
        }

        if(INFINITE==timeout)
            pthread_cond_wait(&pRadio->cond, &pRadio->mutex);

        else if(ETIMEDOUT==pthread_cond_timedwait(&pRadio->cond, &pRadio->mutex, &end_time))
            break;
    }
    
    pthread_mutex_unlock(&pRadio->mutex);

    return ret;
}

void Radio_SetPayloadLength(tRadioData* pRadio, short nPayloadLen)
{
    if(CMT2300A_PKT_TYPE_FIXED==pRadio->packetType) /* Fixed Length */
    {
        pRadio->txAllLen = nPayloadLen;
        pRadio->rxAllLen = nPayloadLen;
        pRadio->rxOffset = 0;
        pRadio->needLenFiled = FALSE;
        
        CMT2300A_SetPayloadLength(pRadio->index, nPayloadLen-1);
    }
    
    else if(CMT2300A_PKT_TYPE_VARIABLE==pRadio->packetType) /* Varibale Length */
    {
        pRadio->txAllLen = nPayloadLen;
        pRadio->rxAllLen = 0;
        pRadio->rxOffset = 1;
        pRadio->needLenFiled = TRUE;
        
        if(RF_STATE_RX_START!=pRadio->currStatus)
            CMT2300A_SetPayloadLength(pRadio->index, nPayloadLen);
    }
    
    pRadio->txIndex = 0;
    pRadio->rxIndex = 0;
}

void Radio_WriteTxFifo(tRadioData* pRadio, BOOL bFirst)
{
    int nRemainLen = pRadio->txAllLen - pRadio->txIndex;
    
    if(nRemainLen > 0)
    {
        int nWriteLen = 0;
        
        if(bFirst)
            nWriteLen = (nRemainLen < RF_FIFO_FULL) ?nRemainLen :RF_FIFO_FULL;
        else
            nWriteLen = (nRemainLen < (RF_FIFO_FULL-RF_FIFO_TH)) ?nRemainLen :(RF_FIFO_FULL-RF_FIFO_TH);
        
        if(nWriteLen > 0) 
        {
            CMT2300A_WriteFifo(pRadio->index, &pRadio->txBuffer[pRadio->txIndex], nWriteLen);
            
            pRadio->txIndex += nWriteLen;
        }
    }
}

void Radio_ReadRxFifo(tRadioData* pRadio, BOOL bPktOk)
{
    int readLen = 0;
    
    if(TRUE==pRadio->needLenFiled)
    {
        pRadio->needLenFiled = FALSE;
        
        CMT2300A_ReadFifo(pRadio->index, &pRadio->rxBuffer[0], pRadio->rxOffset);
        
        pRadio->rxIndex = 0;
        pRadio->rxAllLen = pRadio->rxBuffer[0];
        readLen -= pRadio->rxOffset;
    }
    
    readLen = (bPktOk) ?__min(RF_FIFO_FULL, pRadio->rxAllLen-pRadio->rxIndex) :(RF_FIFO_TH+readLen);
    readLen = __min(readLen, pRadio->rxBufSize-pRadio->rxIndex);
    
    if(readLen > 0)
    {
        CMT2300A_ReadFifo(pRadio->index, &pRadio->rxBuffer[pRadio->rxIndex], readLen);
        
        pRadio->rxIndex += readLen;
    }
}

short Radio_SetTxPower(tRadioData* pRadio, short dBm)
{
    pRadio->txPower = dBm;
    
    if(dBm <-10)
        pRadio->txPower = -10;
    else if(dBm >20)
        pRadio->txPower = 20;
    else
        pRadio->txPower = dBm;
    
    pRadio->txPower += 10;
    
    return pRadio->txPower - 10;
}

static void Radio_Gpio2Callback(void* pParam)
{
    tRadioData* pRadio = (tRadioData*)pParam;
    cmt_spi3_type* pSpi3 = &g_spi3[pRadio->index];
    
    // rising edge
    if(cmt_spi3_gpio2_read(pSpi3))
    {
        if(RF_STATE_RX_WAIT==pRadio->currStatus)
            Radio_ReadRxFifo(pRadio, FALSE);
    }
    else
    {
        if(RF_STATE_TX_WAIT==pRadio->currStatus)
            Radio_WriteTxFifo(pRadio, FALSE);
    }
}

static void Radio_Gpio3Callback(void* pParam)
{
    tRadioData* pRadio = (tRadioData*)pParam;
    cmt_spi3_type* pSpi3 = &g_spi3[pRadio->index];
    
    // rising edge
    if(cmt_spi3_gpio3_read(pSpi3))
    {
        if(RF_STATE_RX_WAIT==pRadio->currStatus)
        {
            pRadio->timestamp = CMT2300A_GetTickCount();
            Radio_SetEvent(pRadio, RF_EVENT_GPIO3_INT);
            //portYIELD_FROM_ISR(pdTRUE);
        }
        else if(RF_STATE_TX_WAIT==pRadio->currStatus)
        {
            pRadio->timestamp = CMT2300A_GetTickCount();
            Radio_SetEvent(pRadio, RF_EVENT_GPIO3_INT);
            //portYIELD_FROM_ISR(pdTRUE);
        }
    }
}

tRadioResult Radio_SendData(tRadioData* pRadio, const u8 buf[], short len)
{
    u32 events;
    
    Radio_ExitTRx(pRadio);
    
    pRadio->txBuffer = buf;
    pRadio->txAllLen = len;
    
    Radio_ClearEvent(pRadio, RF_EVENT_TX_DONE|RF_EVENT_ERR|RF_EVENT_RX_START);
    Radio_SetEvent(pRadio, RF_EVENT_TX_START);
    
    events = Radio_WaitEvent(pRadio, RF_EVENT_TX_DONE|RF_EVENT_ERR, INFINITE);
    
    if(RF_EVENT_ERR & events) {
        return RF_SYS_ERR;
    }
    
    return RF_SUCCESS;
}

tRadioResult Radio_StartRx(tRadioData* pRadio, BOOL waitFinish)
{
    u32 events;
    
    Radio_ExitTRx(pRadio);
    
    pRadio->rxBuffer = pRadio->buffer;
    pRadio->rxBufSize = RF_BUFFER_SIZE;
    
    Radio_ClearEvent(pRadio, RF_EVENT_RX_READY|RF_EVENT_ERR|RF_EVENT_TX_START);
    Radio_SetEvent(pRadio, RF_EVENT_RX_START);
    
    if(FALSE==waitFinish)
        return RF_SUCCESS;
    
    events = Radio_WaitEvent(pRadio, RF_EVENT_RX_READY|RF_EVENT_ERR, INFINITE);
    
    if(RF_EVENT_ERR & events) {
        return RF_SYS_ERR;
    }
    
    return RF_SUCCESS;
}

tRadioResult Radio_ExitTRx(tRadioData* pRadio)
{
    if(RF_STATE_IDLE!=pRadio->currStatus) {
        Radio_SetEvent(pRadio, RF_EVENT_TRX_EXIT);
        Radio_WaitEvent(pRadio, RF_EVENT_TRX_DONE, INFINITE);
    }
    
    return RF_SUCCESS;
}

tRadioResult Radio_ReadRssi(tRadioData* pRadio, short* rssiValue)
{
    if(RF_STATE_RX_WAIT!=pRadio->currStatus)
        return RF_RX_OFF;
    
    Radio_SetEvent(pRadio, RF_EVENT_RSSI_START);
    Radio_WaitEvent(pRadio, RF_EVENT_RSSI_READY, INFINITE);
    
    *rssiValue = CMT2300A_GetRssiDBm(pRadio->index);
    
    Radio_SetEvent(pRadio, RF_EVENT_RSSI_EXIT);
    
    return RF_SUCCESS;
}

void* Radio_Process(void* pParam)
{
    tRadioData* pRadio = (tRadioData*)pParam;
    BOOL bContinue = TRUE;
    u32 events;
    
    while(TRUE==bContinue)
    {
        switch(pRadio->currStatus)
        {
        case RF_STATE_IDLE:
            CMT2300A_GoStby(pRadio->index);
            CMT2300A_ClearInterruptFlags(pRadio->index);
        
            Radio_SetEvent(pRadio, RF_EVENT_TRX_DONE);
            events = Radio_WaitEvent(pRadio, RF_EVENT_TX_START|RF_EVENT_RX_START|RF_EVENT_EXIT, INFINITE);
        
            if(RF_EVENT_EXIT & events) {
                bContinue = FALSE;
            }
            else if(RF_EVENT_TX_START & events) {
                pRadio->currStatus = RF_STATE_TX_START;
            }
            else if(RF_EVENT_RX_START & events) {
                pRadio->currStatus = RF_STATE_RX_START;
            }
            break;
        
        case RF_STATE_RX_START:
            if(FALSE==CMT2300A_GoStby(pRadio->index)) {
                pRadio->currStatus = RF_STATE_ERR;
            }
            else {
                Radio_SetPayloadLength(pRadio, pRadio->rxBufSize);
                
                pRadio->rssiDBm = 0;
                CMT2300A_ClearInterruptFlags(pRadio->index);
                
                Radio_ClearEvent(pRadio, RF_EVENT_GPIO3_INT|RF_EVENT_TRX_DONE/*|RF_EVENT_RX_PAUSE|RF_EVENT_RX_RECOVER*/);
                
                CMT2300A_SetRssiDetectMode(pRadio->index, CMT2300A_RSSI_DET_SYNC_OK);
                CMT2300A_SetFrequencyChannel(pRadio->index, pRadio->rxChannel);
                //CMT2300A_WriteReg(pRadio->index, CMT2300A_CUS_PKT10, pRadio->rxChannel);
                
                /* Must clear FIFO after enable SPI to read or write the FIFO */
                CMT2300A_EnableReadFifo(pRadio->index);
                CMT2300A_ClearRxFifo(pRadio->index);
                
                CMT2300A_ConfigInterrupt(pRadio->index, CMT2300A_INT_SEL_RX_FIFO_TH, CMT2300A_INT_SEL_PKT_DONE); 
                
                if(FALSE==CMT2300A_GoRx(pRadio->index)) {
                    pRadio->currStatus = RF_STATE_ERR;
                }
                else {
                    pRadio->currStatus = RF_STATE_RX_WAIT;
                }
            }
            break;
        
        case RF_STATE_RX_WAIT:
            Radio_SetEvent(pRadio, RF_EVENT_RX_READY);
            events = Radio_WaitEvent(pRadio, RF_EVENT_TRX_EXIT|RF_EVENT_GPIO3_INT|RF_EVENT_RSSI_START|RF_EVENT_RX_PAUSE, INFINITE);
            
            if(RF_EVENT_RSSI_START & events) {
                pRadio->currStatus = RF_STATE_RSSI;
            }
            else if(RF_EVENT_TRX_EXIT & events) {
                pRadio->currStatus = RF_STATE_IDLE;
            }
            else if(RF_EVENT_GPIO3_INT & events) {
                pRadio->currStatus = RF_STATE_RX_DONE;
            }
            else if(RF_EVENT_RX_PAUSE & events) {
                pRadio->currStatus = RF_STATE_RX_PAUSE;
            }
            break;
            
        case RF_STATE_RX_PAUSE:
            CMT2300A_GoStby(pRadio->index);
        
            events = Radio_WaitEvent(pRadio, RF_EVENT_TRX_EXIT|RF_EVENT_RX_RECOVER, INFINITE);
        
            if(RF_EVENT_TRX_EXIT & events) {
                pRadio->currStatus = RF_STATE_IDLE;
            }
            else if(RF_EVENT_RX_RECOVER & events) {
                pRadio->currStatus = RF_STATE_RX_START;
            }
            break;
        
        case RF_STATE_RX_DONE:
            if(FALSE==CMT2300A_GoStby(pRadio->index)) {
                pRadio->currStatus = RF_STATE_ERR;
            }
            else {
                Radio_ReadRxFifo(pRadio, TRUE);
            
                pRadio->rssiDBm = CMT2300A_GetRssiDBm(pRadio->index);
                pRadio->rxCurLen = pRadio->rxIndex;
                CMT2300A_ClearInterruptFlags(pRadio->index);

                RadioRecvList_Push(pRadio);
                
                pRadio->currStatus = RF_STATE_RX_START;
            }
            break;
        
        case RF_STATE_TX_START:
            if(FALSE==CMT2300A_GoStby(pRadio->index)) {
                pRadio->currStatus = RF_STATE_ERR;
            }
            else {
                Radio_SetPayloadLength(pRadio, pRadio->txAllLen);
                
                CMT2300A_ClearInterruptFlags(pRadio->index);
                
                Radio_ClearEvent(pRadio, RF_EVENT_GPIO3_INT|RF_EVENT_TRX_DONE);
                
                CMT2300A_SetTxPower(pRadio->index, g_powerCodeTable[pRadio->txPower], g_pcodeAdjTable[pRadio->txPower]);
                CMT2300A_SetFrequencyChannel(pRadio->index, pRadio->txChannel);
                //CMT2300A_WriteReg(pRadio->index, CMT2300A_CUS_PKT10, pRadio->txChannel);
                
                /* Must clear FIFO after enable SPI to read or write the FIFO */
                CMT2300A_EnableWriteFifo(pRadio->index);
                CMT2300A_ClearTxFifo(pRadio->index);
                
                /* The length need be smaller than 32 */
                //CMT2300A_WriteFifo(index, pRadioData->pTxBuffer, pRadioData->nTxLength);
                Radio_WriteTxFifo(pRadio, TRUE);
                
                CMT2300A_ConfigInterrupt(pRadio->index, CMT2300A_INT_SEL_TX_FIFO_TH, CMT2300A_INT_SEL_TX_DONE);
            
                Radio_SetOtherRxPause(pRadio->txChannel, TRUE);
                
                if(0==(CMT2300A_MASK_TX_FIFO_NMTY_FLG & CMT2300A_ReadReg(pRadio->index, CMT2300A_CUS_FIFO_FLAG))) {
                    pRadio->currStatus = RF_STATE_ERR;
                }
                if(FALSE==CMT2300A_GoTx(pRadio->index)) {
                    pRadio->currStatus = RF_STATE_ERR;
                }
                else {
                    pRadio->currStatus = RF_STATE_TX_WAIT;
                }
            }
            break;
        
        case RF_STATE_TX_WAIT:
            Radio_SetEvent(pRadio, RF_EVENT_TX_READY);
            events = Radio_WaitEvent(pRadio, RF_EVENT_TRX_EXIT|RF_EVENT_GPIO3_INT, INFINITE);
            
            if(RF_EVENT_TRX_EXIT & events) {
                pRadio->currStatus = RF_STATE_IDLE;
            }
            else if(RF_EVENT_GPIO3_INT & events) {
                pRadio->currStatus = RF_STATE_TX_DONE;
            }
            break;
        
        case RF_STATE_TX_DONE:
            if(FALSE==CMT2300A_GoStby(pRadio->index)) {
                pRadio->currStatus = RF_STATE_ERR;
            }
            else {
                CMT2300A_ClearInterruptFlags(pRadio->index);
                
                pRadio->currStatus = RF_STATE_IDLE;
                Radio_SetEvent(pRadio, RF_EVENT_TX_DONE);
            }
            Radio_SetOtherRxPause(pRadio->txChannel, FALSE);
            break;
        
        case RF_STATE_RSSI:
            CMT2300A_SetRssiDetectMode(pRadio->index, CMT2300A_RSSI_DET_ALWAYS);
            Radio_SetEvent(pRadio, RF_EVENT_RSSI_READY);
        
            Radio_WaitEvent(pRadio, RF_EVENT_RSSI_EXIT, INFINITE);
        
            CMT2300A_SetRssiDetectMode(pRadio->index, CMT2300A_RSSI_DET_SYNC_OK);
            pRadio->currStatus = RF_STATE_RX_WAIT;
            break;
        
        case RF_STATE_ERR:
            CMT2300A_GoStby(pRadio->index);
            Radio_Config(pRadio);
        
            Radio_SetEvent(pRadio, RF_EVENT_ERR);
            pRadio->currStatus = RF_STATE_IDLE;
            break;
        }
    }

    pthread_mutex_destroy(&pRadio->mutex);
    pthread_cond_destroy(&pRadio->cond);

    return NULL;
}

tRadioResult Radio_SetOtherRxPause(u8 channel, BOOL bEnable)
{
    u8 i;
    
    for(i=0; i<RADIO_SIZE; i++) {
        if(channel!=g_radios[i].rxChannel)
            continue;
        
        if(TRUE==bEnable) {
            Radio_SetEvent(&g_radios[i], RF_EVENT_RX_PAUSE);
        }
        else {
            Radio_SetEvent(&g_radios[i], RF_EVENT_RX_RECOVER);
        }
    }
    
    return RF_SUCCESS;
}
