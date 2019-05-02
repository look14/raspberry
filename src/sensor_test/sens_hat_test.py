#!/usr/bin/python
#coding=utf-8
import time

from time import sleep
from sense_hat import SenseHat

if __name__ == '__main__':
	sense = SenseHat()
	
	#sense.show_message("Hello world!")

	print("humidity: %s %%rH" % sense.humidity)
	print("pressure: %s Millibars" % sense.pressure)
	print("temperature: %s C" % sense.temperature)
	print("temperature: %s C" % sense.get_temperature_from_pressure())
	
