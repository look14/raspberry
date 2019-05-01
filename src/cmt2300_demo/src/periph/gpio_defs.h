#ifndef __GPIO_DEFS_H
#define __GPIO_DEFS_H

#include <wiringPi.h>
//#include "stm32f10x_conf.h"

/*
 * PIN_1                                3v3 Power
 * PIN_3   BCM_2   WIRINGPI_8
 * PIN_5   BCM_3   WIRINGPI_9
 * PIN_7   BCM_4   WIRINGPI_7
 * PIN_9                                Ground
 * PIN_11  BCM_17  WIRINGPI_0
 * PIN_13  BCM_27  WIRINGPI_2
 * PIN_15  BCM_22  WIRINGPI_3
 * PIN_17                               3v3 Power
 * PIN_19  BCM_10  WIRINGPI_12
 * PIN_21  BCM_9   WIRINGPI_13
 * PIN_23  BCM_11  WIRINGPI_14
 * PIN_25                               Ground
 * PIN_27  BCM_0   WIRINGPI_30
 * PIN_29  BCM_5   WIRINGPI_21
 * PIN_31  BCM_6   WIRINGPI_22
 * PIN_33  BCM_13  WIRINGPI_23
 * PIN_35  BCM_19  WIRINGPI_24
 * PIN_37  BCM_26  WIRINGPI_25
 * PIN_39                               Ground
 *
 * PIN_2                                5v Power
 * PIN_4                                5v Power
 * PIN_6                                Ground
 * PIN_8   BCM_14  WIRINGPI_15
 * PIN_10  BCM_15  WIRINGPI_16
 * PIN_12  BCM_18  WIRINGPI_1
 * PIN_14                               Ground
 * PIN_16  BCM_23  WIRINGPI_4
 * PIN_18  BCM_24  WIRINGPI_5
 * PIN_20                               Ground
 * PIN_22  BCM_25  WIRINGPI_6
 * PIN_24  BCM_8   WIRINGPI_10
 * PIN_26  BCM_7   WIRINGPI_11
 * PIN_28  BCM_1   WIRINGPI_31
 * PIN_30                               Ground
 * PIN_32  BCM_12  WIRINGPI_26
 * PIN_34                               Ground
 * PIN_36  BCM_16  WIRINGPI_27
 * PIN_38  BCM_20  WIRINGPI_28
 * PIN_40  BCM_21  WIRINGPI_29
 *
 */

 #define WIRINGPI_8          8
 #define WIRINGPI_9          9
 #define WIRINGPI_7          7
 #define WIRINGPI_0          0
 #define WIRINGPI_2          2
 #define WIRINGPI_3          3
 #define WIRINGPI_12         12
 #define WIRINGPI_13         13
 #define WIRINGPI_14         14
 #define WIRINGPI_30         30
 #define WIRINGPI_21         21
 #define WIRINGPI_22         22
 #define WIRINGPI_23         23
 #define WIRINGPI_24         24
 #define WIRINGPI_25         25
 #define WIRINGPI_15         15
 #define WIRINGPI_16         16
 #define WIRINGPI_1          1
 #define WIRINGPI_4          4
 #define WIRINGPI_5          5
 #define WIRINGPI_6          6
 #define WIRINGPI_10         10
 #define WIRINGPI_11         11
 #define WIRINGPI_31         31
 #define WIRINGPI_26         26
 #define WIRINGPI_27         27
 #define WIRINGPI_28         28
 #define WIRINGPI_29         29

 #define CMT_CSB_GPIO        WIRINGPI_8
 #define CMT_FCSB_GPIO       WIRINGPI_9 
 #define CMT_SCL_GPIO        WIRINGPI_7
 #define CMT_SDA_GPIO        WIRINGPI_0
 #define CMT_GPIO1_GPIO      WIRINGPI_2
 #define CMT_GPIO2_GPIO      WIRINGPI_3
 #define CMT_GPIO3_GPIO      WIRINGPI_12

 #define CMT2_CSB_GPIO       WIRINGPI_15
 #define CMT2_FCSB_GPIO      WIRINGPI_16
 #define CMT2_SCL_GPIO       WIRINGPI_1
 #define CMT2_SDA_GPIO       WIRINGPI_4
 #define CMT2_GPIO1_GPIO     WIRINGPI_5
 #define CMT2_GPIO2_GPIO     WIRINGPI_6
 #define CMT2_GPIO3_GPIO     WIRINGPI_10


#define SET_GPIO_OUT(x)             pinMode(x, OUTPUT)
#define SET_GPIO_IN(x)              pinMode(x, INPUT)
#define SET_GPIO_OD(x)              GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_OD)
#define SET_GPIO_AIN(x)             GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AIN)
#define SET_GPIO_AFOUT(x)           GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AF_PP)
#define SET_GPIO_AFOD(x)            GPIO_Pin_Setting(x, x##_PIN, GPIO_Speed_50MHz, GPIO_Mode_AF_OD)
#define SET_GPIO_H(x)               digitalWrite(x, HIGH)
#define SET_GPIO_L(x)               digitalWrite(x, LOW)
#define READ_GPIO_PIN(x)            digitalRead(x)

#endif
