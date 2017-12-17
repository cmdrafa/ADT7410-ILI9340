#include <Arduino.h>
#include <Wire.h>

// Define ADT7410 address and Register Addresses. ***This assumes A0 and A1 are
// grounded.***
#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

int tempReading = 0;

float finalTempC = 0.0000;

void ADT7410INIT();
void ADT7410GetTemp();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  ADT7410INIT();
}

void loop() {
  ADT7410GetTemp();
  finalTempC = (tempReading / 128.0);
  Serial.print(finalTempC);
  Serial.println('C');
  delay(500);
}

void ADT7410INIT() {
  // Initialization of the ADT7410 sets the configuration register based on
  // input from the  Analog Devices datasheet page 14. 16-bit resolution
  // selected.
  Wire.beginTransmission(B1001000);
  Wire.send(0x03);
  Wire.send(B10000000);
  Wire.endTransmission();
}

void ADT7410GetTemp() {
  byte MSB;
  byte LSB;
  // Send request for temperature register.
  Wire.beginTransmission(ADT7410Address);
  Wire.send(ADT7410TempReg);
  Wire.endTransmission();
  // Listen for and acquire 16-bit register address.
  Wire.requestFrom(ADT7410Address, 2);
  MSB = Wire.receive();
  LSB = Wire.receive();
  // Assign global 'tempReading' the 16-bit signed value.
  tempReading = ((MSB << 8) | LSB);
}
