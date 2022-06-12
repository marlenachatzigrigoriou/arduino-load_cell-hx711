/*
   HX711 Analog-to-Digital Converter for Weight Scales
*/

#include <HX711_ADC.h>


//pins:
const int HX711_dout = 4; //arduino 4 -> load cell module dout pin
const int HX711_sck = 5; //arduino 5 -> load cell module sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  float calibrationValue; 
  calibrationValue = 15904.80; // the calibration value we found that is needed for our load cell
  unsigned long stabilizingtime = 2000; 
  
  boolean _tare = true; 
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  if (LoadCell.update()) newDataReady = true; //checks for new data and starts the next conversion

  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

}
