#include <Arduino.h>
#include <Wire.h>
#include "adp5360.h"

#define I2C_ADDR 0x40

ADP5360 battery(Wire, I2C_ADDR);

void setup() {
    Serial.begin(115200);
    battery.begin();

    uint16_t voltage = battery.readBatteryVoltage();
    Serial.print("battery voltage (mV): ");
    Serial.println(voltage);
}

void loop() {
    delay(1000);
}
