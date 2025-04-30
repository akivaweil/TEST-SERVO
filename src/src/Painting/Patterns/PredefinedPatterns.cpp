#include "PredefinedPatterns.h"
// #include "PatternActions.h" // No longer needed here
#include "../../Main/SharedGlobals.h" // Needed for PnP variables used in helper
// #include "../Painting.h" // No longer needed here
#include <Arduino.h>
#include "../../Main/GlobalFormulas.h" // Include the new global formulas header
// #include "../../Main/GeneralSettings_PinDef.h" // No longer needed here

// === Helper: Calculate Spacing ===
// Calculates column and row spacing based on global settings.
// This is kept here as it's used by all side-specific pattern files.
void calculatePaintingSpacing(float &colSpacing, float &rowSpacing) {
    // Use the global formula instead of duplicating the logic
    calculateSpacing(colSpacing, rowSpacing, 
                     SquareWidth, 
                     gapX, gapY);
}

// <<< REMOVED executeStandardSerpentineUpDown definition >>>


// <<< REMOVED executeStandardSerpentineSideways definition >>> 