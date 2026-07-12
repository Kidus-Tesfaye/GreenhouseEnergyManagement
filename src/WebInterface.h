#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <WebServer.h>

//====================================================
// Web Server Object
//====================================================

extern WebServer server;

//====================================================
// Web Interface
//====================================================

void handleRoot();

void handleJson();

String buildHtmlPage();

#endif