#pragma once

#include "Mathf.h"

#include <glm/glm.hpp>

namespace OpenGraphics
{
    struct Vector2D
    {
		float x, y;

        Vector2D() : x(0), y(0) {}
    	Vector2D(float x, float y) : x(x), y(y) {}

    	static const Vector2D& zero;
    	static const Vector2D& right;
    	static const Vector2D& left;
    	static const Vector2D& up;
    	static const Vector2D& down;
    	static const Vector2D& one;
    	static const Vector2D& positiveInfinity;
    	static const Vector2D& negativeInfinity;

    	[[nodiscard]] float Length() const;
    	[[nodiscard]] float SqrLength() const;
    	void Normalize();
    	[[nodiscard]] Vector2D Normalized() const;
    	void Set(float newX, float newY);

    	static Vector2D Lerp(const Vector2D& a, const Vector2D& b, const float& t);

        [[nodiscard]] inline Vector2D operator-() const
    	{
			return Vector2D(-x, -y);
		}

		[[nodiscard]] inline Vector2D Add(const Vector2D& other) const
    	{
			return Vector2D(x + other.x, y + other.y);
		}
		inline Vector2D operator+(const Vector2D& other) const
    	{
			return Add(other);
		}

		[[nodiscard]] inline Vector2D Subtract(const Vector2D& other) const
    	{
			return Vector2D(x - other.x, y - other.y);
		}
		inline Vector2D operator-(const Vector2D& other) const
    	{
			return Subtract(other);
		}

		[[nodiscard]] inline Vector2D Multiply(const float& amount) const
    	{
			return Vector2D(x * amount, y * amount);
		}
		inline Vector2D operator*(const float& amount) const
    	{
			return Multiply(amount);
		}

		[[nodiscard]] inline bool Equals(const Vector2D& other) const
    	{
			float xAbs = abs(x - other.x);
			float yAbs = abs(y - other.y);
        	return xAbs < Mathf::Epsilon && yAbs < Mathf::Epsilon;
		}
		[[nodiscard]] inline bool ExactlyEquals(const Vector2D& other) const
    	{
			return x == other.x && y == other.y;
		}
		inline bool operator==(const Vector2D& other) const
    	{
			return Equals(other);
		}
		inline bool operator!=(const Vector2D& other) const
    	{
			return !Equals(other);
		}

		[[nodiscard]] inline glm::vec2 toGlmVector() const
    	{
	        return glm::vec2(x, y);
        }
    };

	inline const Vector2D& Vector2D::zero = Vector2D(0, 0);
	inline const Vector2D& Vector2D::right = Vector2D(1, 0);
	inline const Vector2D& Vector2D::left = Vector2D(-1, 0);
	inline const Vector2D& Vector2D::up = Vector2D(0, 1);
	inline const Vector2D& Vector2D::down = Vector2D(0, -1);
	inline const Vector2D& Vector2D::one = Vector2D(1, 1);
	inline const Vector2D& Vector2D::positiveInfinity = Vector2D(Mathf::PositiveInfinity, Mathf::PositiveInfinity);
	inline const Vector2D& Vector2D::negativeInfinity = Vector2D(Mathf::NegativeInfinity, Mathf::NegativeInfinity);
}

#include "Vector2D.inl"
