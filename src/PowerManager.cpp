#include "PowerManager.h"

#include <Arduino.h>

#include "Config.h"
#include "SystemState.h"

//====================================================
// Relay Control
//====================================================

void setRelay(int pin, bool on)
{
    if (RELAY_ACTIVE_LOW)
    {
        digitalWrite(pin, on ? LOW : HIGH);
    }
    else
    {
        digitalWrite(pin, on ? HIGH : LOW);
    }
}

void allRelaysOff()
{
    setRelay(SOLAR_RELAY_PIN, false);
    setRelay(BATTERY_RELAY_PIN, false);
    setRelay(FAN_RELAY_PIN, false);
    setRelay(PUMP_RELAY_PIN, false);
    setRelay(LED_RELAY_PIN, false);
}

//====================================================
// Power Source Selection
//====================================================

void selectSolarSource()
{
    if (power.usingSolar)
    {
        return;
    }

    // Break-before-make:
    // Never allow the battery and solar sources to be connected
    // simultaneously.

    setRelay(BATTERY_RELAY_PIN, false);

    delay(SOURCE_SWITCH_DEADTIME_MS);

    setRelay(SOLAR_RELAY_PIN, true);

    power.usingSolar = true;
}

void selectBatterySource()
{
    if (!power.usingSolar)
    {
        return;
    }

    // Break-before-make:
    // Never allow the battery and solar sources to be connected
    // simultaneously.

    setRelay(SOLAR_RELAY_PIN, false);

    delay(SOURCE_SWITCH_DEADTIME_MS);

    setRelay(BATTERY_RELAY_PIN, true);

    power.usingSolar = false;
}

void updatePowerSource()
{
    if (power.usingSolar)
    {
        if (sensors.solarPowerW <
                (MIN_SOLAR_POWER_WATTS - SOLAR_HYSTERESIS_WATTS) ||
            sensors.solarBusVoltageV < MIN_SOLAR_VOLTAGE)
        {
            selectBatterySource();
        }
    }
    else
    {
        if (sensors.solarPowerW >
                (MIN_SOLAR_POWER_WATTS_SHUNT + SOLAR_HYSTERESIS_WATTS) &&
            sensors.solarBusVoltageV > MIN_SOLAR_VOLTAGE)
        {
            selectSolarSource();
        }
    }
}