#pragma once
#include "definitions.h"

class ColorConversion {
public:
    //! WRGB
    uint32 rgbaToWrgb(uint32 rgba);
    uint32 argbToRgb(uint32 argb);

private:
    ColorConversion() {}
};
