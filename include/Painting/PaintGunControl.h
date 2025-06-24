#ifndef PAINTGUNCONTROL_H
#define PAINTGUNCONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h> // Make sure this is included if Servo is used
#include "../Main/GeneralSettings_PinDef.h" // Revert to relative path

// Function Declarations
void setupPaintGunServo();
void initializePaintGunControl();
void activatePaintGun();
void deactivatePaintGun(bool forceOff = false);

#endif // PAINTGUNCONTROL_H