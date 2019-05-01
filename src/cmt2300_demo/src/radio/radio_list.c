#include "radio_list.h"

#define RF_RECV_LIST_SIZE 8

tRadioRecv g_radioRecvList[RF_RECV_LIST_SIZE];
int g_radioRecvHead;
int g_radioRecvTail;
int g_radioRecvCount;

pthread_mutex_t g_radioRecvMutex;
pthread_cond_t g_radioRecvCond;

void RadioRecvList_Init(void)
{
	g_radioRecvHead  = 0;
	g_radioRecvTail  = 0;
	g_radioRecvCount = 0;
	
	memset(g_radioRecvList, 0, sizeof(g_radioRecvList));

	pthread_mutex_init(&g_radioRecvMutex, NULL);
	pthread_cond_init(&g_radioRecvCond, NULL);
}

void RadioRecvList_DeInit(void)
{
	pthread_mutex_destroy(&g_radioRecvMutex);
	pthread_cond_destroy(&g_radioRecvCond);
}

BOOL RadioRecvList_IsEmpty(void)
{
	return g_radioRecvCount ?FALSE :TRUE;
}

tRadioRecv* RadioRecvList_GetHead(void)
{
	pthread_mutex_lock(&g_radioRecvMutex);

    while(0==g_radioRecvCount)
        pthread_cond_wait(&g_radioRecvCond, &g_radioRecvMutex);
    
    pthread_mutex_unlock(&g_radioRecvMutex);

    return g_radioRecvCount ?(&g_radioRecvList[g_radioRecvHead]) :NULL; 
}

tRadioRecv* RadioRecvList_Pop(void)
{
	tRadioRecv* back;
	
	if(g_radioRecvCount <= 0)
		return NULL;
    
    pthread_mutex_lock(&g_radioRecvMutex);
	
	back = &g_radioRecvList[g_radioRecvHead];
	g_radioRecvHead = (g_radioRecvHead+1) % RF_RECV_LIST_SIZE;
	g_radioRecvCount--;
    
    pthread_mutex_unlock(&g_radioRecvMutex);
	
	return back;
}

BOOL RadioRecvList_Push(tRadioRecv* radioRecv)
{
	if(g_radioRecvCount >= RF_RECV_LIST_SIZE)
		return FALSE;
    
    pthread_mutex_lock(&g_radioRecvMutex);
	
	memcpy(&g_radioRecvList[g_radioRecvTail], radioRecv, sizeof(tRadioRecv));
    memcpy(&g_radioRecvList[g_radioRecvTail].buffer, radioRecv->rxBuffer, radioRecv->rxCurLen);
    g_radioRecvList[g_radioRecvTail].rxBuffer = g_radioRecvList[g_radioRecvTail].buffer;
    
	g_radioRecvTail = (g_radioRecvTail+1) % RF_RECV_LIST_SIZE;
	g_radioRecvCount++;
    
    pthread_cond_signal(&g_radioRecvCond);
    pthread_mutex_unlock(&g_radioRecvMutex);
	
	return TRUE;
}
