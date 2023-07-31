// 

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320();

void setup() {

  Serial.begin(9600);
  
  while(!Serial) {
    Serial.print("Waiting for serial.");
    delay(5);
  }

  Serial.print("Serial connected.");
  am2320.begin();
  
}




void loop() {
  Serial.print("Temp: ");
  Serial.print(am2320.readTemperature());
  Serial.print(" C");
  Serial.print("\t\t");
  Serial.print("Humidity: ");
  Serial.print(am2320.readHumidity());
  Serial.println(" \%");

  delay(2000);
}
