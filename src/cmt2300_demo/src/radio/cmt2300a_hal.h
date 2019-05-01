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
 * @file    cmt2300a_hal.h
 * @brief   CMT2300A hardware abstraction layer
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __CMT2300A_HAL_H
#define __CMT2300A_HAL_H

#include "typedefs.h"
#include "gpio_defs.h"
#include "common.h"
#include "system.h"

#ifdef __cplusplus 
extern "C" { 
#endif

/* ************************************************************************
*  The following need to be modified by user
*  ************************************************************************ */
#define CMT2300A_SetGpio1In(index)     cmt_spi3_gpio1_in(&g_spi3[index])
#define CMT2300A_SetGpio2In(index)     cmt_spi3_gpio2_in(&g_spi3[index])
#define CMT2300A_SetGpio3In(index)     cmt_spi3_gpio3_in(&g_spi3[index])
#define CMT2300A_ReadGpio1(index)      cmt_spi3_gpio1_read(&g_spi3[index])
#define CMT2300A_ReadGpio2(index)      cmt_spi3_gpio2_read(&g_spi3[index])
#define CMT2300A_ReadGpio3(index)      cmt_spi3_gpio3_read(&g_spi3[index])
#define CMT2300A_DelayMs(ms)           system_delay_ms(ms)
#define CMT2300A_DelayUs(us)           system_delay_us(us)
#define CMT2300A_GetTickCount()        system_get_tick()
/* ************************************************************************ */

void CMT2300A_InitGpio(u8 index);

u8 CMT2300A_ReadReg(u8 index, u8 addr);
void CMT2300A_WriteReg(u8 index, u8 addr, u8 dat);
void CMT2300A_ReadFifo(u8 index, u8 buf[], u16 len);
void CMT2300A_WriteFifo(u8 index, const u8 buf[], u16 len);

#ifdef __cplusplus 
} 
#endif

#endif
