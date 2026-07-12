#include "Display.h"

#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "SystemState.h"

//====================================================
// Display Object
//====================================================

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET);

//====================================================
// Helper Functions
//====================================================

String onOffText(bool value)
{
    return value ? "ON" : "OFF";
}

String sourceText()
{
    return power.usingSolar ? "Solar" : "Battery";
}

//====================================================
// OLED Update
//====================================================

void updateDisplay()
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    //------------------------------------------------
    // Power Source
    //------------------------------------------------

    display.print("Source: ");
    display.println(sourceText());

    //------------------------------------------------
    // Solar Measurements
    //------------------------------------------------

    display.print("Solar: ");
    display.print(sensors.solarBusVoltageV, 1);
    display.print("V ");
    display.print(sensors.solarPowerW, 2);
    display.println("W");

    //------------------------------------------------
    // Temperature
    //------------------------------------------------

    display.print("Temp: ");

    if (isnan(sensors.temperatureC))
    {
        display.print("--");
    }
    else
    {
        display.print(sensors.temperatureC, 1);
    }

    display.println(" C");

    //------------------------------------------------
    // Humidity
    //------------------------------------------------

    display.print("Hum: ");

    if (isnan(sensors.humidityPercent))
    {
        display.print("--");
    }
    else
    {
        display.print(sensors.humidityPercent, 0);
    }

    display.println(" %");

    //------------------------------------------------
    // Soil Moisture
    //------------------------------------------------

    display.print("Soil: ");
    display.print(sensors.soilMoisturePercent);
    display.println(" %");

    //------------------------------------------------
    // Load Status
    //------------------------------------------------

    display.print("F:");
    display.print(onOffText(loads.fanOn));

    display.print(" P:");
    display.print(onOffText(loads.pumpOn));

    display.print(" L:");
    display.println(onOffText(loads.ledOn));

    //------------------------------------------------
    // Wi-Fi Status
    //------------------------------------------------

    display.print("IP: ");

    if (WiFi.status() == WL_CONNECTED)
    {
        display.println(WiFi.localIP());
    }
    else
    {
        display.println("offline");
    }

    display.display();
}