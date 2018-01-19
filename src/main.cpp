#include <Arduino.h>
#include <BPMonoItalic.h>
#include <DSPI.h>
#include <Display7Seg.h>
#include <DisplayCore.h>
#include <ILI9340.h>
#include <Wire.h>

/* COMP PROJECT 2017-2018 Francisco João Cunha Dias and Rafael Magalhães Gomes
Kraemer Demonstration of SPI, I2C and UART communictaion protocols, interfacing
with ADT7410 and an ILI9340 screen. For UART demonstration purposes, a plot with
real time data will be made using an external program.
*/

/* Defining the custom ports in the microcontroller regarding the
ILI9340 tft screen*/
#define cs 7
#define dc 6
#define rst 5

// Defining the register addresses on the ADT7410 according to the datasheet
#define ADT7410Address 0x48
#define ADT7410TempReg 0x00
#define ADT7410ConfigReg 0x03

DSPI0 spi;
ILI9340 tft(spi, cs, dc, rst);

int tempReading = 0;
float finalTempC = 0.0000;

// the "executed" boolean is a "checker" for avoiding the constant execution of
// an UI function, making the microcontroller SLOW and unresponsive.
bool executed = false;

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

  // SCREEN - SPI - Default initialization parameters
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
  /* This function will generate a "HOT UI" based on the temperature fetched
  from the sensor and a limit established in the loop function.
  */
  tft.fillScreen(Color::Red);
  tft.setTextColor(Color::White, Color::Black);
  tft.fillCircle(120, 160, 80, Color::White);
  tft.setFont(Fonts::BPMonoItalic22);
  tft.setCursor(20, 20);
  tft.print("COMP 2017-2018");
}

void generateUICool() {
  /* This function will generate a "COOL UI" based on the temperature fetched
  from the sensor and a limit established in the loop function.
  */
  tft.fillScreen(Color::NavyBlue);
  tft.setTextColor(Color::White, Color::Black);
  tft.fillCircle(120, 160, 80, Color::White);
  tft.setFont(Fonts::BPMonoItalic22);
  tft.setCursor(20, 20);
  tft.print("COMP 2017-2018");
}

void ADT7410INIT() {
  /*Function for initialization of the ADT7410, sending the parameters to the
  registers specified in the datasheet
  */
  Wire.beginTransmission(B1001000);
  Wire.send(0x03);
  Wire.send(B10000000);
  Wire.endTransmission();
}

void ADT7410GetTemp() {
  /* Function for getting the temperature from ADT7410, called in the "loop"
  according to the delay time. Everytime this is called a new temperature value
  is fetched from the ADT7410.
  */
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
