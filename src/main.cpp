#include <Arduino.h>
#include <BPMonoItalic.h>
#include <DSPI.h>
#include <DisplayCore.h>
#include <ILI9340.h>
#include <Wire.h>

#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

DSPI0 spi;

ILI9340 tft(spi, 7, 6, 5); // cd dc rst

int tempReading = 0;

float finalTempC = 0.0000;

void ADT7410INIT();
void ADT7410GetTemp();

void setup() {
  // ADT
  Serial.begin(9600);
  Wire.begin();
  ADT7410INIT();

  // Screen
  tft.initializeDevice();
  tft.fillScreen(Color::Red);
  tft.setTextColor(Color::White, Color::Black);
  tft.setFont(Fonts::BPMonoItalic22);
}

void loop() {
  // Function for reading the temp
  ADT7410GetTemp();

  tft.setCursor(50, 40);
  finalTempC = (tempReading / 128.0);
  tft.print(finalTempC);
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
