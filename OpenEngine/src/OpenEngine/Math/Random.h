#pragma once

#include "Color.h"

#include <random>
#include <cstdlib>
#include <ctime>
#include <cstdint>

namespace OpenGraphics
{
    class FastRandom
    {
    public:
        static void Init();
        static void Seed(const uint32_t& seed);

        static int NextInt();
        static Color NextColor();
        static float NextFloat();
        static double NextDouble();
    };

    class UniformRandom
    {
    public:
        explicit UniformRandom(const uint32_t& seed = time(nullptr));
        void Seed(const uint32_t& seed);

        int NextInt();
        Color NextColor();
        float NextFloat();
        double NextDouble();

        int Range(int minInclusive, int maxInclusive);
        float Range(float minInclusive, float maxInclusive);

    private:
        std::mt19937 m_RandomEngine;
    };
}

#include "Random.inl"
