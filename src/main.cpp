#include <Arduino.h>
#include <BPMonoItalic.h>
#include <DSPI.h>
#include <Display7Seg.h>
#include <DisplayCore.h>
#include <ILI9340.h>
#include <Wire.h>

#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

DSPI0 spi;

ILI9340 tft(spi, 7, 6, 5); // cs dc rst

int tempReading = 0;
bool executed = false;
float finalTempC = 0.0000;

void ADT7410INIT();
void ADT7410GetTemp();
void generateUIHot();
void generateUICool();

void setup() {
  // ADT - I2C
  Wire.begin();
  ADT7410INIT();

  // Serial Communication for plotting data
  Serial.begin(9600);

  // SCREEN - SPI
  tft.initializeDevice();
  tft.fillScreen(Color::NavyBlue);
  tft.setTextColor(Color::White, Color::Black);
  tft.fillCircle(120, 160, 80, Color::White);
  tft.setFont(Fonts::BPMonoItalic22);
  tft.setCursor(20, 20);
  tft.print("COMP 2017-2018");
}

void loop() {
  ADT7410GetTemp();
  finalTempC = (tempReading / 128.0);

  if (finalTempC > 19 && executed == false) {
    generateUIHot();

    executed = true;
  }

  if (finalTempC <= 19 && executed == true) {
    generateUICool();

    executed = false;
  }

  tft.setTextColor(Color::NavyBlue, Color::White);
  tft.setCursor(60, 150);
  tft.setFont(Fonts::Display7Seg32);
  tft.print(finalTempC);
  Serial.println(finalTempC);
  tft.setFont(Fonts::BPMonoItalic30);
  tft.print(" C");
  delay(500);
}

void generateUIHot() {
  tft.fillScreen(Color::Red);
  tft.setTextColor(Color::White, Color::Black);
  tft.fillCircle(120, 160, 80, Color::White);
  tft.setFont(Fonts::BPMonoItalic22);
  tft.setCursor(20, 20);
  tft.print("COMP 2017-2018");
}

void generateUICool() {
  tft.fillScreen(Color::NavyBlue);
  tft.setTextColor(Color::White, Color::Black);
  tft.fillCircle(120, 160, 80, Color::White);
  tft.setFont(Fonts::BPMonoItalic22);
  tft.setCursor(20, 20);
  tft.print("COMP 2017-2018");
}

void ADT7410INIT() {
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
