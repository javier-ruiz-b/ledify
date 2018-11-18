#pragma once
#include <stdint.h>

class Color {
public:
    Color() = default;

    template<class T>
    Color(T r,
          T g,
          T b,
          T w) {
        m_pixel =  (static_cast<uint32_t>(w & 255) << 24)  |
                   (static_cast<uint32_t>(r & 255) << 16)  |
                   (static_cast<uint32_t>(g & 255) << 8)  |
                    static_cast<uint32_t>(b & 255);
    }

    uint32_t pixel() const {
        return m_pixel;
    }

private:

    uint32_t m_pixel;
};
