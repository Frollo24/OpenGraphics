#pragma once

namespace OpenGraphics
{
    inline float Vector4D::Length() const
    {
        return sqrt(SqrLength());
    }

    inline float Vector4D::SqrLength() const
    {
        return x * x + y * y + z * z + w * w;
    }

    inline void Vector4D::Normalize()
    {
        const float norm = 1 / Length();
        this->x *= norm;
        this->y *= norm;
        this->z *= norm;
        this->w *= norm;
    }

    inline Vector4D Vector4D::Normalized() const
    {
        const float norm = 1 / Length();
        return (*this) * norm;
    }

    inline void Vector4D::Set(float newX, float newY, float newZ, float newW)
    {
        this->x = newX;
        this->y = newY;
        this->z = newZ;
        this->w = newW;
    }

    inline Vector4D Vector4D::Lerp(const Vector4D& a, const Vector4D& b, const float& t)
    {
        return a * (1 - t) + b * t;
    }
}
