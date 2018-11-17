#include "Interpolator.h"
#include <cmath>

float Interpolator::value(Type type, float current, float end) {
    switch (type) {
    case InterpolatorAccelerate:
        return acceleratedValue(current, end);
    case InterpolatorDecelerate:
        return deceleratedValue(current, end);
    case InterpolatorLinear:
    default:
        return linearValue(current, end);
    }
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


