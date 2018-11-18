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

public:
    Color() = default;

    template<class T>
    Color(T r, T g, T b, T w) {
        m_pixel = componentsToPixel(r, g, b, w);
    }

    Color(uint32_t pixel) : m_pixel(pixel) {}
    Color(const Color &color, float multiply) {
        m_pixel = componentsToPixel(color.r() * multiply,
                                    color.g() * multiply,
                                    color.b() * multiply,
                                    color.w() * multiply);
    }

    uint32_t pixel() const {
        return m_pixel;
    }

    uint8_t w() const {
        return static_cast<uint8_t>(m_pixel >> 24);
    }

    uint8_t r() const {
        return static_cast<uint8_t>((m_pixel >> 16) & 255);
    }

    uint8_t g() const {
        return static_cast<uint8_t>((m_pixel >> 8) & 255);
    }

    uint8_t b() const {
        return static_cast<uint8_t>(m_pixel & 255);
    }


private:
    uint32_t m_pixel;
};
