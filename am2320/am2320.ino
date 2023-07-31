// 

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();

float t, h;

void setup() {

  Serial.begin(9600);
  pinMode(32, OUTPUT);
  
  while(!Serial) {
    Serial.print("Waiting for serial.");
    delay(5);
  }

  Serial.print("Serial connected.");
  am2320.begin();
  
}

void loop() {

  if (am2320.readTemperatureAndHumidity(&t, &h)) {
    if (t > 20){
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

