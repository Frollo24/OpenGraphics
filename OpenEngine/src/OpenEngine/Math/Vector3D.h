#pragma once

#include "Mathf.h"

#include <glm/glm.hpp>

namespace OpenGraphics
{
    struct Vector3D
    {
		float x, y, z;

        Vector3D() : x(0), y(0), z(0) {}
    	Vector3D(float x, float y) : x(x), y(y), z(0) {}
        Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    	static const Vector3D& zero;
    	static const Vector3D& right;
    	static const Vector3D& left;
    	static const Vector3D& up;
    	static const Vector3D& down;
    	static const Vector3D& forward;
    	static const Vector3D& back;
    	static const Vector3D& one;
    	static const Vector3D& positiveInfinity;
    	static const Vector3D& negativeInfinity;

    	[[nodiscard]] float Length() const;
    	[[nodiscard]] float SqrLength() const;
    	void Normalize();
    	[[nodiscard]] Vector3D Normalized() const;
    	void Set(float newX, float newY, float newZ);
    	Vector3D RotateAroundAxis(const Vector3D& axis, const float& angle);

    	static Vector3D Lerp(const Vector3D& a, const Vector3D& b, const float& t);

        [[nodiscard]] inline Vector3D operator-() const
    	{
			return Vector3D(-x, -y, -z);
		}

		[[nodiscard]] inline Vector3D Add(const Vector3D& other) const
    	{
			return Vector3D(x + other.x, y + other.y, z + other.z);
		}
		inline Vector3D operator+(const Vector3D& other) const
    	{
			return Add(other);
		}

		[[nodiscard]] inline Vector3D Subtract(const Vector3D& other) const
    	{
			return Vector3D(x - other.x, y - other.y, z - other.z);
		}
		inline Vector3D operator-(const Vector3D& other) const
    	{
			return Subtract(other);
		}

		[[nodiscard]] inline Vector3D Multiply(const float& amount) const
    	{
			return Vector3D(x * amount, y * amount, z * amount);
		}
		inline Vector3D operator*(const float& amount) const
    	{
			return Multiply(amount);
		}

		[[nodiscard]] inline bool Equals(const Vector3D& other) const
    	{
			float xAbs = abs(x - other.x);
			float yAbs = abs(y - other.y);
			float zAbs = abs(z - other.z);
        	return xAbs < Mathf::Epsilon && yAbs < Mathf::Epsilon && zAbs < Mathf::Epsilon;
		}
		[[nodiscard]] inline bool ExactlyEquals(const Vector3D& other) const
    	{
			return x == other.x && y == other.y && z == other.z;
		}
		inline bool operator==(const Vector3D& other) const
    	{
			return Equals(other);
		}
		inline bool operator!=(const Vector3D& other) const
    	{
			return !Equals(other);
		}

		[[nodiscard]] inline glm::vec3 toGlmVector() const
    	{
	        return glm::vec3(x, y, z);
        }
    };

	inline const Vector3D& Vector3D::zero = Vector3D(0, 0, 0);
	inline const Vector3D& Vector3D::right = Vector3D(1, 0, 0);
	inline const Vector3D& Vector3D::left = Vector3D(-1, 0, 0);
	inline const Vector3D& Vector3D::up = Vector3D(0, 1, 0);
	inline const Vector3D& Vector3D::down = Vector3D(0, -1, 0);
	inline const Vector3D& Vector3D::forward = Vector3D(0, 0, 1);
	inline const Vector3D& Vector3D::back = Vector3D(0, 0, -1);
	inline const Vector3D& Vector3D::one = Vector3D(1, 1, 1);
	inline const Vector3D& Vector3D::positiveInfinity = Vector3D(Mathf::PositiveInfinity, Mathf::PositiveInfinity, Mathf::PositiveInfinity);
	inline const Vector3D& Vector3D::negativeInfinity = Vector3D(Mathf::NegativeInfinity, Mathf::NegativeInfinity, Mathf::NegativeInfinity);
}

#include "Vector3D.inl"
