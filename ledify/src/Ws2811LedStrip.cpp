#include "Ws2811LedStrip.h"
#include <QtDebug>
#include <Layer.h>

#define GPIO_PIN        18
#define DMA             10

#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      SK6812_STRIP_GRBW

Ws2811LedStrip::Ws2811LedStrip(uint32_t numLeds)
    : m_numLeds(numLeds) {}

void Ws2811LedStrip::initialize() {
    m_ledStrip.freq = TARGET_FREQ;
    m_ledStrip.dmanum = DMA;
    m_ledStrip.channel[0].gpionum = GPIO_PIN;
    m_ledStrip.channel[0].invert = 0;
    m_ledStrip.channel[0].count = static_cast<int>(m_numLeds);
    m_ledStrip.channel[0].strip_type = STRIP_TYPE;
    m_ledStrip.channel[0].brightness = 255;

    ws2811_return_t errCode;
    if ((errCode = ws2811_init(&m_ledStrip)) != WS2811_SUCCESS) {
        qWarning("ws2811_init failed: %s\n", ws2811_get_return_t_str(errCode));
    }
    m_ledBuffer = m_ledStrip.channel[0].leds;
}

void Ws2811LedStrip::deinitialize() {
    ws2811_fini(&m_ledStrip);
}

void Ws2811LedStrip::render(Layer *rootLayer) {
//    for (uint16_t i = 0; i < m_numLeds; i++) {
//        m_ledBuffer[i] = rootLayer->pixel(i);
//    }
    rootLayer->draw(m_ledBuffer, m_numLeds);
    ws2811_return_t errCode;
    if ((errCode = ws2811_render(&m_ledStrip)) != WS2811_SUCCESS) {
        qWarning("ws2811_render failed: %s\n", ws2811_get_return_t_str(errCode));
    }
}
