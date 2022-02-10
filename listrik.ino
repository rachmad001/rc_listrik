#include <PZEM004Tv30.h>
#include <ArduinoJson.h>
#include "ACS712.h"

PZEM004Tv30 pzem(11, 12);
//ACS712 sensor(ACS712_30A, A0);

//initialize variable for voltage sensor
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;

//initialize variable for acs712
//int mVperAmp = 66;
//int RawValue = 0;
//int ACSoffset = 2500;
//double Voltage = 0;
//double Amps = 0;

void setup() {
  Serial.begin(9600);
//  sensor.calibrate();
}

void loop() {
  //get voltage from voltage sensor 
  value = analogRead(A1); //A1 -> PIN VOLTAGE SENSOR
  vOUT = (value * 5.0) / 1024.0;
  vIN =  vOUT / (R2/(R1+R2));

  //get acs712 value
  unsigned int x=0;
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;
    for (int x = 0; x < 150; x++){ //Get 150 samples
    AcsValue = analogRead(A0);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
  }
  AvgAcs=Samples/150.0;
  AcsValueF = (2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.066;
  
  //get value sensor from pzem
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  if (voltage != NAN && current != NAN) {
    StaticJsonDocument<200> doc;
    doc["dc"]["voltage"] = vIN;
    doc["dc"]["current"] = AcsValueF * 0.6;
    doc["ac"]["voltage"] = voltage;
    doc["ac"]["current"] = current;
    doc["ac"]["power"] = power;
    doc["ac"]["energy"] = energy;
    doc["ac"]["frequency"] = frequency;
    doc["ac"]["pf"] = pf;
    String data = "";
    serializeJson(doc, data);
    Serial.println(data);
  }
  delay(50);
}
