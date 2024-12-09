#pragma once

namespace OpenGraphics
{
    inline float Vector2D::Length() const
    {
        return sqrt(SqrLength());
    }

    inline float Vector2D::SqrLength() const
    {
        return x * x + y * y;
    }

    inline void Vector2D::Normalize()
    {
        const float norm = 1 / Length();
        this->x *= norm;
        this->y *= norm;
    }

    inline Vector2D Vector2D::Normalized() const
    {
        const float norm = 1 / Length();
        return (*this) * norm;
    }

    inline void Vector2D::Set(float newX, float newY)
    {
        this->x = newX;
        this->y = newY;
    }

    inline Vector2D Vector2D::Lerp(const Vector2D& a, const Vector2D& b, const float& t)
    {
        return a * (1 - t) + b * t;
    }
}

inline std::ostream& operator<<(std::ostream& os, const OpenGraphics::Vector2D& vector) {
    return os << "{" << vector.x << ", " << vector.y << "}";
}