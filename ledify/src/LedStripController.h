#pragma once
#include "definitions.h"
#include "FadeLayer.h"
#include "ColorLayer.h"

#define COLOR_LAYERS    4
#define FADE_LAYERS     2

class LedStripController {
public:
    LedStripController() {}

private:
    ColorLayer m_color[COLOR_LAYERS];
    FadeLayer m_layer[FADE_LAYERS];

};
