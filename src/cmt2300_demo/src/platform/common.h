#ifndef __COMMON_H
#define __COMMON_H

#include "typedefs.h"
#include "cmt_spi3.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#define RADIO_SIZE	2

extern cmt_spi3_type g_spi3[2];

void Common_Init(void);
void Gpio_Config(void);

#endif
