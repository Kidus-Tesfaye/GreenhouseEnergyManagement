#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//====================================================
// Display Object
//====================================================

extern Adafruit_SSD1306 display;

//====================================================
// Display Functions
//====================================================

String onOffText(bool value);

String sourceText();

void updateDisplay();

#endif