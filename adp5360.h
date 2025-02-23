#ifndef ADP5360_H
#define ADP5360_H

#include <Arduino.h>
#include <Wire.h>

class ADP5360 {
public:
    ADP5360(TwoWire &wire, uint8_t i2cAddress);

    bool begin();  
    uint16_t readBatteryVoltage();

private:
    TwoWire* _wire;
    uint8_t _i2cAddress;

    uint8_t readRegister(uint8_t reg);
};

#endif
