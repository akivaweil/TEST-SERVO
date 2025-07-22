#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "ServoControl.h"
#include "OTA/OTA_Upload.h"
#include "Config.h"

//* ************************************************************************
//* ************************ SERVO RANDOM MOVEMENT TEST *******************
//* ************************************************************************
// Simple test program that moves a servo to random angles with OTA capability.

// Global variables
ServoControl servo;
unsigned long lastMoveTime = 0;
bool isInitialized = false;

//* ************************************************************************
//* ************************ SETUP FUNCTION *******************************
//* ************************************************************************
void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    
    // Initialize status LED
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);  // Turn on LED to indicate startup
    
    // Setup OTA functionality
    setupOTA();
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    
    // Initialize servo
    servo.init(SERVO_PIN, SERVO_CHANNEL, SERVO_FREQUENCY, SERVO_RESOLUTION);
    servo.setAngleRange(SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    
    // Move servo to center position initially
    servo.write(90.0);
    
    // Wait before starting random movements
    delay(RANDOM_SEED_DELAY);
    
    // Initialize random seed
    randomSeed(micros());
    
    isInitialized = true;
    digitalWrite(STATUS_LED_PIN, LOW);  // Turn off LED to indicate ready
}

//* ************************************************************************
//* ************************ LOOP FUNCTION ********************************
//* ************************************************************************
void loop() {
    // Handle OTA updates
    handleOTA();
    
    // Check if it's time to move the servo
    if (isInitialized && (millis() - lastMoveTime >= SERVO_MOVE_INTERVAL)) {
        // Generate random angle between min and max
        float randomAngle = random(SERVO_MIN_ANGLE * 100, SERVO_MAX_ANGLE * 100) / 100.0;
        
        // Move servo to random angle
        servo.write(randomAngle);
        
        // Update last move time
        lastMoveTime = millis();
        
        // Brief LED flash to indicate movement
        digitalWrite(STATUS_LED_PIN, HIGH);
        delay(50);
        digitalWrite(STATUS_LED_PIN, LOW);
    }
}
