#include "ADP5360.h"

ADP5360::ADP5360(TwoWire &wire, uint8_t i2cAddress)
    : _wire(&wire), _i2cAddress(i2cAddress) {}

void ADP5360::begin()
{
    _wire->begin();
}

uint8_t ADP5360::readRegister(uint8_t reg)
{
    _wire->beginTransmission(_i2cAddress);
    _wire->write(reg);
    _wire->endTransmission(false); // Restart Condition

    _wire->requestFrom(_i2cAddress, (uint8_t)1);
    return _wire->available() ? _wire->read() : 0;
}

void ADP5360::writeRegister(uint8_t reg, uint8_t value)
{
    _wire->beginTransmission(_i2cAddress);
    _wire->write(reg);
    _wire->write(value);
    _wire->endTransmission();
}

uint8_t ADP5360::readStatus1()
{
    return readRegister(0x08);
}

uint8_t ADP5360::readStatus2()
{
    return readRegister(0x09);
}

uint8_t ADP5360::readBatSoc()
{
    return readRegister(0x21);
}

uint8_t ADP5360::readPgoodStatus()
{
    return readRegister(0x2F);
}

uint8_t ADP5360::readInterruptEnable1()
{
    return readRegister(0x32);
}

uint8_t ADP5360::readInterruptEnable2()
{
    return readRegister(0x33);
}

uint8_t ADP5360::readInterruptFlag1()
{
    return readRegister(0x34);
}

uint8_t ADP5360::readInterruptFlag2()
{
    return readRegister(0x35);
}

uint16_t ADP5360::readBatteryVoltage()
{
    uint8_t high = readRegister(0x25);
    uint8_t low = readRegister(0x26);

    return ((uint16_t)high << 5) | (low >> 3);
}

uint8_t ADP5360::readFaultRegister()
{
    return readRegister(FAULT_REG);
}

bool ADP5360::checkFaultBit(uint8_t bit)
{
    uint8_t faultStatus = readFaultRegister();
    return (faultStatus & (1 << bit)) != 0;
}

void ADP5360::clearFaultBit(uint8_t bit)
{
    uint8_t faultStatus = readFaultRegister();
    faultStatus |= (1 << bit);
    writeRegister(FAULT_REG, faultStatus);
}

bool ADP5360::checkStatusBit(uint8_t reg, uint8_t bit)
{
    uint8_t status = readRegister(reg);
    return (status & (1 << bit)) != 0;
}

// Functions for CHARGER_STATUS1
bool ADP5360::isVbusOverVoltage()
{
    return (readStatus1() & (1 << 7));
}

bool ADP5360::isAdaptiveChargingActive()
{
    return (readStatus1() & (1 << 6));
}

bool ADP5360::isVbusCurrentLimited()
{
    return (readStatus1() & (1 << 5));
}

uint8_t ADP5360::getChargerStatus()
{
    return (readStatus1() & 0x07); // Extract bits 2:0
}

// Functions for CHARGER_STATUS2
uint8_t ADP5360::getThrStatus()
{
    return ((readStatus2() >> 5) & 0x07); // Extract bits 7:5
}

bool ADP5360::isBatteryOverVoltage()
{
    return (readStatus2() & (1 << 4));
}

bool ADP5360::isBatteryUnderVoltage()
{
    return (readStatus2() & (1 << 3));
}

uint8_t ADP5360::getBatteryChargeStatus()
{
    return (readStatus2() & 0x07); // Extract bits 2:0
}

// Function for BAT_SOC
uint8_t ADP5360::getBatterySoc()
{
    return (readBatSoc() & 0x7F); // Extract bits 6:0
}

bool ADP5360::isMRPressed() { return checkStatusBit(PGOOD_STATUS_REG, 5); }
bool ADP5360::isChargeComplete() { return checkStatusBit(PGOOD_STATUS_REG, 4); }
bool ADP5360::isVBUSOK() { return checkStatusBit(PGOOD_STATUS_REG, 3); }
bool ADP5360::isBatteryOK() { return checkStatusBit(PGOOD_STATUS_REG, 2); }
bool ADP5360::isVOUT2OK() { return checkStatusBit(PGOOD_STATUS_REG, 1); }
bool ADP5360::isVOUT1OK() { return checkStatusBit(PGOOD_STATUS_REG, 0); }

bool ADP5360::hasUndervoltageFault() { return checkFaultBit(7); }
bool ADP5360::hasOvercurrentFault() { return checkFaultBit(6); }
bool ADP5360::hasOverchargeCurrentFault() { return checkFaultBit(5); }
bool ADP5360::hasOverchargeVoltageFault() { return checkFaultBit(4); }
bool ADP5360::hasWatchdogTimeoutFault() { return checkFaultBit(2); }
bool ADP5360::hasTemperatureShutdownFault() { return checkFaultBit(0); }

void ADP5360::clearUndervoltageFault() { clearFaultBit(7); }
void ADP5360::clearOvercurrentFault() { clearFaultBit(6); }
void ADP5360::clearOverchargeCurrentFault() { clearFaultBit(5); }
void ADP5360::clearOverchargeVoltageFault() { clearFaultBit(4); }
void ADP5360::clearWatchdogTimeoutFault() { clearFaultBit(2); }
void ADP5360::clearTemperatureShutdownFault() { clearFaultBit(0); }

// Interrupt Functions
void ADP5360::enableSocLowInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 7);
    }
    else
    {
        regValue &= ~(1 << 7);
    }
    writeRegister(0x32, regValue);
}

void ADP5360::enableSocAccumulationInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 6);
    }
    else
    {
        regValue &= ~(1 << 6);
    }
    writeRegister(0x32, regValue);
}

void ADP5360::enableAdaptiveChargeInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 5);
    }
    else
    {
        regValue &= ~(1 << 5);
    }
    writeRegister(0x32, regValue);
}

void ADP5360::enableBatteryProtectionInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 4);
    }
    else
    {
        regValue &= ~(1 << 4);
    }
    writeRegister(0x32, regValue);
}

void ADP5360::enableThrInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 3);
    }
    else
    {
        regValue &= ~(1 << 3);
    }
    writeRegister(0x32, regValue);
}

void ADP5360::enableBatteryVoltageInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 2);
    }
    else
    {
        regValue &= ~(1 << 2);
    }
    writeRegister(0x32, regValue);
}
void ADP5360::enableChargerModeInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 1);
    }
    else
    {
        regValue &= ~(1 << 1);
    }
    writeRegister(0x32, regValue);
}

void ADP5360::enableVbusInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable1();
    if (enable)
    {
        regValue |= (1 << 0);
    }
    else
    {
        regValue &= ~(1 << 0);
    }
    writeRegister(0x32, regValue);
}

void ADP5360::enableMrInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable2();
    if (enable)
    {
        regValue |= (1 << 7);
    }
    else
    {
        regValue &= ~(1 << 7);
    }
    writeRegister(0x33, regValue);
}

void ADP5360::enableWatchdogInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable2();
    if (enable)
    {
        regValue |= (1 << 6);
    }
    else
    {
        regValue &= ~(1 << 6);
    }
    writeRegister(0x33, regValue);
}

void ADP5360::enableBuckPowerGoodInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable2();
    if (enable)
    {
        regValue |= (1 << 4);
    }
    else
    {
        regValue &= ~(1 << 4);
    }
    writeRegister(0x33, regValue);
}

void ADP5360::enableBuckBoostPowerGoodInterrupt(bool enable)
{
    uint8_t regValue = readInterruptEnable2();
    if (enable)
    {
        regValue |= (1 << 5);
    }
    else
    {
        regValue &= ~(1 << 5);
    }
    writeRegister(0x33, regValue);
}

bool ADP5360::isSocLowInterruptSet()
{
    return (readInterruptFlag1() & (1 << 7));
}

bool ADP5360::isSocAccumulationInterruptSet()
{
    return (readInterruptFlag1() & (1 << 6));
}

bool ADP5360::isAdaptiveChargeInterruptSet()
{
    return (readInterruptFlag1() & (1 << 5));
}

bool ADP5360::isBatteryProtectionInterruptSet()
{
    return (readInterruptFlag1() & (1 << 4));
}

bool ADP5360::isThrInterruptSet()
{
    return (readInterruptFlag1() & (1 << 3));
}

bool ADP5360::isBatteryVoltageInterruptSet()
{
    return (readInterruptFlag1() & (1 << 2));
}

bool ADP5360::isChargerModeInterruptSet()
{
    return (readInterruptFlag1() & (1 << 1));
}

bool ADP5360::isVbusInterruptSet()
{
    return (readInterruptFlag1() & (1 << 0));
}

bool ADP5360::isMrInterruptSet()
{
    return (readInterruptFlag2() & (1 << 7));
}

bool ADP5360::isWatchdogInterruptSet()
{
    return (readInterruptFlag2() & (1 << 6));
}

bool ADP5360::isBuckPowerGoodInterruptSet()
{
    return (readInterruptFlag2() & (1 << 4));
}

bool ADP5360::isBuckBoostPowerGoodInterruptSet()
{
    return (readInterruptFlag2() & (1 << 5));
}