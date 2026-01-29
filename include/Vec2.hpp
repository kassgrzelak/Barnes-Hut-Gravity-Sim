#pragma once

#include <string>

#include "raylib.h"

#define ZERO_VEC_NORMALIZATION 251

class Vec2
{
public:
	float x;
	float y;

	constexpr Vec2(): x(0.0f), y(0.0f) { };
	constexpr Vec2(float x, float y) : x(x), y(y) { };
	constexpr Vec2(Vector2 v) : x(v.x), y(v.y) { };

	Vec2  operator+(const Vec2 v) const;
	Vec2& operator+=(const Vec2 v);
	Vec2  operator-(const Vec2 v) const;
	Vec2& operator-=(const Vec2 v);
	Vec2  operator-() const;
	Vec2  operator*(const float s) const;
	Vec2& operator*=(const float s);
	Vec2  operator/(const float s) const;
	Vec2& operator/=(const float s);
	bool  operator==(const Vec2 v) const;
	bool  operator!=(const Vec2 v) const;

	float mag() const;
	float sqrMag() const;
	Vec2  norm() const;
	float dot(Vec2 v) const;
	float angle() const;
	Vec2  lerp(Vec2 v, float t) const;
	Vec2  rotate(float rad) const;

	std::string asStr() const;
	friend std::ostream& operator<<(std::ostream& os, const Vec2 v);

	static inline Vec2 zero() { return Vec2(); };
	static inline Vec2 xUnit() { return Vec2(1.0f, 0.0f); };
	static inline Vec2 yUnit() { return Vec2{ 0.0f, 1.0f }; };

	inline operator Vector2() const
	{
		return Vector2{ x, y };
	}
};