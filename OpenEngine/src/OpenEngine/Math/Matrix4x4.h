#pragma once

#include "Mathf.h"
#include "Vector4D.h"

#include <glm/glm.hpp>

namespace OpenGraphics
{
    struct Matrix4x4
    {
        Vector4D columns[4];

        Matrix4x4() : columns{Vector4D(), Vector4D(), Vector4D(), Vector4D()} {}
        Matrix4x4(float s)
            : columns{
                Vector4D(s, 0, 0, 0),
                Vector4D(0, s, 0, 0),
                Vector4D(0, 0, s, 0),
                Vector4D(0, 0, 0, s)
            } {}
        Matrix4x4(const Matrix4x4& other) : columns{other[0], other[1], other[2], other[3]} {}
        Matrix4x4(const Vector4D& v0, const Vector4D& v1, const Vector4D& v2, const Vector4D& v3)
            : columns{v0, v1, v2, v3} {}

        static const Matrix4x4& zero;
        static const Matrix4x4& identity;

        Matrix4x4& Inverse();
        Matrix4x4& Transpose();

        Matrix4x4& Translate(const Vector3D& translation);
        Matrix4x4& Rotate(const Vector3D& axis, float angle);
        Matrix4x4& Scale(const Vector3D& scale);

        static Matrix4x4 LookAt(const Vector3D& from, const Vector3D& to, const Vector3D& up);
        static Matrix4x4 Perspective(float fov, float aspect, float zNear, float zFar);

        [[nodiscard]] Vector3D MultiplyVector(const Vector3D& vector) const;
        [[nodiscard]] Vector3D MultiplyPoint(const Vector3D& point) const;

        [[nodiscard]] inline Matrix4x4 operator*(const Matrix4x4& other) const
        {
            glm::mat4 result = this->toGlmMatrix() * other.toGlmMatrix();
            return Matrix4x4(
                Vector4D(result[0].x, result[0].y, result[0].z, result[0].w),
                Vector4D(result[1].x, result[1].y, result[1].z, result[1].w),
                Vector4D(result[2].x, result[2].y, result[2].z, result[2].w),
                Vector4D(result[3].x, result[3].y, result[3].z, result[3].w)
            );
        }

        [[nodiscard]] inline Vector4D operator*(const Vector4D& other) const
        {
            glm::vec4 result = this->toGlmMatrix() * other.toGlmVector();
            return Vector4D(result.x, result.y, result.z, result.w);
        }

        [[nodiscard]] inline Vector4D operator[](const int i) const
        {
            assert(i < 4);
            return columns[i];
        }

        [[nodiscard]] inline glm::mat4 toGlmMatrix() const
        {
            return glm::mat4(
                columns[0].toGlmVector(),
                columns[1].toGlmVector(),
                columns[2].toGlmVector(),
                columns[3].toGlmVector()
            );
        }
    };

    inline const Matrix4x4& Matrix4x4::zero = Matrix4x4(0.0f);
    inline const Matrix4x4& Matrix4x4::identity = Matrix4x4(1.0f);
}

#include "Matrix4x4.inl"
