# Tinkur Humidity and Temp

## Overview

Project to measuring humidity and temperature using an ESP82666MOD and DHT22 sensor.

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
2. Setup Arduino IDE for ESP8266:
  * sdf
  * asdf
  * asdf
3. 
