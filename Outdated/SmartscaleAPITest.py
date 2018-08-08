import json
import requests
import datetime
from random import *

# program data to upload
uploadData = {}

#generate random number b/w [1,300] and add it to randData list 10 times
def generateNumbersInList():
    numList = [randint(1,300000),randint(1,300)]
    
    uploadData['value'] = numList

#def prepareJsonObject():
    
#    generateNumbersInList()
#    jsonObj = json.dumps(uploadData,indent=4)
#    return jsonObj

# Push to API
def send_to_api_gateway(observations):
    PROJ5_API_URL = "https://4d5hwp29ll.execute-api.us-east-1.amazonaws.com/development/eagleio"
	
    payload = observations

    resp = requests.put(PROJ5_API_URL, json=payload, headers={"Content-Type": "application/json"})

    if not 200 <= resp.status_code < 300:
        print("Failed to post to AWS API with status %d" % resp.status_code)
        raise Exception("Failed to post to AWS API with status %d" % resp.status_code)

    return resp

# this is like the main() method. AWS requires 2 parameters defined in this function
def lambda_handler(json_input, context):
    generateNumbersInList()
    print(uploadData)
    returnData = send_to_api_gateway(uploadData)
    print(returnData.text)
    return uploadData
    
# must define function before calling it
lambda_handler('blah', 'blah')