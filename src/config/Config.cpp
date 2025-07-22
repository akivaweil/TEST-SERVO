//* ************************************************************************
//* ************************ CONFIGURATION ***************************
//* ************************************************************************
// Configuration settings for the servo test project.

// Timing configuration
const unsigned long SERVO_MOVE_INTERVAL = 2000;  // Time between servo movements (ms)
const unsigned long RANDOM_SEED_DELAY = 1000;    // Delay before starting random movements

// Servo configuration
const float SERVO_MIN_ANGLE = 0.0;    // Minimum servo angle (degrees)
const float SERVO_MAX_ANGLE = 180.0;  // Maximum servo angle (degrees)
const int SERVO_FREQUENCY = 50;       // PWM frequency (Hz)
const int SERVO_RESOLUTION = 16;      // PWM resolution (bits)

// OTA configuration
const char* OTA_HOSTNAME = "servo-test-esp32";  // Hostname for OTA updates
