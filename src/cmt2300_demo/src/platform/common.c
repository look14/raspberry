#include "common.h"
#include "cmt_spi3.h"
#include <string.h>

cmt_spi3_type g_spi3[2];

void Common_Init(void)
{
	memset(g_spi3, 0, sizeof(g_spi3));
}

void Gpio_Config(void)
{
	wiringPiSetup();

	g_spi3[0].csb_gpio   = CMT_CSB_GPIO;
	g_spi3[0].fcsb_gpio  = CMT_FCSB_GPIO;
	g_spi3[0].scl_gpio   = CMT_SCL_GPIO;
	g_spi3[0].sda_gpio   = CMT_SDA_GPIO;
	g_spi3[0].gpio1_gpio = CMT_GPIO1_GPIO;
	g_spi3[0].gpio2_gpio = CMT_GPIO2_GPIO;
	g_spi3[0].gpio3_gpio = CMT_GPIO3_GPIO;
}
