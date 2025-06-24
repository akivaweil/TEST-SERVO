#ifndef WEBHANLDER_H
#define WEBHANLDER_H

// Include necessary headers (add more as needed)
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h> // Include ArduinoJson

// --- Extern Global Objects ---
extern AsyncWebServer server;       // Declare server object as extern
extern WebSocketsServer webSocket; // Declare webSocket object as extern

// --- Function Declarations ---
void setupWebServerAndWebSocket();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void handleRoot(AsyncWebServerRequest *request); // Declare handleRoot with correct signature
void sendCurrentPositionUpdate(); // Declare other functions defined in WebHandler.cpp if needed globally
void sendAllSettingsUpdate(uint8_t specificClientNum, String message);

// --- Include other necessary extern declarations if WebHandler needs them ---
// Example: extern volatile bool allHomed;
// Example: extern float pnpOffsetX_inch, pnpOffsetY_inch;
// (Consider a dedicated SharedGlobals.h for these)

#endif // WEBHANLDER_H 