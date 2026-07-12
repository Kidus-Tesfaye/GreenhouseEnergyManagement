#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

#include <Arduino.h>

//====================================================
// Sensor Readings
//====================================================

struct SensorData
{
    float temperatureC = NAN;
    float humidityPercent = NAN;

    float solarBusVoltageV = 0.0f;
    float solarCurrentmA = 0.0f;
    float solarPowerW = 0.0f;

    int soilMoisturePercent = 0;
    int soilRaw = 0;
};

//====================================================
// Relay / Load States
//====================================================

struct LoadState
{
    bool fanOn = false;
    bool pumpOn = false;
    bool ledOn = false;
};

//====================================================
// Power Source State
//====================================================

struct PowerState
{
    bool usingSolar = false;
};

//====================================================
// Timing Variables
//====================================================

struct TimingState
{
    unsigned long lastSensorReadMs = 0;
    unsigned long lastDisplayUpdateMs = 0;
    unsigned long lastLedToggleMs = 0;
    unsigned long lastWifiRetryMs = 0;
};

//====================================================
// Global State Objects
//====================================================

extern SensorData sensors;
extern LoadState loads;
extern PowerState power;
extern TimingState timing;

#endif