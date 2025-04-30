#include "PaintPatterns_SideSpecific.h"
#include "PatternActions.h"
#include "PredefinedPatterns.h"
#include "../../Main/SharedGlobals.h"
#include "../Painting.h"
#include <Arduino.h>
#include "../../Main/GeneralSettings_PinDef.h"

// === Left Side Pattern (Side 3) ===
bool executePaintPatternLeft(float speed, float accel) {
    const int sideIndex = 3;
    int patternType = paintPatternType[sideIndex];

    Serial.printf("[Pattern Sequence] Executing LEFT Side Pattern (Side %d) - Type: %s\n", 
                  sideIndex, (patternType == 0) ? "Up/Down" : (patternType == 90 ? "Sideways" : "Unknown"));

    float currentX = 0.0, currentY = 0.0;
    bool stopped = false;

    // --- Shared Pattern Setup --- 
    Serial.println("  Setting up LEFT pattern...");
    
    int targetAngle = ROT_POS_LEFT_DEG;
    Serial.printf("    Target Rotation: %d degrees\n", targetAngle);

    float colSpacing = 0.0, rowSpacing = 0.0;
    calculatePaintingSpacing(colSpacing, rowSpacing);
    float verticalSweepDistance = (gridRows > 1) ? ((gridRows - 1) * rowSpacing) : 0.0f;
    float horizontalSweepDistance = (gridCols > 1) ? ((gridCols - 1) * colSpacing) : 0.0f;
    Serial.printf("    Spacing: Col=%.3f, Row=%.3f, VertSweep=%.3f, HorizSweep=%.3f\n", 
                  colSpacing, rowSpacing, verticalSweepDistance, horizontalSweepDistance);

    // --- Perform Rotation to Left Side Using actionRotateTo ---
    Serial.printf("    Rotating to %d degrees\n", targetAngle);
    // Rotation is now handled in paintSide function before this pattern is called
    // stopped = actionRotateTo(targetAngle);
    // if (stopped) {
    //     Serial.println("Pattern stopped during rotation.");
    //     return true;
    // }

    // --- Pattern Specific Setup & Execution --- 

    if (patternType == 0) { // --- Up/Down Pattern --- 
        Serial.println("  Executing Up/Down Pattern Logic for Left...");

        // Start XY & Sweep Direction
        float startX = 8.0f;
        float startY = 6.5f;
        bool sweepUpFirst = true;
        Serial.printf("    Start Corner: (%.3f, %.3f), First Sweep: UP ^^^\n", startX, startY);
        
        // Sweep and Shift distances
        float verticalSweepDist = trayWidth;
        float horizontalShiftDist = 3.0f + gapY;
        Serial.printf("    Sweep Vertical: %.3f, Shift Horizontal (3 + GapY): %.3f (GapY=%.3f)\n", 
                      verticalSweepDist, horizontalShiftDist, gapY);

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
        Serial.println("  Starting column sweeps...");
        bool currentSweepUp = sweepUpFirst;
        for (int c = 0; c < gridRows; ++c) {
            Serial.printf("\n  -- Column %d --\n", c);
            
            if (c > 0) {
                Serial.printf("    Shifting horizontally by %.3f (3.0 + %.3f)\n", horizontalShiftDist, gapY);
                stopped = actionShiftXY(horizontalShiftDist, 0.0f, currentX, currentY, speed, accel);
                if (stopped) { Serial.printf("Pattern stopped during Shift to Column %d.\n", c); return true; }
            }
            
            if (verticalSweepDist > 0.001) { 
                bool sweepDown = !currentSweepUp;
                Serial.printf("    Sweeping vertically (Up=%s) by %.3f\n", currentSweepUp ? "true" : "false", verticalSweepDist);
                stopped = actionSweepVertical(sweepDown, verticalSweepDist, currentX, currentY, speed, accel);
                if (stopped) { Serial.printf("Pattern stopped during Vertical Sweep in Column %d.\n", c); return true; }
            } else {
                Serial.println("    Skipping vertical sweep (distance is zero).");
            }
            currentSweepUp = !currentSweepUp;
        }

    } else if (patternType == 90) { // --- Sideways Pattern --- 
        Serial.println("  Executing Sideways Pattern Logic for Left...");
        
        // Start XY & Sweep Direction
        float startX = 8.0f;
        float startY = 6.5f;
        bool sweepRightFirst = true;
        Serial.printf("    Start Corner: (%.3f, %.3f), First Sweep: RIGHT >>>\n", startX, startY);
        
        // Sweep and Shift distances
        float horizontalSweepDist = trayHeight;
        float verticalShiftDist = 3.0f + gapX;
        Serial.printf("    Sweep Horizontal: %.3f, Shift Vertical (3 + GapX): %.3f (GapX=%.3f)\n", 
                      horizontalSweepDist, verticalShiftDist, gapX);

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
        Serial.println("  Starting row sweeps...");
        bool currentSweepRight = sweepRightFirst;
        for (int r = 0; r < gridCols; ++r) {
            Serial.printf("\n  -- Row %d --\n", r);
            if (r > 0) {
                Serial.printf("    Shifting vertically by %.3f (3.0 + %.3f)\n", verticalShiftDist, gapX);
                stopped = actionShiftXY(0.0f, verticalShiftDist, currentX, currentY, speed, accel);
                if (stopped) { Serial.printf("Pattern stopped during Shift to Row %d.\n", r); return true; }
            }
            if (horizontalSweepDist > 0.001) { 
                Serial.printf("    Sweeping horizontally (Right=%s) by %.3f\n", currentSweepRight ? "true" : "false", horizontalSweepDist);
                stopped = actionSweepHorizontal(currentSweepRight, horizontalSweepDist, currentY, currentX, speed, accel); 
                if (stopped) { Serial.printf("Pattern stopped during Horizontal Sweep in Row %d.\n", r); return true; }
            } else {
                Serial.println("    Skipping horizontal sweep (distance is zero).");
            }
            currentSweepRight = !currentSweepRight;
        }

    } else { // --- Unknown Pattern Type --- 
        Serial.printf("[ERROR] Unknown paintPatternType: %d for Side %d\n", patternType, sideIndex);
        webSocket.broadcastTXT("{\"status\":\"Error\", \"message\":\"Unknown pattern type selected for Left side.\"}");
        return true;
    }

    // --- Pattern Completion --- 
    Serial.printf("[Pattern Sequence] LEFT Side Pattern (Type %d) COMPLETED.\n", patternType);
    return false;
} 