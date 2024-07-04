import paho.mqtt.client as paho
import time
import json
from random import randint

ACCESS_TOKEN = "IHySAkLlhsHLTZcSgG0C"
broker = "thingsboard.cloud"
port = 1883

def on_publish(client, userdata, result):
    print("data published to thingsboard \n")
    pass

client1 = paho.Client("controll", protocol=paho.MQTTv311)
client1.on_publish = on_publish
client1.username_pw_set(ACCESS_TOKEN)
client1.connect(broker, port, keepalive=60)

while True:
    
    payload = {
        "Humidity": randint(20,80),
        "Temperature": randint(20,50)
    }
    payload = json.dumps(payload)
    ret = client1.publish("v1/devices/me/telemetry", payload)
    print("Please check LATEST TELEMETRY field of your device")
    print(payload)
    time.sleep(5)
