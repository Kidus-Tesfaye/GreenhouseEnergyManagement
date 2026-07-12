#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_INA219.h>
#include <DHT.h>

//====================================================
// Sensor Objects
//====================================================

extern DHT dht;
extern Adafruit_INA219 ina219;

//====================================================
// Sensor Functions
//====================================================

int readSoilMoisturePercent();

void readSensors();

void updateLoads();

#endif