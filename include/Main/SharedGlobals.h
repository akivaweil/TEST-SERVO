#ifndef SHAREDGLOBALS_H
#define SHAREDGLOBALS_H

#include <Arduino.h>
#include <FastAccelStepper.h> // Include if types like FastAccelStepper are needed
#include <Bounce2.h>          // Include if types like Bounce are needed
#include <ESP32Servo.h>       // Include if Servo is needed

// ======================
// Shared Global Variables
// ======================

// --- State Flags ---
extern volatile bool stopRequested;      // Flag to signal stop request
extern volatile bool isMoving;           // Flag indicating motor movement
extern volatile bool isHoming;           // Flag indicating homing sequence active
extern volatile bool allHomed;           // Flag indicating if all axes are homed
extern volatile bool inPickPlaceMode;    // Flag indicating PnP mode active
extern volatile bool inCalibrationMode;  // Flag indicating Calibration mode active
extern volatile bool pendingHomingAfterPnP; // Flag to home after exiting PnP

// --- Steppers ---
// Declare stepper pointers if they need to be accessed globally (e.g., in WebHandler for STOP command)
extern FastAccelStepper *stepper_x;
extern FastAccelStepper *stepper_y_left;
extern FastAccelStepper *stepper_y_right;
extern FastAccelStepper *stepper_z;
extern FastAccelStepper *stepper_rot;

// --- Servos ---
extern Servo servo_pitch; // Declare servo if needed globally

// --- Painting Settings ---
// Declare paint settings arrays if accessed/modified from multiple files
extern float paintZHeight[4];
extern int paintPitchAngle[4];
extern int paintPatternType[4];
extern float paintSpeed[4];
extern float paintPatternOffsetX_inch;
extern float paintPatternOffsetY_inch;
extern float paintGunOffsetX;
extern float paintGunOffsetY;

// --- PnP Settings ---
extern float pnpOffsetX_inch;
extern float pnpOffsetY_inch;
extern float placeFirstXAbsolute;
extern float placeFirstYAbsolute;
extern int gridCols;
extern int gridRows;
extern float gapX;
extern float gapY;

// --- General Speed/Accel ---
extern float patternXSpeed;
extern float patternXAccel;
extern float patternYSpeed;
extern float patternYAccel;
extern float patternZSpeed;
extern float patternZAccel;
extern float patternRotSpeed;
extern float patternRotAccel;

// --- Tray Dimensions ---
extern float trayWidth;
extern float trayHeight;

// ======================
// Shared Global Functions
// ======================

// --- Motion Control ---
extern void homeAllAxes();
extern void moveToPositionInches(float targetX_inch, float targetY_inch, float targetZ_inch);
extern void moveToXYPositionInches(float targetX_inch, float targetY_inch); // If needed
extern void rotateToAbsoluteDegree(int targetDegree); // If needed
extern void stopAllMovement(); // Declare the stop function

// --- Settings ---
extern void saveSettings();
extern void loadSettings();

// --- Pick & Place Control ---
extern void enterPickPlaceMode();
extern void exitPickPlaceMode(bool requestHoming = false);
extern void executeNextPickPlaceStep();
extern void skipPickPlaceLocation();
extern void goBackPickPlaceLocation();

// --- Painting Control ---
extern void paintSide(int sideIndex);
extern void activatePaintGun();
extern void deactivatePaintGun(bool forceOff = false);

// Note: Functions declared in GeneralSettings_PinDef.h (like calculateAndSetGridSpacing)
// don't need to be redeclared here if that header is also included.

#endif // SHAREDGLOBALS_H 