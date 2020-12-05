#include "Ws2811LedStrip.h"
#include <QtDebug>
#include <Config.h>
#include <Layer.h>

#include <thread>

#define GPIO_PIN        18
#define DMA             10

#define TARGET_FREQ     WS2811_TARGET_FREQ
#define STRIP_TYPE      SK6812_STRIP_GRBW

Ws2811LedStrip::Ws2811LedStrip() : ILedStrip() {}

void Ws2811LedStrip::initialize() {
    m_ledCount = Config::instance()->ledCount();
    m_ledBuffer.resize(m_ledCount);
    memset(m_ledBuffer.data(), 0, m_ledCount * sizeof(uint32_t));

    m_ledStrip.freq = TARGET_FREQ;
    m_ledStrip.dmanum = DMA;
    m_ledStrip.channel[0].gpionum = GPIO_PIN;
    m_ledStrip.channel[0].invert = 0;
    m_ledStrip.channel[0].count = static_cast<int>(m_ledCount);
    m_ledStrip.channel[0].strip_type = STRIP_TYPE;
    m_ledStrip.channel[0].brightness = 255;

    ws2811_return_t errCode;
    if ((errCode = ws2811_init(&m_ledStrip)) != WS2811_SUCCESS) {
        qWarning("ws2811_init failed: %s\n", ws2811_get_return_t_str(errCode));
    }
    //use QVector:
    free(m_ledStrip.channel[0].leds);
    m_ledStrip.channel[0].leds = m_ledBuffer.data();
}

void Ws2811LedStrip::deinitialize() {
    m_ledStrip.channel[0].leds = nullptr;
    ws2811_fini(&m_ledStrip);
}

void Ws2811LedStrip::render(Layer *rootLayer) {
//    memset(m_ledBuffer.data(), 0, m_ledCount * sizeof(uint32_t));
    rootLayer->draw(m_ledBuffer);
    ws2811_return_t errCode;
    if ((errCode = ws2811_render(&m_ledStrip)) != WS2811_SUCCESS) {
        qWarning("ws2811_render failed: %s\n", ws2811_get_return_t_str(errCode));
    }
}

bool Ws2811LedStrip::isAnyLedOn() {
    for (uint32_t i = 0; i < m_ledCount; i++) {
        if (m_ledBuffer[i] != 0) {
             return true;
        }
    }
    return false;
}
