#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

namespace OpenGraphics
{
    struct Color
    {
        float r, g, b, a;

        Color() : r(0), g(0), b(0), a(0) {}
        Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

        static const Color& red;
        static const Color& green;
        static const Color& yellow;
        static const Color& blue;
        static const Color& magenta;
        static const Color& cyan;

        static const Color& white;
        static const Color& black;
        static const Color& clear;

        inline bool operator==(const Color& other) const
        {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
        [[nodiscard]] inline glm::vec4 toGlmVector() const
        {
            return glm::vec4(r, g, b, a);
        }
    };

    inline const Color& Color::red = Color(1.0f, 0.0f, 0.0f);
    inline const Color& Color::green = Color(0.0f, 1.0f, 0.0f);
    inline const Color& Color::yellow = Color(1.0f, 1.0f, 0.0f);
    inline const Color& Color::blue = Color(0.0f, 0.0f, 1.0f);
    inline const Color& Color::magenta = Color(1.0f, 0.0f, 1.0f);
    inline const Color& Color::cyan = Color(0.0f, 1.0f, 1.0f);

    inline const Color& Color::white = Color(1.0f, 1.0f, 1.0f, 1.0f);
    inline const Color& Color::black = Color(0.0f, 0.0f, 0.0f, 1.0f);
    inline const Color& Color::clear = Color(0.0f, 0.0f, 0.0f, 0.0f);
}
