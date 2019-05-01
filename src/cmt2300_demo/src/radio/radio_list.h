#ifndef __RADIO_LIST_H
#define __RADIO_LIST_H

#include "radio.h"

typedef tRadioData tRadioRecv;

void RadioRecvList_Init(void);
void RadioRecvList_DeInit(void);
BOOL RadioRecvList_IsEmpty(void);
tRadioRecv* RadioRecvList_GetHead(void);
tRadioRecv* RadioRecvList_Pop(void);
BOOL RadioRecvList_Push(tRadioRecv* radioRecv);


#endif
