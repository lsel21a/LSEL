# LSEL2021
Implementando LoRa con MQTT, para comunicarse con un servidor en la nube, que controle los parámetros de gases, temperatura y humedad. Si estos sensores o el operador detectan algo fuera de lo normal, un dron equipado con una cámara está preparado para sobrevolar la zona y utilizar algoritmos de inteligencia artificial para reconocer si es un incendio y cuales son las zonas más afectadas, para que los equipos de bomberos puedan llegar a las coordenadas más afectadas lo antes posible.

*Este prototipo no usa lora ya que la ESP32 que utilizamos para el desarrollo no tenia conectividad Lora, asi que el protocolo de comunicación es WiFi con MQTT*

[![codecov](https://codecov.io/gh/lsel21a/LSEL/branch/master/graph/badge.svg?token=X4B7AYNMRT)](https://codecov.io/gh/lsel21a/LSEL)
[![baliza - Test and code coverage](https://github.com/lsel21a/LSEL/actions/workflows/test_baliza.yml/badge.svg?branch=master)](https://github.com/lsel21a/LSEL/actions/workflows/test_baliza.yml)
