#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
extern const int SERVO_PIN;
extern const int SERVO_CHANNEL;
extern const int STATUS_LED_PIN;

// Timing configuration
extern const unsigned long SERVO_MOVE_INTERVAL;
extern const unsigned long RANDOM_SEED_DELAY;

// Servo configuration
extern const float SERVO_MIN_ANGLE;
extern const float SERVO_MAX_ANGLE;
extern const int SERVO_FREQUENCY;
extern const int SERVO_RESOLUTION;

// OTA configuration
extern const char* OTA_HOSTNAME;

#endif // CONFIG_H 