#include "FpsCalculator.h"
#include "TimeControl.h"
#include <QtDebug>

#define SHOW_EVERY_MS 2000

FpsCalculator::FpsCalculator() {
    m_startTimeMs = 0;
    m_ticks = 0;
    m_enabled = false;
#ifdef DEBUG
    m_enabled = true;
#endif
    m_time = TimeControl::instance();
}

void FpsCalculator::check() {
    uint32_t diffMs = m_time->millis() - m_startTimeMs;
    if (diffMs > SHOW_EVERY_MS) {
        uint32_t fps = m_ticks / (diffMs / 1000);
        uint32_t fpsDec = ((m_ticks * 10 / diffMs) % 10);
        qDebug().nospace() << "FPS: "
              << static_cast<long unsigned int>(fps)
              << "," << static_cast<long unsigned int>(fpsDec)
              << ", ticks: " << static_cast<long unsigned int>(m_ticks);
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
        m_startTimeMs = m_time->millis();
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
