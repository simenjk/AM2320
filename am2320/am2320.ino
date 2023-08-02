#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();
float t, h;
long lastMsg = 0;

const char* ssid = "xxx";
const char* pass = "xxx";
const char* mqtt_URL = "mqtt-dashboard.com";
const int mqtt_port = 8884;
const char* mqtt_username = "xxx";
const char* mqtt_pass = "xxx";

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  
  delay(100);
  Serial.begin(9600);
  pinMode(32, OUTPUT);
  
  while(!Serial) {
    Serial.print("Waiting for serial.");
    delay(5);
  }

  Serial.print("Serial connected.");
  am2320.begin();
  connect_wifi();

  // Set MQTT Server details 
  client.setServer(mqtt_URL, mqtt_port);
  client.setCallback(callback);

}

void loop() {
  // Connecting to MQTT
  if(!client.connected()){
    reconnect();
  }

  // Delay between sending messages
  long now = millis();
  long previous_time = 0;
  
  if  (now- lastMsg > 10000){
    lastMsg = now;

    // Prints out temperature and humidity to Serial and sends data with MQTT
    float temperature = am2320.readTemperature();
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("sensors/temperature", tempString);
 
    float humidity = am2320.readHumidity();
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("sensors/humidity", humString);
  }

/*
// LED lights up if temperature is too high.
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
  */
}

