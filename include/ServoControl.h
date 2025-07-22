#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>

//* ************************************************************************
//* ************************ SERVO CONTROL LIBRARY ************************
//* ************************************************************************
// Custom servo control library for ESP32 using PWM channels.

class ServoControl {
private:
    int pin;
    int channel;
    int frequency;
    int resolution;
    int minPulseWidth;
    int maxPulseWidth;
    int minAngle;
    int maxAngle;
    
    int angleToDuty(float angle);

public:
    ServoControl();
    
    // Initialize servo with pin, channel, frequency, and resolution
    void init(int servoPin, int pwmChannel, int freq = 50, int res = 16);
    
    // Write angle to servo (0-180 degrees)
    void write(float angle);
    
    // Write pulse width in microseconds
    void writeMicroseconds(int microseconds);
    
    // Detach servo from pin
    void detach();
    
    // Set custom pulse width range
    void setPulseWidthRange(int minUs, int maxUs);
    
    // Set custom angle range
    void setAngleRange(int minDeg, int maxDeg);
};

#endif // SERVO_CONTROL_H 