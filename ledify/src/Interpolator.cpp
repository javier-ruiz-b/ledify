#include "Interpolator.h"
#include <cmath>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(INTERPOLATOR, "ledify.interpolator", QtWarningMsg)


float Interpolator::value(Type type, float current, float end) {
    switch (type) {
    case InterpolatorAccelerate:
        return acceleratedValue(current, end);
    case InterpolatorAccelerate4x:
        return acceleratedValue(current, end, 4);
    case InterpolatorDecelerate:
        return deceleratedValue(current, end);
    case InterpolatorDecelerate4x:
        return deceleratedValue(current, end, 4);
    case InterpolatorLinear:
//    default:
        return linearValue(current, end);
    }
    qCWarning(INTERPOLATOR) << "Wrong interpolator type:" << type;
    return 0;
}

float Interpolator::acceleratedValue(float current, float end, float factor) {
    auto currentPow = powf(current, factor);
    auto endPow = powf(end, factor);
    return currentPow / endPow;
}

float Interpolator::deceleratedValue(float current, float end, float factor) {
    auto endPow = powf(end, factor);
    auto currentPow = powf(end - current, factor);
    auto currentPowInv = endPow - currentPow;
    return currentPowInv / endPow;
}

float Interpolator::linearValue(float current, float end) {
    return current / end;
}


