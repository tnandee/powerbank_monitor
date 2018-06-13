#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <avr/sleep.h>

Adafruit_INA219 ina219;

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiAvrI2c oled;

long previousMillis = 0;
long rfshInterval = 2000;
long turnOffTimeout = 15000;

void setup() {
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    oled.setFont(Adafruit5x7);
    oled.clear();

    // Initialize the INA219.
    // By default the initialization will use the largest range (32V, 2A).  However
    // you can call a setCalibration function to change this range (see comments).
    ina219.begin();
    // To use a slightly lower 32V, 1A range (higher precision on amps):
    //ina219.setCalibration_32V_1A();
    // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
    //ina219.setCalibration_16V_400mA();
}

void loop() {
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;

    unsigned long currentMillis = millis();
  
    if (currentMillis - previousMillis > rfshInterval) {
        previousMillis = currentMillis;
        shuntvoltage = ina219.getShuntVoltage_mV();
        busvoltage = ina219.getBusVoltage_V();
        current_mA = ina219.getCurrent_mA();
        loadvoltage = busvoltage + (shuntvoltage / 1000);
        power_mW = ina219.getPower_mW();  
        
        oled.setCursor(0,0);
        oled.set2X();
        oled.println("|PWR meter|");
        oled.print(loadvoltage); oled.println(" V");
        oled.print(current_mA); oled.println(" mA");
        oled.print(power_mW); oled.println(" mW");
    }
    if (currentMillis > turnOffTimeout) {
      oled.clear();
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_mode();
    }
}
