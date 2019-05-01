#ifndef __CMT_SPI3_H
#define __CMT_SPI3_H

#include "typedefs.h"
#include "gpio_defs.h"
//#include "stm32f10x_conf.h"

typedef struct __cmt_spi3_type {
    u32 csb_gpio;
    u32 fcsb_gpio;
    u32 scl_gpio;
    u32 sda_gpio;
	u32 gpio1_gpio;
	u32 gpio2_gpio;
	u32 gpio3_gpio;
    u32 delay;

} cmt_spi3_type;


#define cmt_spi3_csb_out(p_spi3)    SET_GPIO_OUT((p_spi3)->csb_gpio  )
#define cmt_spi3_csb_in(p_spi3)     SET_GPIO_IN ((p_spi3)->csb_gpio  )
#define cmt_spi3_scl_out(p_spi3)    SET_GPIO_OUT((p_spi3)->scl_gpio  )
#define cmt_spi3_scl_in(p_spi3)     SET_GPIO_IN ((p_spi3)->scl_gpio  )
#define cmt_spi3_sda_out(p_spi3)    SET_GPIO_OUT((p_spi3)->sda_gpio  )
#define cmt_spi3_sda_in(p_spi3)     SET_GPIO_IN ((p_spi3)->sda_gpio  )
#define cmt_spi3_fcsb_out(p_spi3)   SET_GPIO_OUT((p_spi3)->fcsb_gpio )
#define cmt_spi3_fcsb_in(p_spi3)    SET_GPIO_IN ((p_spi3)->fcsb_gpio )
#define cmt_spi3_gpio1_out(p_spi3)  SET_GPIO_OUT((p_spi3)->gpio1_gpio)
#define cmt_spi3_gpio1_in(p_spi3)   SET_GPIO_IN ((p_spi3)->gpio1_gpio)
#define cmt_spi3_gpio2_out(p_spi3)  SET_GPIO_OUT((p_spi3)->gpio2_gpio)
#define cmt_spi3_gpio2_in(p_spi3)   SET_GPIO_IN ((p_spi3)->gpio2_gpio)
#define cmt_spi3_gpio3_out(p_spi3)  SET_GPIO_OUT((p_spi3)->gpio3_gpio)
#define cmt_spi3_gpio3_in(p_spi3)   SET_GPIO_IN ((p_spi3)->gpio3_gpio)

#define cmt_spi3_csb_1(p_spi3)      SET_GPIO_H((p_spi3)->csb_gpio)
#define cmt_spi3_csb_0(p_spi3)      SET_GPIO_L((p_spi3)->csb_gpio)
#define cmt_spi3_scl_1(p_spi3)      SET_GPIO_H((p_spi3)->scl_gpio)
#define cmt_spi3_scl_0(p_spi3)      SET_GPIO_L((p_spi3)->scl_gpio)
#define cmt_spi3_sda_1(p_spi3)      SET_GPIO_H((p_spi3)->sda_gpio)
#define cmt_spi3_sda_0(p_spi3)      SET_GPIO_L((p_spi3)->sda_gpio)
#define cmt_spi3_fcsb_1(p_spi3)     SET_GPIO_H((p_spi3)->fcsb_gpio)
#define cmt_spi3_fcsb_0(p_spi3)     SET_GPIO_L((p_spi3)->fcsb_gpio)
#define cmt_spi3_gpio1_1(p_spi3)    SET_GPIO_H((p_spi3)->gpio1_gpio)
#define cmt_spi3_gpio1_0(p_spi3)    SET_GPIO_L((p_spi3)->gpio1_gpio)
#define cmt_spi3_gpio2_1(p_spi3)    SET_GPIO_H((p_spi3)->gpio2_gpio)
#define cmt_spi3_gpio2_0(p_spi3)    SET_GPIO_L((p_spi3)->gpio2_gpio)
#define cmt_spi3_gpio3_1(p_spi3)    SET_GPIO_H((p_spi3)->gpio3_gpio)
#define cmt_spi3_gpio3_0(p_spi3)    SET_GPIO_L((p_spi3)->gpio3_gpio)

#define cmt_spi3_sda_read(p_spi3)   READ_GPIO_PIN((p_spi3)->sda_gpio) 
#define cmt_spi3_gpio1_read(p_spi3) READ_GPIO_PIN((p_spi3)->gpio1_gpio)
#define cmt_spi3_gpio2_read(p_spi3) READ_GPIO_PIN((p_spi3)->gpio2_gpio)
#define cmt_spi3_gpio3_read(p_spi3) READ_GPIO_PIN((p_spi3)->gpio3_gpio)

void cmt_spi3_init(cmt_spi3_type *p_spi3);
void cmt_spi3_deinit(cmt_spi3_type *p_spi3);

void cmt_spi3_send(cmt_spi3_type *p_spi3, u8 data8);
u8 cmt_spi3_recv(cmt_spi3_type *p_spi3);

void cmt_spi3_write(cmt_spi3_type *p_spi3, u8 addr, const u8* p_buf, u16 len);
void cmt_spi3_read(cmt_spi3_type *p_spi3, u8 addr, u8* p_buf, u16 len);

void cmt_spi3_write_fifo(cmt_spi3_type *p_spi3, const u8* p_buf, u16 len);
void cmt_spi3_read_fifo(cmt_spi3_type *p_spi3, u8* p_buf, u16 len);

#endif
