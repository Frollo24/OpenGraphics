#pragma once

#include <glm/glm.hpp>

namespace OpenGraphics
{
    struct Color
    {
        float r, g, b, a;

        Color() : r(0), g(0), b(0), a(0) {}
        Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

        [[nodiscard]] inline glm::vec4 toGlmVector() const
        {
            return glm::vec4(r, g, b, a);
        }
    };
}
