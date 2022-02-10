#include "AntaresESP8266HTTP.h"

#define ACCESSKEY "efc52fbb21322456:db196a65b2521708"
#define WIFISSID "testing"
#define PASSWORD "testing123"

#define projectName "light"
#define deviceName "arduinowifi"

AntaresESP8266HTTP antares(ACCESSKEY);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("start");
  antares.setDebug(false);
  antares.wifiConnectionNonSecure(WIFISSID,PASSWORD);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
    const int capacity = JSON_OBJECT_SIZE(1000);
    StaticJsonBuffer<capacity> doc;
    JsonObject& obj = doc.parseObject(data);
    if(obj.success()) {
      float vIN = obj["dc"]["voltage"];
      float dcCurrent = obj["dc"]["current"];
      float voltage = obj["ac"]["voltage"];
      float current = obj["ac"]["current"];
      float power = obj["ac"]["power"];
      float energy = obj["ac"]["energy"];
      float frequency = obj["ac"]["frequency"];
      float pf = obj["ac"]["pf"];
      antares.add("dc", "voltage", vIN);
      antares.add("dc", "current", dcCurrent);
      antares.add("ac", "voltage", voltage);
      antares.add("ac", "current", current);
      antares.add("ac", "power", power);
      antares.add("ac", "energy", energy);
      antares.add("ac", "frequency", frequency);
      antares.add("ac", "pf", pf);
      antares.sendNonSecure(projectName, deviceName);
    }
    else {
      Serial.println("error");
    }
  }
}
