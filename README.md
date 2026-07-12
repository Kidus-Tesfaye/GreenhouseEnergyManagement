# Greenhouse Energy Management

An ESP32-based greenhouse energy management system for solar-powered automation, monitoring, and local web reporting.

## Overview

This project runs on an ESP32 and manages a greenhouse using:
- solar power monitoring with an INA219 sensor
- environmental sensing via a DHT11 temperature/humidity sensor
- soil moisture sensing via a resistive soil sensor
- relay control for fan, water pump, status LED, and power source switching
- OLED status display
- local web dashboard and JSON endpoint

The controller automatically switches between solar and battery power based on solar availability, runs cooling and irrigation loads, and exposes system status through a web interface.

## Features

- Solar voltage, current, and power measurement
- Temperature and humidity monitoring
- Soil moisture measurement and irrigation control
- Automatic fan control based on temperature/humidity thresholds
- Automatic source selection between solar and battery power
- OLED display with live status
- Web dashboard at `/`
- JSON status endpoint at `/json`

## Hardware

Required components:

- ESP32 development board
- Adafruit INA219 current/voltage sensor
- DHT11 temperature and humidity sensor
- Resistive soil moisture sensor (LM393-style)
- 2-channel relay module
- 0.96" OLED display (SSD1306)
- Solar panel and 12V battery
- Fan, water pump, LED load

The `hardware/BOM.csv` file contains the components used for the prototype and approximate cost.

## Wiring

Suggested GPIO assignments are configured in `src/Config.h` / `src/Config.example.h`:

- `DHT_PIN` = `23`
- `SOIL_PIN` = `34`
- `SOLAR_RELAY_PIN` = `4`
- `BATTERY_RELAY_PIN` = `18`
- `FAN_RELAY_PIN` = `5`
- `PUMP_RELAY_PIN` = `26`
- `LED_RELAY_PIN` = `13`

Use I2C pins `21` (SDA) and `22` (SCL) for the OLED and INA219.

Relay outputs are assumed to be active LOW; adjust `RELAY_ACTIVE_LOW` in `Config.h` if your relay board is active HIGH.

## Configuration

1. Copy `src/Config.example.h` to `src/Config.h`
2. Update the Wi-Fi credentials:
   ```cpp
   const char* WIFI_SSID = "YOUR_WIFI_NAME";
   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
   ```
3. Adjust thresholds and calibration values if needed:
   - `FAN_TEMP_ON_C`
   - `FAN_HUMIDITY_ON_PERCENT`
   - `SOIL_MOISTURE_PUMP_ON_PERCENT`
   - `SOIL_ADC_DRY`
   - `SOIL_ADC_WET`
   - `MIN_SOLAR_POWER_WATTS`
   - `MIN_SOLAR_VOLTAGE`

## Build and Upload

This project uses the Arduino-style sketch `src/main.ino`.

### Using Arduino IDE

1. Open `src/main.ino` in the Arduino IDE.
2. Install the required libraries:
   - `Adafruit SSD1306`
   - `Adafruit GFX`
   - `Adafruit INA219`
   - `DHT sensor library`
   - `ESP32` board support
3. Select the correct ESP32 board and upload port.
4. Compile and upload.

### Using PlatformIO

This repository includes a `platformio.ini` for the `esp32dev` board. From the repository root:

```bash
platformio run
platformio run --target upload
```

If you need a specific upload port, add `--upload-port <PORT>` to the upload command.

## Usage

After booting, the ESP32 will:

- initialize sensors and OLED
- connect to Wi-Fi
- start a local web server on port `80`
- read sensors every 2 seconds
- update the OLED every 1 second
- toggle the status LED every 30 seconds

Visit the ESP32 IP address in a browser to see the web dashboard.

The JSON endpoint is available at:

```
http://<device-ip>/json
```

## Code Structure

- `src/main.ino` — main setup and loop logic
- `src/Config.example.h` — configuration template for Wi-Fi, pins, thresholds
- `src/SystemState.h` — shared state structures
- `src/Sensors.*` — sensor reading and load control logic
- `src/PowerManager.*` — solar/battery source switching and relay control
- `src/Display.*` — OLED display rendering
- `src/WebInterface.*` — HTTP handlers and dashboard HTML

## Notes

- `Config.h` is intentionally not tracked; keep your Wi-Fi credentials private.
- The repository includes `docs/.gitkeep` to preserve the `docs` folder in Git.
- If your sensors return invalid readings, check wiring and power.

## License

This project is licensed under the MIT License. See `LICENSE` for details.
