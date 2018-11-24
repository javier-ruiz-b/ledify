#pragma once
#include <stdint.h>

#define ACCELERATION_FACTOR 2.0

class Interpolator {

public:
    enum Type {
        InterpolatorLinear,
        InterpolatorAccelerate,
        InterpolatorAccelerate4x,
        InterpolatorDecelerate,
        InterpolatorDecelerate4x
    };

public:
    static float value(Type type, float current, float end);

    static inline float acceleratedValue(float current, float end, float factor = ACCELERATION_FACTOR);
    static inline float deceleratedValue(float current, float end, float factor = ACCELERATION_FACTOR);
    static inline float linearValue(float current, float end);

private:
    Interpolator(){}
};
