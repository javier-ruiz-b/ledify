#include "CenterToEdgesAnimationLayer.h"

#include <QtDebug>
#include <QLoggingCategory>
Q_LOGGING_CATEGORY(FADE, "ledify.centertoedges", QtWarningMsg)

CenterToEdgesAnimationLayer::CenterToEdgesAnimationLayer() {

}

void CenterToEdgesAnimationLayer::setParams(QSharedPointer<Layer> background, Interpolator::Type interpolator, uint32_t color, uint16_t pixels, uint16_t durationMs, float endDistance) {
    m_background = background;
    m_interpolator = interpolator;
    m_color = color;
    m_pixels = pixels;
    m_durationMs = durationMs;
    m_endDistance = endDistance;
}
