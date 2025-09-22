#pragma once

#include <limits>

namespace OpenGraphics
{
    class Mathf
    {
    public:
        static constexpr float Pi = 3.14159265358979323846f;
        static constexpr float OneOverPi = 0.318309886183790671538f;
        static constexpr float Epsilon = 1e-05f;
        static constexpr float PositiveInfinity = std::numeric_limits<float>::infinity();
        static constexpr float NegativeInfinity = -std::numeric_limits<float>::infinity();

        static float Lerp(float a, float b, float t);

        static int Pow(int base, int exponent);
        static float Pow(float base, int exponent);
        static float Pow(float base, float exponent);
    };
}

#include "Mathf.inl"
