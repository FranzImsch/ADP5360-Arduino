#ifndef ADP5360_H
#define ADP5360_H

#include <Arduino.h>
#include <Wire.h>

#define FAULT_REG 0x2E
#define PGOOD_STATUS_REG 0x2F

class ADP5360
{
public:
    ADP5360(TwoWire &wire, uint8_t i2cAddress);

    void begin();

    uint8_t readStatus1();          // CHARGER_STATUS1 (Address 0x08)
    uint8_t readStatus2();          // CHARGER_STATUS2 (Address 0x09)
    uint8_t readBatSoc();           // BAT_SOC (Address 0x21)
    uint8_t readFaultRegister();    // FAULT (Address 0x2E)
    uint8_t readPgoodStatus();      // PGOOD_STATUS (Address 0x2F)
    uint8_t readInterruptEnable1(); // INTERRUPT_ENABLE1 (Address 0x32)
    uint8_t readInterruptEnable2(); // INTERRUPT_ENABLE2 (Address 0x33)
    uint8_t readInterruptFlag1();   // INTERRUPT_FLAG1 (Address 0x34)
    uint8_t readInterruptFlag2();   // INTERRUPT_FLAG2 (Address 0x35)
    uint16_t readBatteryVoltage();  // VBAT_READ (Address 0x25 and 0x26)

    // Functions for CHARGER_STATUS1
    bool isVbusOverVoltage();
    bool isAdaptiveChargingActive();
    bool isVbusCurrentLimited();
    uint8_t getChargerStatus();

    // Functions for CHARGER_STATUS2
    uint8_t getThrStatus();
    bool isBatteryOverVoltage();
    bool isBatteryUnderVoltage();
    uint8_t getBatteryChargeStatus();

    // Function for BAT_SOC
    uint8_t getBatterySoc();

    // Functions for PGOOD_STATUS
    bool isMRPressed();
    bool isChargeComplete();
    bool isVBUSOK();
    bool isBatteryOK();
    bool isVOUT2OK();
    bool isVOUT1OK();

    // Functions for FAULT
    bool hasUndervoltageFault();
    bool hasOvercurrentFault();
    bool hasOverchargeCurrentFault();
    bool hasOverchargeVoltageFault();
    bool hasWatchdogTimeoutFault();
    bool hasTemperatureShutdownFault();

    void clearUndervoltageFault();
    void clearOvercurrentFault();
    void clearOverchargeCurrentFault();
    void clearOverchargeVoltageFault();
    void clearWatchdogTimeoutFault();
    void clearTemperatureShutdownFault();

    // Interrupt Functions
    void enableSocLowInterrupt(bool enable);
    void enableSocAccumulationInterrupt(bool enable);
    void enableAdaptiveChargeInterrupt(bool enable);
    void enableBatteryProtectionInterrupt(bool enable);
    void enableThrInterrupt(bool enable);
    void enableBatteryVoltageInterrupt(bool enable);
    void enableChargerModeInterrupt(bool enable);
    void enableVbusInterrupt(bool enable);
    void enableMrInterrupt(bool enable);
    void enableWatchdogInterrupt(bool enable);
    void enableBuckPowerGoodInterrupt(bool enable);
    void enableBuckBoostPowerGoodInterrupt(bool enable);

    bool isSocLowInterruptSet();
    bool isSocAccumulationInterruptSet();
    bool isAdaptiveChargeInterruptSet();
    bool isBatteryProtectionInterruptSet();
    bool isThrInterruptSet();
    bool isBatteryVoltageInterruptSet();
    bool isChargerModeInterruptSet();
    bool isVbusInterruptSet();
    bool isMrInterruptSet();
    bool isWatchdogInterruptSet();
    bool isBuckPowerGoodInterruptSet();
    bool isBuckBoostPowerGoodInterruptSet();

private:
    TwoWire *_wire;
    uint8_t _i2cAddress;

    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t value);
    bool checkFaultBit(uint8_t bit);
    void clearFaultBit(uint8_t bit);
    bool checkStatusBit(uint8_t reg, uint8_t bit);
};

#endif
