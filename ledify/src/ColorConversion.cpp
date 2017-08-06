#include "ColorConversion.h"

uint32 ColorConversion::rgbaToWrgb(uint32 rgba)
{

}

uint32 ColorConversion::argbToRgb(uint32 argb) {
    byte alpha = (byte)(argb >> 24);
    byte r = (byte)(argb >> 16);
    byte g = (byte)(argb >> 8);
    byte b = (byte)(argb >> 0);
    uint16 rgb =    ((r * alpha) << 16) |
                    ((g * alpha) << 8) |
                    (b * alpha);
    return rgb;
}
