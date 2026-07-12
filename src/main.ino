#include <Wire.h>
#include <WiFi.h>

#include "Config.h"
#include "SystemState.h"

#include "PowerManager.h"
#include "Sensors.h"
#include "Display.h"
#include "WebInterface.h"

//====================================================
// Global System State
//====================================================

SensorData sensors;
LoadState loads;
PowerState power;
TimingState timing;

//====================================================
// Wi-Fi
//====================================================

void connectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD);

    unsigned long startMs = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startMs < 8000)
    {
        delay(250);
    }
}

//====================================================
// Setup
//====================================================

void setup()
{
    Serial.begin(115200);

    delay(500);

    //------------------------------------------------
    // Relay Outputs
    //------------------------------------------------

    pinMode(SOLAR_RELAY_PIN, OUTPUT);
    pinMode(BATTERY_RELAY_PIN, OUTPUT);
    pinMode(FAN_RELAY_PIN, OUTPUT);
    pinMode(PUMP_RELAY_PIN, OUTPUT);
    pinMode(LED_RELAY_PIN, OUTPUT);

    allRelaysOff();

    //------------------------------------------------
    // Safe Startup
    //------------------------------------------------

    // Start on battery until solar availability
    // has been confirmed.

    setRelay(BATTERY_RELAY_PIN, true);

    power.usingSolar = false;

    //------------------------------------------------
    // I2C
    //------------------------------------------------

    Wire.begin(21, 22);

    //------------------------------------------------
    // Sensors
    //------------------------------------------------

    dht.begin();

    if (!ina219.begin())
    {
        Serial.println(
            "INA219 not found. Check wiring.");
    }

    //------------------------------------------------
    // OLED
    //------------------------------------------------

    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            0x3C))
    {
        Serial.println(
            "OLED not found. Check wiring.");
    }
    else
    {
        display.clearDisplay();

        display.setTextSize(1);

        display.setTextColor(SSD1306_WHITE);

        display.setCursor(0, 0);

        display.println("Starting...");

        display.display();
    }

    //------------------------------------------------
    // Wi-Fi
    //------------------------------------------------

    connectWiFi();

    //------------------------------------------------
    // Web Server
    //------------------------------------------------

    server.on("/", handleRoot);

    server.on("/json", handleJson);

    server.begin();

    //------------------------------------------------
    // Timers
    //------------------------------------------------

    timing.lastLedToggleMs = millis();
}

//====================================================
// Main Loop
//====================================================

void loop()
{
    unsigned long now = millis();

    //------------------------------------------------
    // Sensor Update
    //------------------------------------------------

    if (now - timing.lastSensorReadMs >=
        SENSOR_INTERVAL_MS)
    {
        readSensors();

        updatePowerSource();

        updateLoads();

        timing.lastSensorReadMs = now;

        Serial.print("Source=");
        Serial.print(sourceText());

        Serial.print(" Solar=");
        Serial.print(sensors.solarPowerW, 3);

        Serial.print("W Temp=");
        Serial.print(sensors.temperatureC);

        Serial.print("C Hum=");
        Serial.print(sensors.humidityPercent);

        Serial.print("% Soil=");
        Serial.print(sensors.soilMoisturePercent);

        Serial.println("%");
    }

    //------------------------------------------------
    // OLED Update
    //------------------------------------------------

    if (now - timing.lastDisplayUpdateMs >=
        DISPLAY_INTERVAL_MS)
    {
        updateDisplay();

        timing.lastDisplayUpdateMs = now;
    }

    //------------------------------------------------
    // Wi-Fi Reconnect
    //------------------------------------------------

    if (WiFi.status() != WL_CONNECTED &&
        now - timing.lastWifiRetryMs >=
            WIFI_RETRY_INTERVAL_MS)
    {
        connectWiFi();

        timing.lastWifiRetryMs = now;
    }

    //------------------------------------------------
    // HTTP Server
    //------------------------------------------------

    server.handleClient();
}