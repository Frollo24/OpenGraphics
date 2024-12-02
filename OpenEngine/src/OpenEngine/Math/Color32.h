#pragma once

#include <stdint.h>

namespace OpenGraphics
{
    struct Color32
    {
        uint8_t r, g, b, a;

        Color32() : r(0), g(0), b(0), a(0) {}
        Color32(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
        Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    };
}
