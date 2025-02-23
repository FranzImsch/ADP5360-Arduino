#include "adp5360.h"

#define REG_HI 0x25  // High-Byte (Bits 12:5)
#define REG_LO 0x26  // Low-Byte (Bits 4:0, Bits 2:0 reserviert)

// Konstruktor: Speichert das Wire-Objekt und die I2C-Adresse
ADP5360::ADP5360(TwoWire &wire, uint8_t i2cAddress)
    : _wire(&wire), _i2cAddress(i2cAddress) {}

bool ADP5360::begin() {
    _wire->begin();  // I2C starten (muss vorher aufgerufen werden, falls nicht schon gestartet)
    return true;  // Falls spätere Initialisierungen notwendig sind, kann hier eine Prüfung erfolgen
}

uint16_t ADP5360::readBatteryVoltage() {
    uint8_t high = readRegister(REG_HI);
    uint8_t low  = readRegister(REG_LO);

    return ((uint16_t)high << 5) | (low >> 3); // 13-Bit-Zahl zusammensetzen
}

uint8_t ADP5360::readRegister(uint8_t reg) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(reg);
    _wire->endTransmission(false); // Restart Condition (kein STOP)

    _wire->requestFrom(_i2cAddress, (uint8_t)1);
    return _wire->available() ? _wire->read() : 0; // Byte auslesen
}
