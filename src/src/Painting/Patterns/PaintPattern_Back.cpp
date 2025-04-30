#include "PaintPatterns_SideSpecific.h"
#include "PatternActions.h"
#include "PredefinedPatterns.h"
#include "../../Main/SharedGlobals.h"
#include "../Painting.h"
#include <Arduino.h>
#include "../../Main/GeneralSettings_PinDef.h"

// === Back Side Pattern (Side 0) ===
bool executePaintPatternBack(float speed, float accel) {
    const int sideIndex = 0;
    int patternType = paintPatternType[sideIndex];

    Serial.printf("[Pattern Sequence] Executing BACK Side Pattern (Side %d) - Type: %s\n", 
                  sideIndex, (patternType == 0) ? "Up/Down" : (patternType == 90 ? "Sideways" : "Unknown"));

    float currentX = 0.0, currentY = 0.0;
    bool stopped = false;

    // --- Shared Pattern Setup --- 
    Serial.println("  Setting up BACK pattern...");
    
    int targetAngle = ROT_POS_BACK_DEG;
    Serial.printf("    Target Rotation: %d degrees\n", targetAngle);

    // Calculate Spacing
    float colSpacing = 0.0, rowSpacing = 0.0;
    calculatePaintingSpacing(colSpacing, rowSpacing);
    float verticalSweepDistance = (gridRows > 1) ? ((gridRows - 1) * rowSpacing) : 0.0f;
    float horizontalSweepDistance = (gridCols > 1) ? ((gridCols - 1) * colSpacing) : 0.0f;
    Serial.printf("    Spacing: Col=%.3f, Row=%.3f, VertSweep=%.3f, HorizSweep=%.3f\n", 
                  colSpacing, rowSpacing, verticalSweepDistance, horizontalSweepDistance);

    // --- Perform Rotation to Back Side Using actionRotateTo ---
    Serial.printf("    Rotating to %d degrees\n", targetAngle);
    // Rotation is now handled in paintSide function before this pattern is called
    // stopped = actionRotateTo(targetAngle);
    // if (stopped) {
    //     Serial.println("Pattern stopped during rotation.");
    //     return true;
    // }

    // --- Pattern Specific Setup & Execution --- 

    if (patternType == 0) { // --- Up/Down Pattern --- 
        Serial.println("  Executing Up/Down Pattern Logic for Back...");

        // Start XY & Sweep Direction
        float startX = 25.0f;
        float startY = 30.0f;
        bool sweepDownFirst = true;
        Serial.printf("    Start Corner: (%.3f, %.3f), First Sweep: DOWN vvv\n", startX, startY);

        // Sweep and Shift distances
        float verticalSweepDist = trayHeight;
        float horizontalShiftDist = 3.0f + gapX;
        Serial.printf("    Sweep Vertical: %.3f, Shift Horizontal (3 + GapX): %.3f (GapX=%.3f)\n",
                     verticalSweepDist, horizontalShiftDist, gapX);

        // Execute Action Sequence
        float startZ = paintZHeight[sideIndex];
        Serial.printf("    Moving Z to paint height: %.3f\n", startZ);
        stopped = actionMoveToZ(startZ, patternZSpeed, patternZAccel);
        if (stopped) { Serial.println("Pattern stopped during Z Move."); return true; }
        currentX = (float)stepper_x->getCurrentPosition() / STEPS_PER_INCH_XY;
        currentY = (float)stepper_y_left->getCurrentPosition() / STEPS_PER_INCH_XY;
        Serial.printf("    Moving to Start XY: (%.3f, %.3f)\n", startX, startY);
        stopped = actionMoveToXY(startX, startY, speed, accel, currentX, currentY);
        if (stopped) { Serial.println("Pattern stopped during initial XY Move."); return true; }

        // Ensure rotation is complete before starting painting pattern
        if (stepper_rot && stepper_rot->isRunning()) {
            Serial.println("    Waiting for rotation motor to complete before starting pattern...");
            while (stepper_rot->isRunning()) {
                yield();
                webSocket.loop();
                if (stopRequested) {
                    Serial.println("    Pattern stopped while waiting for rotation to complete.");
                    return true;
                }
            }
            Serial.println("    Rotation complete, proceeding with painting pattern.");
        }

        // Column Loop
        bool currentSweepDown = sweepDownFirst;
        for (int col = 0; col < gridCols; col++) {
            float colX = startX - (col * horizontalShiftDist);
            
            if (currentSweepDown) {
                Serial.printf("    Column %d: Sweeping DOWN (%.3f, %.3f) to (%.3f, %.3f)\n", 
                             col, colX, startY, colX, startY - verticalSweepDist);
                stopped = actionSweepVertical(true, verticalSweepDist, colX, currentY, speed, accel);
                if (stopped) { Serial.println("Pattern stopped during DOWN sweep."); return true; }
            } else {
                Serial.printf("    Column %d: Sweeping UP (%.3f, %.3f) to (%.3f, %.3f)\n", 
                             col, colX, startY - verticalSweepDist, colX, startY);
                stopped = actionSweepVertical(false, verticalSweepDist, colX, currentY, speed, accel);
                if (stopped) { Serial.println("Pattern stopped during UP sweep."); return true; }
            }
            
            if (col < gridCols - 1) {
                currentY = currentSweepDown ? (startY - verticalSweepDist) : startY;
                Serial.printf("    Moving to next column left by %.3f\n", horizontalShiftDist);
                stopped = actionShiftXY(-horizontalShiftDist, 0.0f, currentX, currentY, speed, accel);
                if (stopped) { Serial.println("Pattern stopped during column shift."); return true; }
            }
            
            currentSweepDown = !currentSweepDown;
        }

    } else if (patternType == 90) { // --- Sideways Pattern --- 
        Serial.println("  Executing Sideways Pattern Logic for Back...");
        
        // Start XY & Sweep Direction
        float startX = 25.0f;
        float startY = 30.0f;
        bool sweepLeftFirst = true;
        Serial.printf("    Start Corner: (%.3f, %.3f), First Sweep: LEFT <<<\n", startX, startY);

        // Sweep and Shift distances
        float horizontalSweepDist = trayWidth;
        float verticalShiftDist = 3.0f + gapY;
        Serial.printf("    Sweep Horizontal: %.3f, Shift Vertical (3 + GapY): %.3f (GapY=%.3f)\n", 
                     horizontalSweepDist, verticalShiftDist, gapY);

        // Execute Action Sequence
        float startZ = paintZHeight[sideIndex];
        Serial.printf("    Moving Z to paint height: %.3f\n", startZ);
        stopped = actionMoveToZ(startZ, patternZSpeed, patternZAccel);
        if (stopped) { Serial.println("Pattern stopped during Z Move."); return true; }
        currentX = (float)stepper_x->getCurrentPosition() / STEPS_PER_INCH_XY;
        currentY = (float)stepper_y_left->getCurrentPosition() / STEPS_PER_INCH_XY;
        Serial.printf("    Moving to Start XY: (%.3f, %.3f)\n", startX, startY);
        stopped = actionMoveToXY(startX, startY, speed, accel, currentX, currentY);
        if (stopped) { Serial.println("Pattern stopped during initial XY Move."); return true; }

        // Ensure rotation is complete before starting painting pattern
        if (stepper_rot && stepper_rot->isRunning()) {
            Serial.println("    Waiting for rotation motor to complete before starting pattern...");
            while (stepper_rot->isRunning()) {
                yield();
                webSocket.loop();
                if (stopRequested) {
                    Serial.println("    Pattern stopped while waiting for rotation to complete.");
                    return true;
                }
            }
            Serial.println("    Rotation complete, proceeding with painting pattern.");
        }

        // Row Loop
        bool currentSweepLeft = sweepLeftFirst;
        for (int row = 0; row < gridRows; row++) {
            float rowY = startY - (row * verticalShiftDist);
            
            if (currentSweepLeft) {
                Serial.printf("    Row %d: Sweeping LEFT (%.3f, %.3f) to (%.3f, %.3f)\n", 
                             row, startX, rowY, startX - horizontalSweepDist, rowY);
                stopped = actionSweepHorizontal(false, horizontalSweepDist, rowY, currentX, speed, accel);
                if (stopped) { Serial.println("Pattern stopped during LEFT sweep."); return true; }
            } else {
                Serial.printf("    Row %d: Sweeping RIGHT (%.3f, %.3f) to (%.3f, %.3f)\n", 
                             row, startX - horizontalSweepDist, rowY, startX, rowY);
                stopped = actionSweepHorizontal(true, horizontalSweepDist, rowY, currentX, speed, accel);
                if (stopped) { Serial.println("Pattern stopped during RIGHT sweep."); return true; }
            }
            
            if (row < gridRows - 1) {
                currentX = currentSweepLeft ? (startX - horizontalSweepDist) : startX;
                Serial.printf("    Moving to next row down by %.3f\n", verticalShiftDist);
                stopped = actionShiftXY(0.0f, -verticalShiftDist, currentX, rowY, speed, accel);
                if (stopped) { Serial.println("Pattern stopped during row shift."); return true; }
            }
            
            currentSweepLeft = !currentSweepLeft;
        }
    } else {
        Serial.printf("  Unknown pattern type %d for BACK side\n", patternType);
        webSocket.broadcastTXT("{\"status\":\"Error\", \"message\":\"Unknown pattern type for BACK side\"}");
        return true;
    }

    // End of pattern
    Serial.println("[Pattern Sequence] BACK Side Pattern Complete!");
    return false;
} 