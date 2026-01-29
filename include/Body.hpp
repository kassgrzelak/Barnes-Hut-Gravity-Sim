#pragma once

#include "common.hpp"
class Simtree;

struct Body
{
	Vec2 pos;
	Vec2 vel;
	Vec2 prevAccel = Vec2::zero();

	float mass;
	float radius;
	Color color;
	bool immovable;

	Body();
	Body(const Vec2 pos, const Vec2 vel, const float mass, const float radius, const Color color, const bool immovable);
	void initialize(const Simtree& sim);

	void update(const Simtree& sim);

	inline void draw() const
	{
		DrawTexturePro(
			circleTex,
			{ 0, 0, 100, 100 },
			{ pos.x, pos.y, radius2, radius2 },
			{ radius, radius },
			0.0f,
			color
		);
	}

private:
	float radius2;
};
