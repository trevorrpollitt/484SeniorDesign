import sys
import time
import requests

import RPi.GPIO as GPIO
from hx711 import HX711


# choose pins on rpi (BCM5 and BCM6)
hx = HX711(dout=5, pd_sck=6)

# HOW TO CALCULATE THE REFFERENCE UNIT
#########################################
# To set the reference unit to 1.
# Call get_weight before and after putting 1000g weight on your sensor.
# Divide difference with grams (1000g) and use it as refference unit.

hx.setReferenceUnit(1)

def average(array,times):
    average = 0 # Define local variable

    for i in range(0,times): # Do x times

        average = average/10 # all raw / 10 for average
        average = int(round(average))

    return average

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

def bordered(text):
    lines = text.splitlines()
    width = max(len(s) for s in lines) + 2
    x = int((width)/2)
    res = ['┼' + '─' * x + '//' + '─' * (width-x)  + '┼']
    for s in lines:
        res.append('│' + ('  ' + s + ' ' * width)[:width] + '  ' + '│')
    res.append('┼' + '─' * x + '//' + '─' * (width-x)  + '┼' + '\n')
    return '\n'.join(res)

hx.reset()
hx.tare()

while True:
    
    ####### INIT #######
    raw = [0,0,0,0,0,0,0,0,0,0]
    real= [0,0,0,0,0,0,0,0,0,0]
    data = []
    uploadData = {}
    value = 0
    ####### INIT #######

    n=input(bordered('1: Calibration\n2: Show Raw Data\n3: Show Real Data\n4: Fast Upload\n5: Variable Upload\n6: Continuous Upload\n7: Exit Program\nChoose a selection\n'))
    
    if n=='1':
        print('Calibrating...\n')
        x=hx.tare # Calibrates the ADC with our reference value and zero value
        print(x)

    elif n=='2':
        print('Gathering Raw Data...\n')
        true=True
        while true:
            try:
                value= hx.getValue() # Get raw value
                value = int(round(value)
                print(value)
            except (KeyboardInterrupt):
                true=False
            
    elif n=='3':
        print('Gathering Real Data...\n')
        true=True
        while true:
            try:
                value=hx.getWeight() # Get real value
                value = int(round(value)
                print(value)
            except (KeyboardInterrupt):
                true=False
    elif n=='4':
        m = input(bordered('1: Fast Upload\n2: Variable Upload\n3: Continuous Upload\n4: Back\nChoose a selection...\n'))
            
        if m == '1':
            print('Performing Quick Upload...\n')

            i=0
            # Get ten values to average
            for i in range(0,10): # Do ten times
                real[i]=hx.getWeight() # Get weight value
                raw[i]=hx.getValue() # Get raw value

            i=0
            # Average vlaues
            averageRaw = average(raw,10) # average the raw values
            averageReal = average(real,10) # average the real values
            print("Raw Value = ", averageRaw, ", Real Value = ", averageReal) # Print what is about to be sent
            response = send_to_api_gateway(averageRaw,averageReal) # Send to AWS
            print(response) # Print response from AWS
            
        elif m == '2':
            uploadTimes = input('How many times would you like to upload? ')
            uploadTimes = int(uploadTimes)

            i=0
            for i in range(0,uploadTimes): # Do x times

                i=0
                # Get ten values to average
                for i in range(0,10): # Do ten times
                    real[i]=hx.getWeight() # Get weight value
                    raw[i]=hx.getValue() # Get raw value
                    
                i=0
                # Average values
                averageRaw = average(raw,10) # average the raw values
                averageReal = average(real,10) # average the real values
                print("Raw Value = ", averageRaw, ", Real Value = ", averageReal) # Print what is about to be sent
                response = send_to_api_gateway(averageRaw,averageReal) # Send to AWS
                print(response) # Print the response from AWS

        elif m == '3':
            true = True
            while true:
                try:
                    i=0
                    # Get ten values to average
                    for i in range(0,10): # Do ten times
                        real[i]=hx.getWeight() # Get weight value
                        raw[i]=hx.getValue() # Get raw value

                    i=0
                    # Average vlaues
                    averageRaw = average(raw,10) # average the raw values
                    averageReal = average(real,10) # average the real values
                    print("Raw Value = ", averageRaw, ", Real Value = ", averageReal) # Print what is about to be sent
                    response = send_to_api_gateway(averageRaw,averageReal) # Send to AWS
                    print(response) # Print response from AWS

                except (KeyboardInterrupt):
                    true=False

    elif n=='6':
        GPIO.cleanup()
        break