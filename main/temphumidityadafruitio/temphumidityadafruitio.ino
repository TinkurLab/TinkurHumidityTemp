/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"
#include "secrets.h"

// DHT 11 sensor
#define DHTPIN D2
#define DHTTYPE DHT22 

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup feeds for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME AIO_HUM_FEED);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME AIO_TEMP_FEED);
Adafruit_MQTT_Publish voltage = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME AIO_VOLTAGE_FEED);


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

// Variables for battery voltage
int BAT= A0;              //Analog channel A0 as used to measure battery voltage
float RatioFactor=5.714;  //Resistors Ration Factor

void setup() {
  // Init DHT22 sensor
  dht.begin();
  
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Starting TinkurHumidityTemp..."));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  Serial.println("Feed IDs");
  Serial.println(AIO_HUM_FEED);
  Serial.println(AIO_TEMP_FEED);
  Serial.println(AIO_VOLTAGE_FEED);
  Serial.println();
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  // Grab the current state of the sensor
  int humidity_data = (int)dht.readHumidity(); // percent
  int temperature_data = (int)dht.readTemperature(); // degrees C

  temperature_data = (temperature_data * 9.0)/ 5.0 + 32.0; // convert to degrees F

  int value = LOW;
  float Tvoltage=0.0;
  float Vvalue=0.0,Rvalue=0.0;

  // Get battery voltage 
  for(unsigned int i=0;i<10;i++){
  Vvalue=Vvalue+analogRead(BAT);         //Read analog Voltage
  delay(5);                              //ADC stable
  }
  Vvalue=(float)Vvalue/10.0;            //Find average of 10 values
  Rvalue=(float)(Vvalue/1024.0)*5;      //Convert Voltage in 5v factor
  Tvoltage=Rvalue*RatioFactor;          //Find original voltage by multiplying with factor
  
  value = HIGH;

  // Publish humidity to Adafruit IO
  Serial.print(F("\nSending humidity val "));
  Serial.print(humidity_data);
  Serial.print("...");
  if (! humidity.publish(humidity_data)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // Publish temperature to Adafruit IO
  Serial.print(F("\nSending temperature val "));
  Serial.print(temperature_data);
  Serial.print("...");
  if (! temperature.publish(temperature_data)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // Publish voltage to Adafruit IO
  Serial.print(F("\nSending voltage val "));
  Serial.print(Tvoltage);
  Serial.print("...");
  if (! voltage.publish(Tvoltage)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */

  delay(60000);  //update 1x a minute = 60000
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
