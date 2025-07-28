#pragma once

namespace OpenGraphics
{
    inline void FastRandom::Init()
    {
        srand(time(NULL));
    }

    inline void FastRandom::Seed(const uint32_t seed)
    {
        srand(seed);
    }

    inline int FastRandom::NextInt()
    {
        return rand();
    }

    inline Color FastRandom::NextColor()
    {
        return Color(NextFloat(), NextFloat(), NextFloat(), 1.0f);
    }

    inline float FastRandom::NextFloat()
    {
        return static_cast<float>(NextDouble());
    }

    inline double FastRandom::NextDouble()
    {
        return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    }


    inline UniformRandom::UniformRandom(const uint32_t seed)
        : m_RandomEngine(seed) {}

    inline void UniformRandom::Seed(const uint32_t seed)
    {
        m_RandomEngine.seed(seed);
    }

    inline int UniformRandom::NextInt()
    {
        thread_local std::uniform_int_distribution<int> distribution;
        return distribution(m_RandomEngine);
    }

    inline Color UniformRandom::NextColor()
    {
        return Color(NextFloat(), NextFloat(), NextFloat(), 1.0f);
    }

    inline float UniformRandom::NextFloat()
    {
        thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
        return distribution(m_RandomEngine);
    }

    inline double UniformRandom::NextDouble()
    {
        thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
        return distribution(m_RandomEngine);
    }

    inline int UniformRandom::Range(const int minInclusive, const int maxInclusive)
    {
        std::uniform_int_distribution<int> distribution(minInclusive, maxInclusive);
        return distribution(m_RandomEngine);
    }

    inline float UniformRandom::Range(const float minInclusive, const float maxInclusive)
    {
        std::uniform_real_distribution<float> distribution(minInclusive, maxInclusive);
        return distribution(m_RandomEngine);
    }
}