#include <Arduino.h>
#include <ESP32Servo.h>

// According to your rules, OTA_Manager.cpp should not have a header file.
// To make its functions available to main.cpp, we declare them here.
// These functions are defined in your OTA_Manager.cpp file.
void initOTA();
void handleOTA();

// Per your rules, we are using comments to explain the code.
// This section defines the pin for the servo motor.
// Using int for the pin number to ensure proper servo functionality.
const int servoPin = 14;

// This creates a servo object.
Servo myServo;

//* ************************************************************************
//* ************************ SETUP ******************************************
//* ************************************************************************
// This function runs once when the ESP32 starts up.
void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  Serial.println("Starting servo tester...");
  
  // Try to attach the servo - continue even if it fails
  if (myServo.attach(servoPin)) {
    Serial.println("Servo successfully attached to pin " + String(servoPin));
  } else {
    Serial.println("Servo failed to attach to pin " + String(servoPin) + " - continuing anyway");
  }
  
  // Test initial position
  myServo.write(90);
  Serial.println("Servo moved to 90 degrees (center position)");
  delay(2000); // Give time to see initial movement
}

//* ************************************************************************
//* ************************ MAIN LOOP ************************************
//* ************************************************************************
// This function runs repeatedly after setup() is complete.
void loop() {
  //! ************************************************************************
  //! STEP 1: MOVE SERVO TO 24 DEGREES
  //! ************************************************************************
  Serial.println("Moving servo to 24 degrees");
  myServo.write(24);
  delay(1000); // wait 1 second

  //! ************************************************************************
  //! STEP 2: MOVE SERVO TO 90 DEGREES
  //! ************************************************************************
  Serial.println("Moving servo to 90 degrees");
  myServo.write(90);
  delay(1000); // wait 1 second
} 