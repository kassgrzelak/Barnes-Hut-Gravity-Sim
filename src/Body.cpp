#include "Body.hpp"
#include "Simtree.hpp"

// Default constructor for vector resizing.
Body::Body() { }

Body::Body(const Vec2 pos, const Vec2 vel, const float mass, const float radius, const Color color, const bool immovable)
	: pos(pos), vel(vel), mass(mass), radius(radius), radius2(2 * radius), color(color), immovable(immovable) { }

void Body::initialize(const Simtree& tree)
{
	vel += tree.accelAt(pos) * dt05;
}

void Body::update(const Simtree& tree)
{
	if (!immovable) [[likely]]
	{
		// Looks like Euler but because of the initialization at the start this becomes Leapfrog.
		const Vec2 accel = tree.accelAt(pos);
		vel += accel * dt;
		pos += vel * dt;
	}
}
