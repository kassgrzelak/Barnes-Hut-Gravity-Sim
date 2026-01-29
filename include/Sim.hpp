#pragma once

#include "common.hpp"
#include "Simtree.hpp"

class Sim
{
public:
	std::vector<Body> bodies;

	Sim(const std::vector<Body> bodies);

	void updateDraw();
	void draw() const;

	static std::vector<Body> bodyGrid(const Vec2 tl, const Vec2 dims, const int rows, const int cols,
		const Vec2 vel, const float mass, const float radius, const Color color);
	static std::vector<Body> bodyCirclePack(const Vec2 center, const float circleRadius,
		const Vec2 vel, const float mass, float radius, const Color color, const float squishFactor = 1.0f);
	static std::vector<Body> bodyGalaxy(const float outerRadius, const float innerRadius, const int rings, const float density,
		const float centerMass, const float mass, const float radius, const Color color);

private:
	Simtree tree;

#ifdef QUADTREE_VIS
	void vizualiseQuadtree(const NodeIndex node, const Rectangle& rec) const;
#endif

#ifdef SHOW_SIM_INFO
	void showSimInfo() const;
#endif
};