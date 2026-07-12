#ifndef POWERMANAGER_H
#define POWERMANAGER_H

//====================================================
// Relay Control
//====================================================

void setRelay(int pin, bool on);

void allRelaysOff();

//====================================================
// Power Source Selection
//====================================================

void selectSolarSource();

void selectBatterySource();

void updatePowerSource();

#endif