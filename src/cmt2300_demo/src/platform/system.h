#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "typedefs.h"

typedef struct {
	void(*isr)(void);
	void(*callback)(void* param);
	void* param;

} tWiringPiISR;

extern tWiringPiISR g_wiringPiISR[];

u32 system_get_tick(void);
void system_delay_ms(u32 n);
void system_delay_us(u32 n);
void system_delay_ns(u32 n);

void IntSrv_Register(int pin, int edgeType, void(*callback)(void*), void* param);
void IntSrv_UnRegister(int pin);

#endif
