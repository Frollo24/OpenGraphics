#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/optimum_pow.hpp>

namespace OpenGraphics
{
    inline float Mathf::Lerp(const float a, const float b, const float t)
    {
        return a * (1 - t) + b * t;
    }

    inline int Mathf::Pow(int base, int exponent)
    {
        int result = 1;
        if (exponent == 0) return 1;
        if (exponent == 1) return base;

        while (exponent > 0)
        {
            if (exponent & 1)
                result *= base;

            exponent /= 2;
            base *= base;
        }

        return result;
    }

    inline float Mathf::Pow(float base, int exponent)
    {
        float result = 1.0f;
        if (exponent == 0) return 1;
        if (exponent == 1) return base;

        while (exponent > 0)
        {
            if (exponent & 1)
                result *= base;

            exponent /= 2;
            base *= base;
        }

        return result;
    }

    inline float Mathf::Pow(const float base, const float exponent)
    {
        return powf(base, exponent);
    }
}
