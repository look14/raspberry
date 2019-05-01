#include "typedefs.h"
#include "radio.h"
#include "radio_list.h"

int main(int argc, char* argv[])
{
	u8 buf[] = {1,2,3,4,5,6,7,8,9,10};

    Gpio_Config();
    RadioRecvList_Init();
    Radio_Init(&g_radios[0], 0);

    Radio_SendData(&g_radios[0], buf, sizeof(buf));
    Radio_StartRx(&g_radios[0], FALSE);

    while(1)
    {
        int i;
        tRadioRecv* pRadio = RadioRecvList_GetHead();

        printf("index:%d ch:%d rssi:%d [ ", pRadio->index, pRadio->rxChannel, pRadio->rssiDBm);
        for(i=0; i<pRadio->rxCurLen; i++) printf("%02X ", pRadio->buffer[i]);
        printf("]\n");
        
        RadioRecvList_Pop();
    }
    
    Radio_SetEvent(&g_radios[0], RF_EVENT_EXIT|RF_EVENT_TRX_EXIT);
    pthread_join(g_radios[0].thread, NULL);
    RadioRecvList_DeInit();

    return 0;
}
