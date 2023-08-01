#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();
float t, h;

const char* ssid = "xxx";
const char* pass = "xxx";
const char* mqtt_URL = "mqtt-dashboard.com";
const int mqtt_port = 8884;
const int mqtt_socket_port = 8884;
const char* mqtt_username = "xxx";
const char* mqtt_pass = "xxx";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;


void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!client.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      Serial.print(".");
      
      if (client.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        client.subscribe("sensors/message");
      }      
  }
}

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

  if(!client.connected()){
    reconnect();
  }

  //StaticJsonDocument<80> doc;
  char output[80];

  // Delay between sending messages
  long now = millis();
  long previous_time = 0;
  
  if  (now- lastMsg > 10000){
    lastMsg = now;

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

    // Add variables to JSON document 
    //doc["t"] = temp;
    //doc["h"] = humidity;

    //serializeJson(doc, output);
    //Serial.print(output);
    //client.publish("/home/sensors", output);


  }
/*

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

  void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
}
