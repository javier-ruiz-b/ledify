#include "ColorConversion.h"

uint32 ColorConversion::argbToRgb(uint32 argb) {
    byte alpha = (byte)(argb >> 24);
    byte r = (byte)(argb >> 16);
    byte g = (byte)(argb >> 8);
    byte b = (byte)(argb);
    uint32 rgb =    ((uint32)(r * alpha) << 16) |
                    ((uint32)(g * alpha) << 8) |
                    (uint32)(b * alpha);
    return rgb;
}
