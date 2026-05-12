/**
 * @file Utils.h
 * @author Bruno Leppe 
 * @date 12/5/2026
 */

#ifndef LIGHTWEIGTHGUI_UTILS_H
#define LIGHTWEIGTHGUI_UTILS_H
#include "Types.h"

namespace lw {
struct DrawingUtils {

    static LwColor GetElevationColor(const LwColor& baseThemeColor, float elevationAlpha) {
        // Clamp alpha between 0.0f and 1.0f just to be safe
        if (elevationAlpha < 0.0f) elevationAlpha = 0.0f;
        if (elevationAlpha > 1.0f) elevationAlpha = 1.0f;

        LwColor result;

        // Foreground is white (255), so we interpolate between base and 255
        result.r = (unsigned char)((float)baseThemeColor.r * (1.0f - elevationAlpha) + 255.0f * elevationAlpha);
        result.g = (unsigned char)((float)baseThemeColor.g * (1.0f - elevationAlpha) + 255.0f * elevationAlpha);
        result.b = (unsigned char)((float)baseThemeColor.b * (1.0f - elevationAlpha) + 255.0f * elevationAlpha);

        // The resulting color is 100% solid/opaque!
        result.a = 255;

        return result;
    }
};
}

#endif //LIGHTWEIGTHGUI_UTILS_H
