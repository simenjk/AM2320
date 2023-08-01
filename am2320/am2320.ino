#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();
float t, h;

const char* ssid = "xxx";
const char* pass = "xxx";
const char* mqtt_URL = "xxx";
const int mqtt_port = 8883;
const int mqtt_socket_port = 8884;

void setup() {
  
  Serial.begin(9600);
  pinMode(32, OUTPUT);
  
  while(!Serial) {
    Serial.print("Waiting for serial.");
    delay(5);
  }

  Serial.print("Serial connected.");
  am2320.begin();
  connect_wifi();
}

void loop() {

  if (am2320.readTemperatureAndHumidity(&t, &h)) {
    if (t > 25){
      digitalWrite(32, HIGH);
      Serial.print("High Temperature "); Serial.println(t);
      Serial.print("Humidity: "); Serial.println(h);
      delay(1000);
  }
    else{
    Serial.print("Temp: "); Serial.println(t);
    Serial.print("Humidity: "); Serial.println(h);
    delay(1000);
    }
  }
}

