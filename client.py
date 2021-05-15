#! /bin/python3

import ssl
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

## Config
MQTT_HOST = 'raspberrypi.lan'
# MQTT_PORT = 8883
MQTT_PORT = 1883
MQTT_CA   = '/home/pi/client/ca.crt'

INFLUX_HOST = 'raspberrypi.lan'
INFLUX_PORT = 8086
INFLUX_USER = 'admin'
INFLUX_PASS = '1234567890'
INFLUX_DBNAME = 'test'

## Topic actions
def fun_incendio(baliza_id, topic, payload):
    print(f"Incendio detectado en baliza {baliza_id}")

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
    except Exception as err:
        print("InfluxDB error:", err)

def fun_temperatura(baliza_id, topic, payload):
    sensor_num = topic.split('/')[-1]
    print(f"Temperatura recibida desde balida {baliza_id} - sensor {sensor_num}: {payload}")

    influx_msg = [
        {
            "measurement": "temperatura",
            "tags": {
                "baliza": str(baliza_id),
                "sensor": str(sensor_num),
            },
            "fields": {
                "value": payload,
            }
        }
    ]

    try:
        client = InfluxDBClient(INFLUX_HOST, INFLUX_PORT, INFLUX_USER, INFLUX_PASS, INFLUX_DBNAME)
        client.write_points(influx_msg)
    except Exception as err:
        print("InfluxDB error:", err)

def fun_humedad(baliza_id, topic, payload):
    sensor_num = topic.split('/')[-1]
    print(f"Humedad recibida desde balida {baliza_id} - sensor {sensor_num}: {payload}")

    influx_msg = [
        {
            "measurement": "humedad",
            "tags": {
                "baliza": str(baliza_id),
                "sensor": str(sensor_num),
            },
            "fields": {
                "value": payload,
            }
        }
    ]

    try:
        client = InfluxDBClient(INFLUX_HOST, INFLUX_PORT, INFLUX_USER, INFLUX_PASS, INFLUX_DBNAME)
        client.write_points(influx_msg)
    except Exception as err:
        print("InfluxDB error:", err)

def fun_gases(baliza_id, topic, payload):
    sensor_num = topic.split('/')[-1]
    print(f"Resistencia gases recibida desde balida {baliza_id} - sensor {sensor_num}: {payload}")

    influx_msg = [
        {
            "measurement": "gases",
            "tags": {
                "baliza": str(baliza_id),
                "sensor": str(sensor_num),
            },
            "fields": {
                "value": payload,
            }
        }
    ]

    try:
        client = InfluxDBClient(INFLUX_HOST, INFLUX_PORT, INFLUX_USER, INFLUX_PASS, INFLUX_DBNAME)
        client.write_points(influx_msg)
    except Exception as err:
        print("InfluxDB error:", err)

## Topics
TOPICS = {
    'incendio': ("baliza/+/incendio", fun_incendio),
    'temperatura': ("baliza/+/Datos_Baliza/temperatura/+", fun_temperatura),
    'humedad': ("baliza/+/Datos_Baliza/humedad/+", fun_humedad),
    'gases': ("baliza/+/Datos_Baliza/gases/+", fun_gases),
}

# def accion_incendio(id):
#     print(f"Incendio en {id}!")

def on_connect(client, userdata, flags, rc):
    for i, v in TOPICS.items():
        client.subscribe(v[0])

def on_message(client, userdata, msg):
    baliza_id = msg.topic.split('/')[1]

    for key in TOPICS.keys():
        if key.lower() in msg.topic.lower():
            fun = TOPICS[key][1]
            if fun is not None:
                fun(baliza_id, msg.topic, msg.payload)
            else:
                print("Topic:", msg.topic, "Payload:", msg.payload)


if __name__ == "__main__":
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    # client.tls_set(ca_certs=MQTT_CA, cert_reqs=ssl.CERT_REQUIRED, tls_version=ssl.PROTOCOL_TLSv1_2)
    # client.tls_insecure_set(True)
    client.connect(MQTT_HOST, MQTT_PORT, 60)

    print('Starting MQTT loop')
    client.loop_forever()
