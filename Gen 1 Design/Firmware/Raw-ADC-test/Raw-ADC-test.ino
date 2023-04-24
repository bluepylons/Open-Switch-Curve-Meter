// This is from one of the Examples in the HX711 Arduino library, and used for getting raw ADC values out of the HX711 load cell, for calibration
//This uses Rob Tillart's HX711 library: https://github.com/RobTillaart/HX711

#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;
int flag = 0;

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {

  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.println(reading);
  } 

/*
  else {
    flag = 1;
    Serial.println("HX711 not found.");
  }
*/
  delay(100);
  
}
