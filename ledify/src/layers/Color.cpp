#include "Color.h"

uint8_t Color::add(uint8_t component1, uint8_t component2) {
    if ((component1 + component2) > 255) {
        return 255;
    }
    return component1 + component2;
}

Color::Color(const Color &color, float multiply) {
    m_rgbw = componentsToPixel(color.r() * multiply,
                               color.g() * multiply,
                               color.b() * multiply,
                               color.w() * multiply);
}

Color Color::add(Color &otherColor) {
    return Color(add(otherColor.r(), r()),
                 add(otherColor.g(), g()),
                 add(otherColor.b(), b()),
                 add(otherColor.w(), w()));
}

uint32_t Color::rgbw() const {
    return m_rgbw;
}

uint8_t Color::r() const {
    return static_cast<uint8_t>((m_rgbw >> 16) & 255);
}

uint8_t Color::g() const {
    return static_cast<uint8_t>((m_rgbw >> 8) & 255);
}

uint8_t Color::b() const {
    return static_cast<uint8_t>(m_rgbw & 255);
}

uint8_t Color::w() const {
    return static_cast<uint8_t>(m_rgbw >> 24);
}

