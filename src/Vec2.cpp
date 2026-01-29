#include "Vec2.hpp"
#include "logging.hpp"
#include <cmath>

Vec2 Vec2::operator+(const Vec2 v) const
{
	return Vec2(x + v.x, y + v.y);
}

Vec2& Vec2::operator+=(const Vec2 v)
{
	this->x += v.x;
	this->y += v.y;

	return *this;
}

Vec2 Vec2::operator-(const Vec2 v) const
{
	return Vec2(x - v.x, y - v.y);
}

Vec2& Vec2::operator-=(const Vec2 v)
{
	this->x -= v.x;
	this->y -= v.y;

	return *this;
}

Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}

Vec2 Vec2::operator*(const float s) const
{
	return Vec2(s * x, s * y);
}

Vec2& Vec2::operator*=(const float s)
{
	this->x *= s;
	this->y *= s;

	return *this;
}

Vec2 Vec2::operator/(const float s) const
{
	return Vec2(x / s, y / s);
}

Vec2& Vec2::operator/=(const float s)
{
	this->x /= s;
	this->y /= s;

	return *this;
}

bool Vec2::operator==(const Vec2 v) const
{
	return x == v.x && y == v.y;
}

bool Vec2::operator!=(const Vec2 v) const
{
	return x != v.x || y != v.y;
}

float Vec2::mag() const
{
	return sqrt(sqrMag());
}

float Vec2::sqrMag() const
{
	return x * x + y * y;
}

Vec2 Vec2::norm() const
{
	float magnitude = mag();

	return *this / magnitude;
}

float Vec2::dot(Vec2 v) const
{
	return x * v.x + y * v.y;
}

float Vec2::angle() const
{
	return atan2(y, x);
}

Vec2 Vec2::lerp(Vec2 v, float t) const
{
	return *this * (1 - t) + v * t;
}

Vec2 Vec2::rotate(float rad) const
{
	const float s = sin(rad);
	const float c = cos(rad);

	return Vec2(x * c - y * s, x * s + y * c);
}

std::string Vec2::asStr() const
{
	std::string xString = std::to_string(x);
	std::string yString = std::to_string(y);
	return "[" + xString + " " + yString + "]";
}

std::ostream& operator<<(std::ostream& os, const Vec2 v)
{
	os << "[" << v.x << " " << v.y << "]";
	return os;
}
