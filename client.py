#! /bin/python3

import ssl
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

MQTT_HOST = 'raspberrypi.lan'
MQTT_PORT = 8883
MQTT_CA   = './ca.crt'

INFLUX_HOST = 'localhost'
INFLUX_PORT = 8086
INFLUX_USER = 'admin'
INFLUX_PASS = '1234567890'
INFLUX_DBNAME = 'test'

INCENDIO_MSG = b"II"


def accion_incendio(id):
    print(f"Incendio en {id}!")

def on_connect(client, userdata, flags, rc):
    client.subscribe("baliza/+/incendio")

def on_message(client, userdata, msg):
    baliza_id = msg.topic.split('/')[1]

    if msg.payload == INCENDIO_MSG:
        influx_msg = [
            {
                "measurement": "incendio",
                "tags": {   
                    "baliza": str(baliza_id),
                },
                "fields": {
                    "incendio": True,
                }
            }
        ]
        try:
            client = InfluxDBClient(INFLUX_HOST, INFLUX_PORT, INFLUX_USER, INFLUX_PASS, INFLUX_DBNAME)
            client.write_points(influx_msg)
            #print("write: status =", client.write_points(influx_msg))
        except Exception as err:
            print(err)

        accion_incendio(baliza_id)


if __name__ == "__main__":
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.tls_set(ca_certs=MQTT_CA, cert_reqs=ssl.CERT_REQUIRED, tls_version=ssl.PROTOCOL_TLSv1_2)
    # client.tls_insecure_set(True)
    client.connect(MQTT_HOST, MQTT_PORT, 60)

    client.loop_forever()
