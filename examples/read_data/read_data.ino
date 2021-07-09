/*
Reads the data from a dell charger using OneWire and parses the wattage, voltage, and current

Connect the center pin of the Dell power supply to the desired Arduino I/O pin
Add a 4.7K pull-up resistor between the tiny center pin of the Dell power supply and the Arduino +5V rail 
Connect the ground pin of the Dell power supply to the Arduino GND
Do not connect the 19.5V pin of the Dell power supply to the Arduino!!!! 
*/

#include "dell_psu.h"

DellPSU dell(1);   //specify the desired Arduino pin number  
  
void setup() {
  Serial.begin (115200);
  while(!Serial); 
}

void loop() {
  // NOTE: by default, we allow reading of non-genuine/3rd-party DELL 'compatible' adapters.
  // if you wish to disable this, and only read DELL original adapters, replace the
  // 'dell.read_data()' call with 'dell.read_data(false)'
  if (dell.read_data()==true)
  {
    Serial.write(dell.raw_response(), DELL_PSU_BYTES_TO_READ);
    Serial.println();
    Serial.print(dell.watts());
    Serial.print("W "); 
    Serial.print(dell.millivolts());
    Serial.print("mv ");
    Serial.print(dell.milliamps());
    Serial.println("mA "); 
  }
  else
  {
    Serial.print(".");
  }
  delay(1000);
 
}

















