#include "WebInterface.h"

#include <Arduino.h>

#include "Config.h"
#include "Display.h"
#include "SystemState.h"

//====================================================
// Web Server
//====================================================

WebServer server(80);

//====================================================
// Private Helper Functions
//====================================================

static String cardHtml(const String &label,
                       const String &value,
                       const String &className)
{
    String html = "<section class='card'><div class='label'>";
    html += label;
    html += "</div><div class='value ";
    html += className;
    html += "'>";
    html += value;
    html += "</div></section>";

    return html;
}

static void appendPageHeader(String &html)
{
    html += "<!doctype html><html><head>";
    html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
    html += "<meta http-equiv='refresh' content='5'>";
    html += "<title>ESP32 Solar Controller</title>";

    html += "<style>";

    html += "body{font-family:Arial,sans-serif;margin:0;background:#f4f6f8;color:#17202a}";
    html += "main{max-width:720px;margin:0 auto;padding:20px}";
    html += "h1{font-size:24px;margin:0 0 16px}";
    html += ".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(160px,1fr));gap:12px}";
    html += ".card{background:white;border:1px solid #d9e1e8;border-radius:8px;padding:14px}";
    html += ".label{font-size:12px;color:#5d6d7e;text-transform:uppercase}";
    html += ".value{font-size:24px;font-weight:700;margin-top:6px}";
    html += ".ok{color:#117a65}";
    html += ".warn{color:#b03a2e}";

    html += "</style></head><body><main>";
    html += "<h1>ESP32 Solar Controller</h1>";
    html += "<div class='grid'>";
}

static void appendCards(String &html)
{
    html += cardHtml(
        "Power Source",
        sourceText(),
        power.usingSolar ? "ok" : "");

    html += cardHtml(
        "Solar Voltage",
        String(sensors.solarBusVoltageV, 2) + " V",
        "");

    html += cardHtml(
        "Solar Current",
        String(sensors.solarCurrentmA, 1) + " mA",
        "");

    html += cardHtml(
        "Solar Power",
        String(sensors.solarPowerW, 3) + " W",
        sensors.solarPowerW >= MIN_SOLAR_POWER_WATTS ? "ok" : "warn");

    if (isnan(sensors.temperatureC))
    {
        html += cardHtml("Temperature", "--", "warn");
    }
    else
    {
        html += cardHtml(
            "Temperature",
            String(sensors.temperatureC, 1) + " C",
            loads.fanOn ? "warn" : "");
    }

    if (isnan(sensors.humidityPercent))
    {
        html += cardHtml("Humidity", "--", "warn");
    }
    else
    {
        html += cardHtml(
            "Humidity",
            String(sensors.humidityPercent, 0) + " %",
            loads.fanOn ? "warn" : "");
    }

    html += cardHtml(
        "Soil Moisture",
        String(sensors.soilMoisturePercent) + " %",
        loads.pumpOn ? "warn" : "");

    html += cardHtml(
        "Fan",
        onOffText(loads.fanOn),
        loads.fanOn ? "ok" : "");

    html += cardHtml(
        "Pump",
        onOffText(loads.pumpOn),
        loads.pumpOn ? "ok" : "");

    html += cardHtml(
        "LED",
        onOffText(loads.ledOn),
        loads.ledOn ? "ok" : "");

    html += cardHtml(
        "Soil ADC",
        String(sensors.soilRaw),
        "");
}

//====================================================
// HTML Page
//====================================================

String buildHtmlPage()
{
    String html;

    html.reserve(3500);

    appendPageHeader(html);

    appendCards(html);

    html += "</div></main></body></html>";

    return html;
}

//====================================================
// HTTP Handlers
//====================================================

void handleRoot()
{
    server.send(
        200,
        "text/html",
        buildHtmlPage());
}

void handleJson()
{
    String json;

    json.reserve(800);

    json += "{";

    json += "\"source\":\"" + sourceText() + "\",";

    json += "\"solar_voltage_v\":";
    json += String(sensors.solarBusVoltageV, 3);
    json += ",";

    json += "\"solar_current_ma\":";
    json += String(sensors.solarCurrentmA, 3);
    json += ",";

    json += "\"solar_power_w\":";
    json += String(sensors.solarPowerW, 4);
    json += ",";

    json += "\"temperature_c\":";
    json += isnan(sensors.temperatureC)
                ? String("null")
                : String(sensors.temperatureC, 2);
    json += ",";

    json += "\"humidity_percent\":";
    json += isnan(sensors.humidityPercent)
                ? String("null")
                : String(sensors.humidityPercent, 2);
    json += ",";

    json += "\"soil_moisture_percent\":";
    json += String(sensors.soilMoisturePercent);
    json += ",";

    json += "\"soil_raw\":";
    json += String(sensors.soilRaw);
    json += ",";

    json += "\"fan_on\":";
    json += String(loads.fanOn ? "true" : "false");
    json += ",";

    json += "\"pump_on\":";
    json += String(loads.pumpOn ? "true" : "false");
    json += ",";

    json += "\"led_on\":";
    json += String(loads.ledOn ? "true" : "false");

    json += "}";

    server.send(
        200,
        "application/json",
        json);
}