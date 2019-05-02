#!/usr/bin/python
#coding=utf-8

import smbus
import time

CMT3501_CMD_SOFT_RESET		= 0x06
CMT3501_CMD_READ_PT			= 0x10
CMT3501_CMD_READ_AT			= 0x11
CMT3501_CMD_READ_P			= 0x30
CMT3501_CMD_READ_A			= 0x31
CMT3501_CMD_READ_T			= 0x32
CMT3501_CMD_ADC_CONVERT		= 0x40
CMT3501_CMD_RE_CAL			= 0x28
CMT3501_CMD_READ_MEM		= 0x80
CMT3501_CMD_WRITE_MEM		= 0xC0
CMT3501_CMD_MANU_PUP		= 0x2A
CMT3501_CMD_MANU_PDN		= 0x2C
CMT3501_CMD_NVM_ACC			= 0x26
CMT3501_CMD_NMODE			= 0x22
CMT3501_CMD_CMODE			= 0x20
CMT3501_CMD_TMODE			= 0x24
CMT3501_CMD_OSC_CAL			= 0x2E
CMT3501_CMD_TEST_BANK0		= 0x02
CMT3501_CMD_TEST_BANK1		= 0x04

class Cmt3501(object):
	def __init__(self, DEVICE_ADDR=0x76):
		self.DEVICE_ADDR = DEVICE_ADDR
		self.bus = smbus.SMBus(1)

		self.WriteRegs(CMT3501_CMD_SOFT_RESET, [])
		time.sleep(.2)

	def WriteRegs(self, cmd, vals):
		if None == vals or 0 == len(vals):
			return self.bus.write_byte(self.DEVICE_ADDR, cmd)
		else:
			return self.bus.write_i2c_block_data(self.DEVICE_ADDR, cmd, vals)

	def ReadRegs(self, cmd, _size):
		if 0 == _size:
			return self.bus.read_byte(self.DEVICE_ADDR, cmd)
		else:
			return self.bus.read_i2c_block_data(self.DEVICE_ADDR, cmd, _size)

	def GetAdcFromBuf(self, buf):
		nAdc = (buf[0]<<16) | (buf[1]<<8) | (buf[2])
		return nAdc

	def AdcNegativeConvert(self, nAdc):
		if (nAdc & 0x8000):
			sRet = 0x1000000 - nAdc
			sRet = -sRet
		else:
			sRet = nAdc

		return sRet

	def ReadPTValue(self):
		self.WriteRegs(CMT3501_CMD_ADC_CONVERT, [])
		time.sleep(.2)

		buf   = self.ReadRegs(CMT3501_CMD_READ_PT, 6)
		temp  = 1.0*self.AdcNegativeConvert(self.GetAdcFromBuf(buf)) /100
		press = 1.0*self.GetAdcFromBuf(buf[3:]) /100

		return temp, press

if __name__ == '__main__':
	cmt3501 = Cmt3501(0x76)

	while True:
		print("temp:%.2f degrees, press:%.2f mbar" %(cmt3501.ReadPTValue()))
		time.sleep(1)

