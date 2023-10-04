#!/bin/python3

import serial
from time import sleep
import sys
import threading
import json

from jsonrpcclient import request_json, parse, Ok
import logging
# import requests


COM = 'COM5'# /dev/ttyACM0 (Linux)
BAUD = 115200

ser = serial.Serial(COM, BAUD, timeout = .1)

print('{python}', 'Waiting for device');
sleep(3)
print('{python}', ser.name)

#check args
# if("-m" in sys.argv or "--monitor" in sys.argv):
# 	monitor = True
# else:
# 	monitor= False

def captureSerial():
    while True:
        try:
            val = str(ser.readline().decode().strip('\r\n'))#Capture serial output as a decoded string
            if (val != ''):
                print('{esp}', val, end='\n', flush=True)
                json_object = json.loads(val)
                parsed = parse(json_object)
                if isinstance(parsed, Ok):
                    print(parsed.result)
                else:
                    logging.error(parsed.message)
        except Exception as e:
            print('{python}', "Error at captureSerial : ", e)
        # val = val.split("/")




t1 = threading.Thread(target=captureSerial)
t1.start()
while True:

    # response = requests.post("http://localhost:5000/", json=request("ping"))


    request_str = request_json("ping")
    print('{python}', request_str)
    ser.write(str(request_str).encode())
    sleep(5)
    #request_str = request_json("status")
    #print('{python}', request_str)
    #ser.write(str(request_str).encode())
    #sleep(5)
