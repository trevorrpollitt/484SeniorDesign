import CHIP_IO.GPIO as GPIO

def testFreq():

    GPIO.setup("CSID1",GPIO.OUT)

    while True: # do 24 times
        # Pulse the ADC once
        GPIO.output("CSID1",GPIO.HIGH) # push to high to get the value
        GPIO.output("CSID1",GPIO.LOW) # end low

try:

    testFreq()

except (KeyboardInterrupt, SystemExit):
	print('Bye :)')
	
finally:
	GPIO.cleanup("CSID1")