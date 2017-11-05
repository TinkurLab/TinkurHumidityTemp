# Tinkur Humidity and Temp

## Overview

Project to measuring humidity and temperature using an ESP82666MOD and DHT22 sensor.

## Hardware

* HiLetgo ESP8266 NodeMCU https://www.amazon.com/gp/product/B010O1G1ES/
* DHT22 Humidity + Temp Sensor https://www.adafruit.com/product/385
* 10K resistor 
* power supply, usb cable, hookup supplies

## Setup

### Setup Secrets

Create a 'secrets.h' file in the Sketch's root director with the following contents:

~~~
#define WLAN_SSID       "WiFiSSID"
#define WLAN_PASS       "WiFiPassword"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "AdafruitIOUsername"
#define AIO_KEY         "AdafruitIOKey"
~~~

This file is ignored by Git.

### Setup Ardunino IDE

1. Download and install any Ardunio libraries as needed.
2. Setup Arduino IDE under Tools menu:
  * Board: NodeMCU 1.0 (ESP=12E Module)
  * CPU Frequency: 80MHz
  * Flash Size: 4M (3M SPIFFS)
  * Upload Speed: 115200
  * Port: 
3. Compole and Upload Sketch
