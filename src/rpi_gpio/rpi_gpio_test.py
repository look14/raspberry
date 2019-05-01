import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
#GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)


'''GPIO.setup(37, GPIO.OUT, initial=GPIO.HIGH)

for i in range(0,20):
	GPIO.output(37, True)
	GPIO.output(37, False)'''

GPIO.setup(37, GPIO.OUT)
p = GPIO.PWM(37, 1000)
p.start(0)
for i in range(0,10):
	for dc in range(0,100,10):
		p.ChangeDutyCycle(dc)
		time.sleep(0.01)

	for dc in range(100,0,-10):
		p.ChangeDutyCycle(dc)
		time.sleep(0.01)
p.stop()


GPIO.cleanup()

