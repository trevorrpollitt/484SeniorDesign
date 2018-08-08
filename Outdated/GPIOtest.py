import CHIP_IO.GPIO as GPIO
import time
import requests

def initGPIO132():
    
    GPIO.setup("CSID0",GPIO.IN)

def initGPIO133():
    
    GPIO.setup("CSID1",GPIO.OUT)

def isReady(port): # Check if HX711 is ready

    if GPIO.input(port): # If data pin is low
        return True
    else: # if data pin is high
        return False

def readADC(dout,pd_sck,gain_select):

    if not isReady(dout): # If data pin is low, ADC is ready
        data = []
        for i in range(0,24): # do 24 times
            # Pulse the ADC once
            GPIO.output(pd_sck,GPIO.HIGH) # push to high to get the value
            data.append(GPIO.input(dout)) # read value from ADC and append to data
            GPIO.output(pd_sck,GPIO.LOW) # end low

        if gain_select == 128:  # if you want 128 gain pulse one more time. 25 times total
            GPIO.output(pd_sck,GPIO.HIGH) # Pulse
            GPIO.output(pd_sck,GPIO.LOW)

        if gain_select == 64: # if you want 32 gain, channel B pulse two more times. 26 times total
            for i in range(0,2): # do 2 times
                GPIO.output(pd_sck,GPIO.HIGH) # Pulse
                GPIO.output(pd_sck,GPIO.LOW)

        if gain_select == 32: # if you want 64 gain pulse three more times. 27 times total
            for i in range(0,3): # do 3 times
                GPIO.output(pd_sck,GPIO.HIGH) # Pulse
                GPIO.output(pd_sck,GPIO.LOW)

        print(data)
        data = 0 # Clear data for next read
    
    else:
        print('ADC is not ready!') # If data pin is high, ADC is not ready
        
def GPIOtest():

    initGPIO132()
    initGPIO133()
    readADC("CSID0","CSID1",64)
    GPIO.cleanup("CSID0")
    GPIO.cleanup("CSID1")

GPIOtest()