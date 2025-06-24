#include <Arduino.h>
#include <ESP32Servo.h>

// According to your rules, OTA_Manager.cpp should not have a header file.
// To make its functions available to main.cpp, we declare them here.
// These functions are defined in your OTA_Manager.cpp file.
void initOTA();
void handleOTA();

// Per your rules, we are using comments to explain the code.
// This section defines the pin for the servo motor.
// Using float for a pin number as per your general rule, it will be converted to int by the attach function.
const float servoPin = 9.0;

// This creates a servo object.
Servo myServo;

//* ************************************************************************
//* ************************ SETUP ******************************************
//* ************************************************************************
// This function runs once when the ESP32 starts up.
void setup() {
  // According to your rules, we initialize OTA on startup.
  initOTA();

  // This attaches the servo object to the specified pin.
  // The float value of servoPin will be truncated to an integer.
  myServo.attach(servoPin);
}

//* ************************************************************************
//* ************************ MAIN LOOP ************************************
//* ************************************************************************
// This function runs repeatedly after setup() is complete.
void loop() {
  // According to your rules, we handle OTA updates in the main loop.
  handleOTA();

  //! ************************************************************************
  //! STEP 1: MOVE SERVO TO 24 DEGREES
  //! ************************************************************************
  myServo.write(24);
  delay(1000); // wait 1 second

  //! ************************************************************************
  //! STEP 2: MOVE SERVO TO 90 DEGREES
  //! ************************************************************************
  myServo.write(90);
  delay(1000); // wait 1 second
} 