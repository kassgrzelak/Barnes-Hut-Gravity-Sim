#pragma once

#include "Body.hpp"

using ConstBodyIt = std::vector<Body>::const_iterator;

struct CoM
{
	Vec2 pos = Vec2::zero();
	float mass = 0.0f;

	void add(ConstBodyIt begin, ConstBodyIt end)
	{
		Vec2 momentSum = Vec2::zero();
		float massSum = 0.0f;

		for (ConstBodyIt it = begin; it != end; ++it)
		{
			const float mass = it->mass;
			momentSum += it->pos * mass;
			massSum += mass;
		}

		pos = momentSum / massSum;
		mass = massSum;
	}

	inline void set(const Body& body)
	{
		pos = body.pos;
		mass = body.mass;
	}
};