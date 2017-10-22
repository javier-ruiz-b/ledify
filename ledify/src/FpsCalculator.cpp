#include "FpsCalculator.h"

#define SHOW_EVERY_MS 2000

FpsCalculator::FpsCalculator() {
    m_startTimeMs = 0;
    m_ticks = 0;
    m_enabled = false;
#ifdef DEBUG
    m_enabled = true;
#endif
}

void FpsCalculator::check() {
    uint32 diff = millis() - m_startTimeMs;
    if (diff > SHOW_EVERY_MS) {
        uint32 fps = m_ticks / diff ;
        uint32 fpsDec = ((m_ticks * 10 / diff) % 10);
        print("FPS: %u.%u, ticks: %u", fps, fpsDec, m_ticks);
        m_ticks = 0;
        m_startTimeMs = 0;
    }
}

void FpsCalculator::tick() {
    if (!m_enabled) {
        return;
    }
    m_ticks++;

    if (m_startTimeMs == 0) {
        m_startTimeMs = millis();
    }

    if ((m_ticks % 128) == 0) {
        check();
    }

}

void FpsCalculator::setEnabled(bool enabled) {
    m_enabled = enabled;
    m_startTimeMs = 0;
    m_ticks = 0;
}
