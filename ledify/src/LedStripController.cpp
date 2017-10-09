#include "LedStripController.h"

#ifdef ARDUINO
#include <Adafruit_NeoPixel.h>
#else
#include "FakeNeoPixelLib.h"
#endif
#include "Layer.h"
#include "ColorLayer.h"
#include "FadeLayer.h"


LedStripController::LedStripController(Adafruit_NeoPixel *neoPixelLib) {
    m_rootLayer = nullptr;
    for (byte i = 0; i < AVAILABLE_LAYERS_NUM; i++) {
        m_availableLayers[i] = nullptr;
    }
}

CommandReader &LedStripController::commandReader() {
    return m_commandReader;
}

void LedStripController::writeChar(char c) {
    if (m_commandReader.writeChar(c)) {

    }
}

void LedStripController::draw() {

}
