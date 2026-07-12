#include "Sensors.h"

#include <Arduino.h>

#include "Config.h"
#include "SystemState.h"
#include "PowerManager.h"

//====================================================
// Sensor Objects
//====================================================

DHT dht(DHT_PIN, DHT_TYPE);

Adafruit_INA219 ina219;

//====================================================
// Soil Moisture
//====================================================

int readSoilMoisturePercent()
{
    sensors.soilRaw = analogRead(SOIL_PIN);

    int percent = map(
        sensors.soilRaw,
        SOIL_ADC_DRY,
        SOIL_ADC_WET,
        0,
        100);

    return constrain(percent, 0, 100);
}

//====================================================
// Read All Sensors
//====================================================

void readSensors()
{
    sensors.temperatureC = dht.readTemperature();

    sensors.humidityPercent = dht.readHumidity();

    sensors.soilMoisturePercent = readSoilMoisturePercent();

    sensors.solarBusVoltageV = ina219.getBusVoltage_V();

    sensors.solarCurrentmA = ina219.getCurrent_mA();

    sensors.solarPowerW = ina219.getPower_mW() / 1000.0;

    // Ignore tiny negative readings caused by sensor noise.

    if (sensors.solarCurrentmA < 0.0f)
    {
        sensors.solarCurrentmA = 0.0f;
    }

    if (sensors.solarPowerW < 0.0f)
    {
        sensors.solarPowerW = 0.0f;
    }
}

//====================================================
// Load Control
//====================================================

void updateLoads()
{
    bool validDht =
        !isnan(sensors.temperatureC) &&
        !isnan(sensors.humidityPercent);

    loads.fanOn =
        validDht &&
        (
            sensors.temperatureC >= FAN_TEMP_ON_C ||
            sensors.humidityPercent >= FAN_HUMIDITY_ON_PERCENT
        );

    loads.pumpOn =
        sensors.soilMoisturePercent <
        SOIL_MOISTURE_PUMP_ON_PERCENT;

    unsigned long now = millis();

    if (now - timing.lastLedToggleMs >= LED_TOGGLE_INTERVAL_MS)
    {
        loads.ledOn = !loads.ledOn;
        timing.lastLedToggleMs = now;
    }

    setRelay(FAN_RELAY_PIN, loads.fanOn);

    setRelay(PUMP_RELAY_PIN, loads.pumpOn);

    setRelay(LED_RELAY_PIN, loads.ledOn);
}