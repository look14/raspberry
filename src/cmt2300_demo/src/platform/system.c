#include "system.h"
#include "common.h"

#define DEF_WIRINGPI_ISR(x)\
	void wiringPiISR_##x(void) {\
		if(g_wiringPiISR[x].callback)\
			g_wiringPiISR[x].callback(g_wiringPiISR[x].param);\
	}

DEF_WIRINGPI_ISR(0)
DEF_WIRINGPI_ISR(1)
DEF_WIRINGPI_ISR(2)
DEF_WIRINGPI_ISR(3)
DEF_WIRINGPI_ISR(4)
DEF_WIRINGPI_ISR(5)
DEF_WIRINGPI_ISR(6)
DEF_WIRINGPI_ISR(7)
DEF_WIRINGPI_ISR(8)
DEF_WIRINGPI_ISR(9)
DEF_WIRINGPI_ISR(10)
DEF_WIRINGPI_ISR(11)
DEF_WIRINGPI_ISR(12)
DEF_WIRINGPI_ISR(13)
DEF_WIRINGPI_ISR(14)
DEF_WIRINGPI_ISR(15)
DEF_WIRINGPI_ISR(16)
DEF_WIRINGPI_ISR(21)
DEF_WIRINGPI_ISR(22)
DEF_WIRINGPI_ISR(23)
DEF_WIRINGPI_ISR(24)
DEF_WIRINGPI_ISR(25)
DEF_WIRINGPI_ISR(26)
DEF_WIRINGPI_ISR(27)
DEF_WIRINGPI_ISR(28)
DEF_WIRINGPI_ISR(29)
DEF_WIRINGPI_ISR(30)
DEF_WIRINGPI_ISR(31)

tWiringPiISR g_wiringPiISR[] = {
	{ wiringPiISR_0,  NULL, NULL, },
	{ wiringPiISR_1,  NULL, NULL, },
	{ wiringPiISR_2,  NULL, NULL, },
	{ wiringPiISR_3,  NULL, NULL, },
	{ wiringPiISR_4,  NULL, NULL, },
	{ wiringPiISR_5,  NULL, NULL, },
	{ wiringPiISR_6,  NULL, NULL, },
	{ wiringPiISR_7,  NULL, NULL, },
	{ wiringPiISR_8,  NULL, NULL, },
	{ wiringPiISR_9,  NULL, NULL, },
	{ wiringPiISR_10, NULL, NULL, },
	{ wiringPiISR_11, NULL, NULL, },
	{ wiringPiISR_12, NULL, NULL, },
	{ wiringPiISR_13, NULL, NULL, },
	{ wiringPiISR_14, NULL, NULL, },
	{ wiringPiISR_15, NULL, NULL, },
	{ wiringPiISR_16, NULL, NULL, },
	{ NULL,           NULL, NULL, },
	{ NULL,           NULL, NULL, },
	{ NULL,           NULL, NULL, },
	{ NULL,           NULL, NULL, },
	{ wiringPiISR_21, NULL, NULL, },
	{ wiringPiISR_22, NULL, NULL, },
	{ wiringPiISR_23, NULL, NULL, },
	{ wiringPiISR_24, NULL, NULL, },
	{ wiringPiISR_25, NULL, NULL, },
	{ wiringPiISR_26, NULL, NULL, },
	{ wiringPiISR_27, NULL, NULL, },
	{ wiringPiISR_28, NULL, NULL, },
	{ wiringPiISR_29, NULL, NULL, },
	{ wiringPiISR_30, NULL, NULL, },
	{ wiringPiISR_31, NULL, NULL, },
};

u32 system_get_tick(void)
{
	struct timeval now;
	u32 tmp;

	gettimeofday(&now, NULL);
	tmp = now.tv_sec*1000 + now.tv_usec/1000;

	return tmp;
}

void system_delay_ms(u32 n)
{
	usleep(n*1000);
}

void system_delay_us(u32 n)
{	
	usleep(n);
}

void system_delay_ns(u32 n)
{
	struct timespec beg,end;
	u32 tmp;

	clock_gettime(CLOCK_MONOTONIC, &beg);

	while(1)
	{
		clock_gettime(CLOCK_MONOTONIC, &end);
		tmp = (end.tv_sec-beg.tv_sec)*1000000000 + (end.tv_nsec-beg.tv_nsec);

		if(tmp > n)
			break;
	}
}

void IntSrv_Register(int pin, int edgeType, void(*callback)(void*), void* param)
{
	g_wiringPiISR[pin].callback = callback;
	g_wiringPiISR[pin].param = param;
	wiringPiISR(pin, edgeType, g_wiringPiISR[pin].isr);
}

void IntSrv_UnRegister(int pin)
{
	g_wiringPiISR[pin].callback = NULL;
	g_wiringPiISR[pin].param = NULL;
	wiringPiISR(pin, INT_EDGE_SETUP, NULL);
}
