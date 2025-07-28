#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace OpenGraphics
{
    inline float Vector3D::Length() const
    {
        return sqrt(SqrLength());
    }

    inline float Vector3D::SqrLength() const
    {
        return x * x + y * y + z * z;
    }

    inline void Vector3D::Normalize()
    {
        const float norm = 1 / Length();
        this->x *= norm;
        this->y *= norm;
        this->z *= norm;
    }

    inline Vector3D Vector3D::Normalized() const
    {
        const float norm = 1 / Length();
        return (*this) * norm;
    }

    inline void Vector3D::Set(const float newX, const float newY, const float newZ)
    {
        this->x = newX;
        this->y = newY;
        this->z = newZ;
    }

    inline Vector3D Vector3D::RotateAroundAxis(const Vector3D& axis, const float angle)
    {
        const glm::vec3 result = glm::rotate(toGlmVector(), glm::radians(angle), axis.toGlmVector());

        this->x = result.x;
        this->y = result.y;
        this->z = result.z;

        return *this;
    }

    inline Vector3D Vector3D::Lerp(const Vector3D& a, const Vector3D& b, const float t)
    {
        return a * (1 - t) + b * t;
    }
}

inline std::ostream& operator<<(std::ostream& os, const OpenGraphics::Vector3D& vector) {
    return os << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}";
}
