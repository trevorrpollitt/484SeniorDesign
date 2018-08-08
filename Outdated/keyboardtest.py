import sys
import time
import requests

import RPi.GPIO as GPIO
from hx711 import HX711

real=[0,0,0,0,0,0,0,0,0,0]
data = []
uploadData = {}
average = 0
averageRaw = 0

# choose pins on rpi (BCM5 and BCM6)
hx = HX711(dout=5, pd_sck=6)

# HOW TO CALCULATE THE REFFERENCE UNIT
#########################################
# To set the reference unit to 1.
# Call get_weight before and after putting 1000g weight on your sensor.
# Divide difference with grams (1000g) and use it as refference unit.

hx.setReferenceUnit(1)

def send_to_api_gateway(averageRaw, averageReal):
    
    PROJ5_API_URL = "https://4d5hwp29ll.execute-api.us-east-1.amazonaws.com/development/eagleio"
	data = [averageRaw,averageReal] # create data array
    uploadData['value'] = data # add data array to value dictionary key
    print(uploadData)
    payload = uploadData

    resp = requests.put(PROJ5_API_URL, json=payload, headers={"Content-Type": "application/json"})

    if not 200 <= resp.status_code < 300:
        print("Failed to post to AWS API with status %d" % resp.status_code)
        raise Exception("Failed to post to AWS API with status %d" % resp.status_code)

    return resp

hx.reset()
hx.tare()

while True:
    n=input('\n1: Calibration\n2: Show Raw Data\n3: Show Real Data\n4: Show Both (Raw, Real) and Upload\n5: Exit\n Choose a selection\n')
    
    if n=='1':
        print('calibration loop\n')
        x=hx.tare # Calibrates the ADC with our reference value and zero value
        print(x)

    elif n=='2':
        print('Raw Loop\n')
        true=True
        while true:
            try:
                value=hx.getValue() # Get raw value
                int(value)
                print(value)
            except (KeyboardInterrupt):
                true=False
            
    elif n=='3':
        print('Real Loop\n')
        true=True
        while true:
            try:
                value=hx.getWeight() # Get real value
                int(value)
                print(value)
            except (KeyboardInterrupt):
                true=False
    elif n=='4':
        print('Upload Once\n')

        i=0
        # Get ten values to average
        for i in range(0,10): # Do ten times
            real[i]=hx.getWeight() # Get weight value
            raw[i]=hx.getValue() # Get raw value

        i=0
        # Average vlaues
        for i in range(0,10): # Do ten times
            average+=real[i] # Add all real weight values in array
            averageRaw+=raw[i] # Add all raw values in array
        average=average/10 # all real / 10 for average
        averageRaw=average/10 # all raw / 10 for average
        averageRaw = int(round(averageRaw))
        averageRaw = int(round(average))
        print(averageRaw + ',' + average) # Print what is about to be sent
        response = send_to_api_gateway(averageRaw,average) # Send to AWS
        print(response) # Print response from AWS
        
    elif n=='5':
        print('Upload Five Times\n')

        i=0
        for i in range(0,5) # Do five times

            i=0
            # Get ten values to average
            for i in range(0,10): # Do ten times
                real[i]=hx.getWeight() # Get weight value
                raw[i]=hx.getValue() # Get raw value
                
            i=0
            # Average values
            for i in range(0,10): # Do ten times
                average+=real[i] # Add all real weight values in array
                averageRaw+=raw[i] # Add all raw values in array
            average=average/10 # all real / 10 for average
            averageRaw=average/10 # all raw / 10 for average
            averageRaw = int(round(averageRaw))
            averageRaw = int(round(average))
            print(averageRaw + ',' + average) # Print what is about to be sent
            response = send_to_api_gateway(averageRaw,average) # Send to AWS
            print(response) # Print the response from AWS

    elif n=='5':
        print('Upload Twenty-Five Times\n')

        i=0
        for i in range(0,25) # Do twenty five times

            i=0
            # Get ten values and write to array
            for i in range(0,10): # Do ten times
                real[i]=hx.getWeight() # Get weight value
                raw[i]=hx.getValue() # Get raw value

            i=0
            # Average values in array by adding them up and dividing 10 (the amount of values)
            for i in range(0,10): # Do ten times
                average+=real[i] # Add all real weight values in array
                averageRaw+=raw[i] # Add all raw values in array
            average=average/10 # all real / 10 for average
            averageRaw=average/10 # all raw / 10 for average
            averageRaw = int(round(averageRaw))
            averageRaw = int(round(average))
            print(averageRaw + ',' + average) # Print what is about to be sent
            response = send_to_api_gateway(averageRaw,average) # Send to AWS
            print(response) # Print response from AWS

    elif n=='6':
        print('Continuous Upload\n')
        true = True
        While true
            for i in range(0,10): # Do ten times
                real[i]=hx.getWeight() # Get weight value
                raw[i]=hx.getValue() # Get raw value

            for i in range(0,10): # Do ten times
                average+=real[i] # Add all real weight values in array
                averageRaw+=raw[i] # Add all raw values in array
            average=average/10 # all real / 10 for average
            averageRaw=average/10 # all raw / 10 for average
            averageRaw = int(round(averageRaw))
            averageRaw = int(round(average))
            print(averageRaw + ',' + average) # Print what is about to be sent
            response = send_to_api_gateway(averageRaw,average) # Send to AWS
            print(response) # Print response from AWS

    elif n=='5':
        GPIO.cleanup()
        break