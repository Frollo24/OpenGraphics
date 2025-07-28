#pragma once

#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/matrix_transform.hpp>

namespace OpenGraphics
{
    inline float Matrix4x4::Determinant() const
    {
        return glm::determinant(this->toGlmMatrix());
    }

    inline Matrix4x4& Matrix4x4::Inverse()
    {
        const glm::mat4 inverse = glm::inverse(this->toGlmMatrix());
        columns[0] = Vector4D(inverse[0].x, inverse[0].y, inverse[0].z, inverse[0].w);
        columns[1] = Vector4D(inverse[1].x, inverse[1].y, inverse[1].z, inverse[1].w);
        columns[2] = Vector4D(inverse[2].x, inverse[2].y, inverse[2].z, inverse[2].w);
        columns[3] = Vector4D(inverse[3].x, inverse[3].y, inverse[3].z, inverse[3].w);
        return *this;
    }

    inline Matrix4x4& Matrix4x4::Transpose()
    {
        const glm::mat4 transpose = glm::transpose(this->toGlmMatrix());
        columns[0] = Vector4D(transpose[0].x, transpose[0].y, transpose[0].z, transpose[0].w);
        columns[1] = Vector4D(transpose[1].x, transpose[1].y, transpose[1].z, transpose[1].w);
        columns[2] = Vector4D(transpose[2].x, transpose[2].y, transpose[2].z, transpose[2].w);
        columns[3] = Vector4D(transpose[3].x, transpose[3].y, transpose[3].z, transpose[3].w);
        return *this;
    }

    inline Matrix4x4& Matrix4x4::Translate(const Vector3D& translation)
    {
        const glm::mat4 translate = glm::translate(this->toGlmMatrix(), translation.toGlmVector());
        columns[0] = Vector4D(translate[0].x, translate[0].y, translate[0].z, translate[0].w);
        columns[1] = Vector4D(translate[1].x, translate[1].y, translate[1].z, translate[1].w);
        columns[2] = Vector4D(translate[2].x, translate[2].y, translate[2].z, translate[2].w);
        columns[3] = Vector4D(translate[3].x, translate[3].y, translate[3].z, translate[3].w);
        return *this;
    }

    inline Matrix4x4& Matrix4x4::Rotate(const Vector3D& axis, const float angle)
    {
        const glm::mat4 rotate = glm::rotate(this->toGlmMatrix(), glm::radians(angle), axis.toGlmVector());
        columns[0] = Vector4D(rotate[0].x, rotate[0].y, rotate[0].z, rotate[0].w);
        columns[1] = Vector4D(rotate[1].x, rotate[1].y, rotate[1].z, rotate[1].w);
        columns[2] = Vector4D(rotate[2].x, rotate[2].y, rotate[2].z, rotate[2].w);
        columns[3] = Vector4D(rotate[3].x, rotate[3].y, rotate[3].z, rotate[3].w);
        return *this;
    }

    inline Matrix4x4& Matrix4x4::Scale(const Vector3D& scale)
    {
        const glm::mat4 scaled = glm::scale(this->toGlmMatrix(), scale.toGlmVector());
        columns[0] = Vector4D(scaled[0].x, scaled[0].y, scaled[0].z, scaled[0].w);
        columns[1] = Vector4D(scaled[1].x, scaled[1].y, scaled[1].z, scaled[1].w);
        columns[2] = Vector4D(scaled[2].x, scaled[2].y, scaled[2].z, scaled[2].w);
        columns[3] = Vector4D(scaled[3].x, scaled[3].y, scaled[3].z, scaled[3].w);
        return *this;
    }

    inline Matrix4x4 Matrix4x4::LookAt(const Vector3D& from, const Vector3D& to, const Vector3D& up)
    {
        const glm::mat4 lookAt = glm::lookAt(from.toGlmVector(), to.toGlmVector(), up.toGlmVector());
        return Matrix4x4(
            Vector4D(lookAt[0].x, lookAt[0].y, lookAt[0].z, lookAt[0].w),
            Vector4D(lookAt[1].x, lookAt[1].y, lookAt[1].z, lookAt[1].w),
            Vector4D(lookAt[2].x, lookAt[2].y, lookAt[2].z, lookAt[2].w),
            Vector4D(lookAt[3].x, lookAt[3].y, lookAt[3].z, lookAt[3].w)
        );
    }

    inline Matrix4x4 Matrix4x4::Ortho(const float left, const float right, const float bottom, const float top,
        const float zNear, const float zFar)
    {
        const glm::mat4 ortho = glm::ortho(left, right, bottom, top, zNear, zFar);
        return Matrix4x4(
            Vector4D(ortho[0].x, ortho[0].y, ortho[0].z, ortho[0].w),
            Vector4D(ortho[1].x, ortho[1].y, ortho[1].z, ortho[1].w),
            Vector4D(ortho[2].x, ortho[2].y, ortho[2].z, ortho[2].w),
            Vector4D(ortho[3].x, ortho[3].y, ortho[3].z, ortho[3].w)
        );
    }

    inline Matrix4x4 Matrix4x4::Perspective(const float fov, const float aspect, const float zNear, const float zFar)
    {
        const glm::mat4 perspective = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
        return Matrix4x4(
            Vector4D(perspective[0].x, perspective[0].y, perspective[0].z, perspective[0].w),
            Vector4D(perspective[1].x, perspective[1].y, perspective[1].z, perspective[1].w),
            Vector4D(perspective[2].x, perspective[2].y, perspective[2].z, perspective[2].w),
            Vector4D(perspective[3].x, perspective[3].y, perspective[3].z, perspective[3].w)
        );
    }

    inline Vector3D Matrix4x4::MultiplyVector(const Vector3D& vector) const
    {
        const glm::vec4 result = this->toGlmMatrix() * glm::vec4(vector.toGlmVector(), 0.0f);
        return Vector3D(result.x, result.y, result.z);
    }

    inline Vector3D Matrix4x4::MultiplyPoint(const Vector3D& point) const
    {
        const glm::vec4 result = this->toGlmMatrix() * glm::vec4(point.toGlmVector(), 1.0f);
        return Vector3D(result.x, result.y, result.z);
    }

}
