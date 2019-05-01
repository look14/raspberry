#include "cmt_spi3.h"
#include "gpio_defs.h"
#include "system.h"
#include <string.h>

#define cmt_spi3_delay(p_spi3)				system_delay_ns(200)
#define cmt_spi3_delay_half_cycle(p_spi3)	system_delay_ns(200)
#define cmt_spi3_delay_one_cycle(p_spi3)	system_delay_ns(400)
#define cmt_spi3_delay_2us(p_spi3)			system_delay_ns(2500)
#define cmt_spi3_delay_4us(p_spi3)			system_delay_ns(5000)

void cmt_spi3_init(cmt_spi3_type *p_spi3)
{
    cmt_spi3_csb_1(p_spi3);
    cmt_spi3_csb_out(p_spi3);
    cmt_spi3_csb_1(p_spi3);
    
    cmt_spi3_scl_0(p_spi3);
    cmt_spi3_scl_out(p_spi3);
    cmt_spi3_scl_0(p_spi3);   /* SCL has an internal pull-down resistor */
    
    cmt_spi3_sda_in(p_spi3);
    
    cmt_spi3_fcsb_1(p_spi3);
    cmt_spi3_fcsb_out(p_spi3);
    cmt_spi3_fcsb_1(p_spi3);
    
    cmt_spi3_gpio1_in(p_spi3);
    cmt_spi3_gpio2_in(p_spi3);
    cmt_spi3_gpio3_in(p_spi3);

    cmt_spi3_delay(p_spi3);
}

void cmt_spi3_deinit(cmt_spi3_type *p_spi3)
{
    cmt_spi3_csb_in(p_spi3);
    cmt_spi3_scl_in(p_spi3);
    cmt_spi3_sda_in(p_spi3);
    cmt_spi3_fcsb_in(p_spi3);
    cmt_spi3_gpio1_in(p_spi3);
    cmt_spi3_gpio2_in(p_spi3);
    cmt_spi3_gpio3_in(p_spi3);
    cmt_spi3_delay(p_spi3);
}

void cmt_spi3_send(cmt_spi3_type *p_spi3, u8 data8)
{
    u8 i;

    for(i=0; i<8; i++)
    {
		cmt_spi3_scl_0(p_spi3);

        /* Send byte on the rising edge of SCL */
		if(data8 & 0x80) {
			cmt_spi3_sda_1(p_spi3);
        }
		else {
			cmt_spi3_sda_0(p_spi3);
        }

		cmt_spi3_delay(p_spi3);

		data8 <<= 1;
		cmt_spi3_scl_1(p_spi3);
		cmt_spi3_delay(p_spi3);
    }
}

u8 cmt_spi3_recv(cmt_spi3_type *p_spi3)
{
    u8 i;
    u8 data8 = 0;

    for(i=0; i<8; i++)
    {
		cmt_spi3_scl_0(p_spi3);
		cmt_spi3_delay(p_spi3);
        data8 <<= 1;

		cmt_spi3_scl_1(p_spi3);

        /* Read byte on the rising edge of SCL */
		if(cmt_spi3_sda_read(p_spi3)) {
			data8 |= 0x01;
        }
		else {
			//data8 &= ~0x01;
        }

		cmt_spi3_delay(p_spi3);
    }

    return data8;
}

void cmt_spi3_write(cmt_spi3_type *p_spi3, u8 addr, const u8* p_buf, u16 len)
{
    u16 i;

    cmt_spi3_fcsb_1(p_spi3);
    cmt_spi3_csb_0(p_spi3);
    cmt_spi3_sda_1(p_spi3);
    cmt_spi3_sda_out(p_spi3);
    
    cmt_spi3_delay_half_cycle(p_spi3);      /* > 0.5 SCL cycle */
    cmt_spi3_send(p_spi3, 0x7F & addr);     /* r/w = 0 */

    for(i=0; i<len; i++) {
		cmt_spi3_send(p_spi3, p_buf[i]);
    }

    cmt_spi3_scl_0(p_spi3);
    cmt_spi3_delay_half_cycle(p_spi3);      /* > 0.5 SCL cycle */
    
    cmt_spi3_sda_in(p_spi3);
    cmt_spi3_csb_1(p_spi3);
    cmt_spi3_fcsb_1(p_spi3);
    cmt_spi3_delay(p_spi3);
}

void cmt_spi3_read(cmt_spi3_type *p_spi3, u8 addr, u8* p_buf, u16 len)
{
    u16 i;

    cmt_spi3_fcsb_1(p_spi3);
    cmt_spi3_csb_0(p_spi3);
    cmt_spi3_sda_1(p_spi3);
    cmt_spi3_sda_out(p_spi3);
    
    cmt_spi3_delay_half_cycle(p_spi3);      /* > 0.5 SCL cycle */
    cmt_spi3_send(p_spi3, 0x80 | addr);     /* r/w = 1 */
    
    /* Must set SDA to input before the falling edge of SCL */
    cmt_spi3_sda_in(p_spi3);

    for(i=0; i<len; i++) {
		p_buf[i] = cmt_spi3_recv(p_spi3);
    }
    
    cmt_spi3_scl_0(p_spi3);
    cmt_spi3_delay_half_cycle(p_spi3);      /* > 0.5 SCL cycle */

    cmt_spi3_sda_in(p_spi3);
    cmt_spi3_csb_1(p_spi3);
    cmt_spi3_fcsb_1(p_spi3);
    cmt_spi3_delay(p_spi3);
}

void cmt_spi3_write_fifo(cmt_spi3_type *p_spi3, const u8* p_buf, u16 len)
{
    u16 i;

    cmt_spi3_csb_1(p_spi3);
    cmt_spi3_sda_out(p_spi3);

    for(i=0; i<len; i++)
    {
		cmt_spi3_fcsb_0(p_spi3);
        
		cmt_spi3_delay_one_cycle(p_spi3);   /* > 1 SCL cycle */
		cmt_spi3_send(p_spi3, p_buf[i]);

		cmt_spi3_scl_0(p_spi3);
		cmt_spi3_delay_2us(p_spi3);         /* > 2 us */

		cmt_spi3_fcsb_1(p_spi3);
        cmt_spi3_delay_4us(p_spi3);         /* > 4 us */
    }
    
    cmt_spi3_sda_in(p_spi3);
    cmt_spi3_csb_1(p_spi3);
}

void cmt_spi3_read_fifo(cmt_spi3_type *p_spi3, u8* p_buf, u16 len)
{
    u16 i;

    cmt_spi3_csb_1(p_spi3);
    cmt_spi3_sda_in(p_spi3);

    for(i=0; i<len; i++)
    {
        cmt_spi3_fcsb_0(p_spi3);
    	
        cmt_spi3_delay_one_cycle(p_spi3);   /* > 1 SCL cycle */
        p_buf[i] = cmt_spi3_recv(p_spi3);

        cmt_spi3_scl_0(p_spi3);
        cmt_spi3_delay_2us(p_spi3);         /* > 2 us */

        cmt_spi3_fcsb_1(p_spi3);
        cmt_spi3_delay_4us(p_spi3);         /* > 4 us */
    }
    
    cmt_spi3_sda_in(p_spi3);
    cmt_spi3_csb_1(p_spi3);
}

