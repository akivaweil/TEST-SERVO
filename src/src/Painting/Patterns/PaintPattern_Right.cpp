#include "PaintPatterns_SideSpecific.h"
#include "PatternActions.h"
#include "PredefinedPatterns.h"
#include "../../Main/SharedGlobals.h"
#include "../Painting.h"
#include <Arduino.h>
#include "../../Main/GeneralSettings_PinDef.h"
#include "../../Main/GlobalFormulas.h"

// === Right Side Pattern (Side 1) ===
bool executePaintPatternRight(float speed, float accel) {
    const int sideIndex = 1;
    int patternType = paintPatternType[sideIndex];

    Serial.printf("[Pattern Sequence] Executing RIGHT Side Pattern (Side %d) - Type: %s\n", 
                  sideIndex, (patternType == 0) ? "Up/Down" : (patternType == 90 ? "Sideways" : "Unknown"));

    float currentX = 0.0, currentY = 0.0;
    bool stopped = false;

    // --- Shared Pattern Setup ---
    Serial.println("  Setting up RIGHT pattern...");
    
    int targetAngle = ROT_POS_RIGHT_DEG;
    Serial.printf("    Target Rotation: %d degrees\n", targetAngle);

    // Calculate Spacing
    float colSpacing = 0.0, rowSpacing = 0.0;
    calculatePaintingSpacing(colSpacing, rowSpacing);
    float verticalSweepDistance = (gridRows > 1) ? ((gridRows - 1) * rowSpacing) : 0.0f;
    float horizontalSweepDistance = (gridCols > 1) ? ((gridCols - 1) * colSpacing) : 0.0f;
    Serial.printf("    Spacing: Col=%.3f, Row=%.3f, VertSweep=%.3f, HorizSweep=%.3f\n", 
                  colSpacing, rowSpacing, verticalSweepDistance, horizontalSweepDistance);

    // --- Pattern Specific Setup & Execution ---

    if (patternType == 0) { // --- Up/Down Pattern ---
        Serial.println("  Executing Up/Down Pattern Logic for Right...");

        // Start XY & Sweep Direction
        float startX = 29.5f;
        float startY = 20.0f;
        bool sweepDownFirst = true;
        Serial.printf("    Start Corner: (%.3f, %.3f), First Sweep: DOWN vvv\n", startX, startY);
        
        // Sweep and Shift distances
        float verticalSweepDist = trayWidth;
        float horizontalShiftDist = 3.0f + gapY;
        Serial.printf("    Sweep Vertical: %.3f, Shift Horizontal (3 + GapY): %.3f (GapY=%.3f)\n", 
                      verticalSweepDist, horizontalShiftDist, gapY);

        // Execute Action Sequence
        // Rotation is now handled in paintSide function before this pattern is called
        float startZ = paintZHeight[sideIndex];
        Serial.printf("    Moving Z to paint height: %.3f\n", startZ);
        stopped = actionMoveToZ(startZ, patternZSpeed, patternZAccel);
        if (stopped) { Serial.println("Pattern stopped during Z Move."); return true; }
        currentX = stepsToInchesXY(stepper_x->getCurrentPosition());
        currentY = stepsToInchesXY(stepper_y_left->getCurrentPosition()); // Assume Y steppers are synced
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
        bool currentSweepDown = sweepDownFirst;
        for (int c = 0; c < gridRows; ++c) { // Loop ROWS times
            Serial.printf("\n  -- Column/Sweep %d --\n", c);
            if (c > 0) {
                Serial.printf("    Shifting horizontally LEFT by %.3f (3.0 + %.3f)\n", horizontalShiftDist, gapY);
                stopped = actionShiftXY(-horizontalShiftDist, 0.0f, currentX, currentY, speed, accel); // Use NEGATIVE horizontal shift
                if (stopped) { Serial.printf("Pattern stopped during Shift to Column %d.\n", c); return true; }
            }
            if (verticalSweepDist > 0.001) {
                Serial.printf("    Sweeping vertically (Down=%s) by %.3f\n", currentSweepDown ? "true" : "false", verticalSweepDist);
                stopped = actionSweepVertical(currentSweepDown, verticalSweepDist, currentX, currentY, speed, accel);
                if (stopped) { Serial.printf("Pattern stopped during Vertical Sweep in Column %d.\n", c); return true; }
            } else {
                Serial.println("    Skipping vertical sweep (distance is zero).");
            }
            currentSweepDown = !currentSweepDown;
        }

    } else if (patternType == 90) { // --- Sideways Pattern ---
        Serial.println("  Executing Sideways Pattern Logic for Right...");
        
        // Start XY & Sweep Direction
        float startX = 29.5f;
        float startY = 20.0f;
        bool sweepLeftFirst = true;
        Serial.printf("    Start Corner: (%.3f, %.3f), First Sweep: LEFT <<<\n", startX, startY);
        
        // Sweep and Shift distances
        float horizontalSweepDist = trayHeight;
        float verticalShiftDist = 3.0f + gapX;
        Serial.printf("    Sweep Horizontal: %.3f, Shift Vertical (3 + GapX): %.3f (GapX=%.3f)\n", 
                      horizontalSweepDist, verticalShiftDist, gapX);

        // Execute Action Sequence
        // Rotation is now handled in paintSide function before this pattern is called
        float startZ = paintZHeight[sideIndex];
        Serial.printf("    Moving Z to paint height: %.3f\n", startZ);
        stopped = actionMoveToZ(startZ, patternZSpeed, patternZAccel);
        if (stopped) { Serial.println("Pattern stopped during Z Move."); return true; }
        currentX = stepsToInchesXY(stepper_x->getCurrentPosition());
        currentY = stepsToInchesXY(stepper_y_left->getCurrentPosition()); // Assume Y steppers are synced
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
        bool currentSweepLeft = sweepLeftFirst;
        for (int r = 0; r < gridCols; ++r) { // Loop COLS times
            Serial.printf("\n  -- Row/Sweep %d --\n", r);
            if (r > 0) {
                Serial.printf("    Shifting vertically DOWN by %.3f (3.0 + %.3f)\n", verticalShiftDist, gapX);
                stopped = actionShiftXY(0.0f, -verticalShiftDist, currentX, currentY, speed, accel); // Use NEGATIVE vertical shift
                if (stopped) { Serial.printf("Pattern stopped during Shift to Row %d.\n", r); return true; }
            }
            if (horizontalSweepDist > 0.001) {
                // Note: actionSweepHorizontal takes sweepRight flag. sweepLeft=true means sweepRight=false
                bool sweepRight = !currentSweepLeft;
                Serial.printf("    Sweeping horizontally (Left=%s) by %.3f\n", currentSweepLeft ? "true" : "false", horizontalSweepDist);
                stopped = actionSweepHorizontal(sweepRight, horizontalSweepDist, currentY, currentX, speed, accel);
                if (stopped) { Serial.printf("Pattern stopped during Horizontal Sweep in Row %d.\n", r); return true; }
            } else {
                Serial.println("    Skipping horizontal sweep (distance is zero).");
            }
            currentSweepLeft = !currentSweepLeft;
        }

    } else { // --- Unknown Pattern Type ---
        Serial.printf("[ERROR] Unknown paintPatternType: %d for Side %d\n", patternType, sideIndex);
        webSocket.broadcastTXT("{\"status\":\"Error\", \"message\":\"Unknown pattern type selected for Right side.\"}");
        return true;
    }

    // --- Pattern Completion ---
    Serial.printf("[Pattern Sequence] RIGHT Side Pattern (Type %d) COMPLETED.\n", patternType);
    return false;
}