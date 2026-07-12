#ifndef CONFIG_H
#define CONFIG_H

//====================================================
// Wi-Fi Configuration
//====================================================

// Rename this file to Config.h
// and replace the values below with your own network.

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

//====================================================
// GPIO Pin Assignments
//====================================================

constexpr int DHT_PIN = 23;
constexpr int SOIL_PIN = 34;

// Relay Outputs
constexpr int SOLAR_RELAY_PIN = 4;
constexpr int BATTERY_RELAY_PIN = 18;
constexpr int FAN_RELAY_PIN = 5;
constexpr int PUMP_RELAY_PIN = 26;
constexpr int LED_RELAY_PIN = 13;

// Most relay boards are active LOW.
constexpr bool RELAY_ACTIVE_LOW = true;

//====================================================
// OLED Configuration
//====================================================

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;

//====================================================
// Sensor Configuration
//====================================================

#define DHT_TYPE DHT11

//====================================================
// Environmental Thresholds
//====================================================

constexpr float FAN_TEMP_ON_C = 20.0f;
constexpr float FAN_HUMIDITY_ON_PERCENT = 75.0f;
constexpr int SOIL_MOISTURE_PUMP_ON_PERCENT = 40;

//====================================================
// Solar Source Selection
//====================================================

constexpr float MIN_SOLAR_POWER_WATTS = 0.4f;
constexpr float MIN_SOLAR_POWER_WATTS_SHUNT = 0.05f;
constexpr float SOLAR_HYSTERESIS_WATTS = 0.0000001f;
constexpr float MIN_SOLAR_VOLTAGE = 5.0f;

//====================================================
// Soil Sensor Calibration
//====================================================

constexpr int SOIL_ADC_DRY = 3000;
constexpr int SOIL_ADC_WET = 1200;

//====================================================
// Timing
//====================================================

constexpr unsigned long SENSOR_INTERVAL_MS = 2000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 1000;
constexpr unsigned long LED_TOGGLE_INTERVAL_MS = 30000;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 10000;
constexpr unsigned long SOURCE_SWITCH_DEADTIME_MS = 250;

#endif