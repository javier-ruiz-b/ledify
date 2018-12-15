#pragma once
#include <stdint.h>

class Color {

private:
    template<class T>
    static uint32_t componentsToPixel   (T r,
                                         T g,
                                         T b,
                                         T w) {
       return  ((static_cast<uint32_t>(w) & 255) << 24)  |
               ((static_cast<uint32_t>(r) & 255) << 16)  |
               ((static_cast<uint32_t>(g) & 255) << 8)  |
                (static_cast<uint32_t>(b) & 255);
    }

    uint8_t add(uint8_t component1, uint8_t component2);

public:
    Color() = default;

    template<class T>
    Color(T r, T g, T b, T w) {
        m_rgbw = componentsToPixel(r, g, b, w);
    }

    Color(uint32_t rgbw) : m_rgbw(rgbw) {}
    Color(const Color &color, float multiply);

    Color add(Color &otherColor);

    uint32_t rgbw() const;

    uint8_t r() const;
    uint8_t g() const;
    uint8_t b() const;
    uint8_t w() const;


private:
    uint32_t m_rgbw;
};
