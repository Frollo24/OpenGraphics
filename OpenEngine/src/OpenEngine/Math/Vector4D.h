#pragma once

#include "Mathf.h"
#include "Vector3D.h"

#include <glm/glm.hpp>

namespace OpenGraphics
{
    struct Vector4D
    {
    	float x, y, z, w;

        Vector4D() : x(0), y(0), z(0), w(0) {}
        Vector4D(const float x, const float y) : x(x), y(y), z(0), w(0) {}
        Vector4D(const float x, const float y, const float z) : x(x), y(y), z(z), w(0) {}
        Vector4D(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
    	Vector4D(const Vector3D& vector, const float w) : x(vector.x), y(vector.y), z(vector.z), w(w) {}

    	static const Vector4D& zero;
    	static const Vector4D& one;
    	static const Vector4D& positiveInfinity;
    	static const Vector4D& negativeInfinity;

    	[[nodiscard]] float Length() const;
    	[[nodiscard]] float SqrLength() const;
    	void Normalize();
    	[[nodiscard]] Vector4D Normalized() const;
    	void Set(float newX, float newY, float newZ, float newW);

    	static Vector4D Lerp(const Vector4D& a, const Vector4D& b, float t);

        [[nodiscard]] inline Vector4D operator-() const
    	{
			return Vector4D(-x, -y, -z, -w);
		}

		[[nodiscard]] inline Vector4D Add(const Vector4D& other) const
    	{
			return Vector4D(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		inline Vector4D operator+(const Vector4D& other) const
    	{
			return Add(other);
		}

		[[nodiscard]] inline Vector4D Subtract(const Vector4D& other) const
    	{
			return Vector4D(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		inline Vector4D operator-(const Vector4D& other) const
    	{
			return Subtract(other);
		}

		[[nodiscard]] inline Vector4D Multiply(const float& amount) const
    	{
			return Vector4D(x * amount, y * amount, z * amount, w * amount);
		}
		inline Vector4D operator*(const float& amount) const
    	{
			return Multiply(amount);
		}

		[[nodiscard]] inline bool Equals(const Vector4D& other) const
    	{
			const float xAbs = abs(x - other.x);
			const float yAbs = abs(y - other.y);
			const float zAbs = abs(z - other.z);
        	const float wAbs = abs(w - other.w);
        	return xAbs < Mathf::Epsilon && yAbs < Mathf::Epsilon && zAbs < Mathf::Epsilon && wAbs < Mathf::Epsilon;
		}
		[[nodiscard]] inline bool ExactlyEquals(const Vector4D& other) const
    	{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}
		inline bool operator==(const Vector4D& other) const
    	{
			return Equals(other);
		}
		inline bool operator!=(const Vector4D& other) const
    	{
			return !Equals(other);
		}

		[[nodiscard]] inline glm::vec4 toGlmVector() const
    	{
	        return glm::vec4(x, y, z, w);
        }
    };

	inline const Vector4D& Vector4D::zero = Vector4D(0, 0, 0, 0);
	inline const Vector4D& Vector4D::one = Vector4D(1, 1, 1, 1);
	inline const Vector4D& Vector4D::positiveInfinity = Vector4D(Mathf::PositiveInfinity, Mathf::PositiveInfinity, Mathf::PositiveInfinity, Mathf::PositiveInfinity);
	inline const Vector4D& Vector4D::negativeInfinity = Vector4D(Mathf::NegativeInfinity, Mathf::NegativeInfinity, Mathf::NegativeInfinity, Mathf::NegativeInfinity);
}

#include "Vector4D.inl"
