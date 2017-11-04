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
    uint32 diffMs = millis() - m_startTimeMs;
    if (diffMs > SHOW_EVERY_MS) {
        uint32 fps = m_ticks / (diffMs / 1000);
        uint32 fpsDec = ((m_ticks * 10 / diffMs) % 10);
        print("FPS: %lu.%lu, ticks: %lu\n",
              static_cast<long unsigned int>(fps),
              static_cast<long unsigned int>(fpsDec),
              static_cast<long unsigned int>(m_ticks));
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
