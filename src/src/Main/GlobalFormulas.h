#ifndef GLOBAL_FORMULAS_H
#define GLOBAL_FORMULAS_H

#include <Arduino.h>

/**
 * @file GlobalFormulas.h
 * @brief Centralized file for all formulas and calculations used in the Paint Machine
 * 
 * This file organizes mathematical formulas and unit conversions into logical sections.
 * When modifying calculations, update them here rather than in individual files.
 */

// =====================================================================
// SECTION 1: MOTOR AND STEPS CONVERSION FACTORS
// =====================================================================

/**
 * @brief Steps per inch for X/Y axes
 * Formula: (Steps per revolution / (teeth * belt pitch)) * mm per inch
 * = (400 steps/rev / (20 teeth * 2mm pitch)) * 25.4 mm/in
 */
#define STEPS_PER_INCH_XY 254.0f

/**
 * @brief Steps per inch for Z axis
 */
#define STEPS_PER_INCH_Z 254.0f

/**
 * @brief Steps per degree for rotation
 * Formula: Steps per revolution / 360 degrees
 * = 4000.0f / 360.0f = 11.11111f steps per degree
 */
#define STEPS_PER_DEGREE 11.11111f

// =====================================================================
// SECTION 2: MOVEMENT AND HOMING PARAMETERS
// =====================================================================

/**
 * @brief Homing speed in steps/second
 */
#define HOMING_SPEED 2000

/**
 * @brief Homing acceleration in steps/second²
 */
#define HOMING_ACCEL 12500

/**
 * @brief Homing timeout in milliseconds
 */
#define HOMING_TIMEOUT 15000

/**
 * @brief Debounce interval for switches in milliseconds
 */
#define DEBOUNCE_INTERVAL 5

// =====================================================================
// SECTION 3: TRAVEL LIMITS
// =====================================================================

/**
 * @brief Maximum X travel in positive direction (inches)
 */
#define X_MAX_TRAVEL_POS_INCH 30.0f

/**
 * @brief Maximum Y travel in positive direction (inches)
 */
#define Y_MAX_TRAVEL_POS_INCH 30.0f

/**
 * @brief Z home position in inches
 */
#define Z_HOME_POS_INCH 0.0f

/**
 * @brief Minimum Z travel in negative direction (inches)
 */
#define Z_MAX_TRAVEL_NEG_INCH 0.0f

/**
 * @brief Maximum Z travel in positive direction (inches)
 */
#define Z_MAX_TRAVEL_POS_INCH 2.75f

// =====================================================================
// SECTION 4: PICK AND PLACE PARAMETERS
// =====================================================================

// This section intentionally left empty
// Up and down motion is controlled by pneumatic cylinder attached to pin 10

// =====================================================================
// SECTION 5: SERVO CONTROL PARAMETERS
// =====================================================================

/**
 * @brief Initial position for pitch servo (degrees)
 */
#define SERVO_INIT_POS_PITCH 30

/**
 * @brief Delay between servo movement steps (milliseconds)
 */
#define SERVO_MOVEMENT_DELAY_MS 75

// =====================================================================
// SECTION 6: PATTERN TIMING PARAMETERS
// =====================================================================

/**
 * @brief Default delay between pattern repetitions (milliseconds)
 */
#define DEFAULT_PATTERN_DELAY 1000

// =====================================================================
// SECTION 7: CALCULATION FUNCTIONS
// =====================================================================

/**
 * @brief Converts inches to motor steps for X/Y axes
 * @param inches Distance in inches
 * @return long Number of steps
 */
inline long inchesToStepsXY(float inches) {
    return (long)(inches * STEPS_PER_INCH_XY);
}

/**
 * @brief Converts motor steps to inches for X/Y axes
 * @param steps Number of steps
 * @return float Distance in inches
 */
inline float stepsToInchesXY(long steps) {
    return (float)steps / STEPS_PER_INCH_XY;
}

/**
 * @brief Converts inches to motor steps for Z axis
 * @param inches Distance in inches
 * @return long Number of steps
 */
inline long inchesToStepsZ(float inches) {
    return (long)(inches * STEPS_PER_INCH_Z);
}

/**
 * @brief Converts motor steps to inches for Z axis
 * @param steps Number of steps
 * @return float Distance in inches
 */
inline float stepsToInchesZ(long steps) {
    return (float)steps / STEPS_PER_INCH_Z;
}

/**
 * @brief Converts degrees to motor steps for rotation
 * @param degrees Angle in degrees
 * @return long Number of steps
 */
inline long degreesToSteps(float degrees) {
    return (long)(degrees * STEPS_PER_DEGREE);
}

/**
 * @brief Converts motor steps to degrees for rotation
 * @param steps Number of steps
 * @return float Angle in degrees
 */
inline float stepsToDegrees(long steps) {
    return (float)steps / STEPS_PER_DEGREE;
}

/**
 * @brief Calculates spacing for painting patterns
 * @param colSpacing Reference to store calculated column spacing (inches)
 * @param rowSpacing Reference to store calculated row spacing (inches)
 * @param squareWidth Width of square item in inches
 * @param gapX Horizontal gap between items in inches
 * @param gapY Vertical gap between items in inches
 */
inline void calculateSpacing(float &colSpacing, float &rowSpacing, 
                            float squareWidth,
                            float gapX, float gapY) {
    colSpacing = squareWidth + gapX;
    rowSpacing = squareWidth + gapY;
}

/**
 * @brief Calculates gap spacing for a grid layout
 * 
 * X gap = (trayWidth - (borderWidth * 2) - (SquareWidth * columns)) / (columns - 1)
 * Y gap = (trayHeight - (borderWidth * 2) - (SquareWidth * rows)) / (rows - 1)
 * 
 * @param gapX Reference to store calculated X gap between items (inches)
 * @param gapY Reference to store calculated Y gap between items (inches)
 * @param trayWidth Width of the tray in inches
 * @param trayHeight Height of the tray in inches
 * @param borderWidth Width of the border in inches
 * @param squareWidth Width of each square item in inches
 * @param columns Number of columns in the grid
 * @param rows Number of rows in the grid
 */
inline void calculateGaps(float &gapX, float &gapY,
                         float trayWidth, float trayHeight,
                         float borderWidth, float squareWidth,
                         int columns, int rows) {
    // Calculate X-direction gap
    if (columns > 1) {
        gapX = (trayWidth - (2 * borderWidth) - (squareWidth * columns)) / (columns - 1);
    } else {
        gapX = 0; // No gap needed for 1 column
    }
    
    // Calculate Y-direction gap
    if (rows > 1) {
        gapY = (trayHeight - (2 * borderWidth) - (squareWidth * rows)) / (rows - 1);
    } else {
        gapY = 0; // No gap needed for 1 row
    }
    
    // Clamp negative gaps to zero
    if (gapX < 0) gapX = 0;
    if (gapY < 0) gapY = 0;
}

#endif // GLOBAL_FORMULAS_H 