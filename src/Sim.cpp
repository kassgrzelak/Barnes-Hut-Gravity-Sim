#include "Sim.hpp"
#include <execution>
#include <atomic>

Sim::Sim(std::vector<Body> bodies)
	: bodies(bodies), tree(bodies)
{
	for (Body& body : bodies)
		body.initialize(tree);
}

static inline bool inBounds(const Vec2 p)
{
	return p.x >= minCoordsWithMargin.x && p.x < maxCoordsWithMargin.x &&
		   p.y >= minCoordsWithMargin.y && p.y < maxCoordsWithMargin.y;
}

void Sim::updateDraw()
{
	static std::vector<Body> retainedBodies;
	retainedBodies.resize(bodies.size());

	std::atomic<uint32_t> nextIndex{ 0 };

	std::for_each(std::execution::par, bodies.begin(), bodies.end(),
		[&](Body& body)
		{
			body.update(tree);

			if (inBounds(body.pos))
			{
				uint32_t index = nextIndex.fetch_add(1, std::memory_order_relaxed);
				retainedBodies[index] = body;
			}
		});

	retainedBodies.resize(nextIndex);
	std::swap(bodies, retainedBodies);

	for (const Body& body : bodies)
		body.draw();

#ifdef QUADTREE_VIS
	vizualiseQuadtree(tree.root, { minCoordsWithMargin.x - sizeWithMargin.x / 2.0f, minCoordsWithMargin.y - sizeWithMargin.y / 2.0f, sizeWithMargin.x * 2.0f, sizeWithMargin.y * 2.0f });
#endif

#ifdef SHOW_FPS
	DrawFPS(5, 5);
#endif

#ifdef SHOW_SIM_INFO
	showSimInfo();
#endif

	tree = Simtree(bodies);
}

void Sim::draw() const
{
	for ( const Body& body : bodies )
		body.draw();

#ifdef QUADTREE_VIS
	vizualiseQuadtree(tree.root, { minCoordsWithMargin.x - sizeWithMargin.x / 2.0f, minCoordsWithMargin.y - sizeWithMargin.y / 2.0f, sizeWithMargin.x * 2.0f, sizeWithMargin.y * 2.0f });
#endif

#ifdef SHOW_FPS
	DrawFPS(5, 5);
#endif

#ifdef SHOW_SIM_INFO
	showSimInfo();
#endif
}

std::vector<Body> Sim::bodyGrid(const Vec2 tl, const Vec2 dims, const int rows, const int cols,
	const Vec2 vel, const float mass, const float radius, const Color color)
{
	std::vector<Body> bodies = {};

	for (float y = tl.y; y < tl.y + dims.y; y += dims.y / rows)
		for (float x = tl.x; x < tl.x + dims.x; x += dims.x / cols)
			bodies.emplace_back(Vec2(x, y), vel, mass, radius, color, false);

	return bodies;
}

std::vector<Body> Sim::bodyCirclePack(const Vec2 center, const float circleRadius, const Vec2 vel, const float mass, float radius, const Color color, const float squishFactor)
{
	std::vector<Body> bodies = {};

	radius *= squishFactor;
	const float h = sqrt(3.0f) * radius;
	int i = 0;

	for (float y = center.y - circleRadius; y < center.y + circleRadius; y += h)
	{
		for (float x = center.x - circleRadius + (i % 2 == 0 ? 0.0f : radius); x < center.x + circleRadius; x += 2 * radius)
		{
			if ((center - Vec2(x, y)).sqrMag() > circleRadius * circleRadius)
				continue;

			bodies.emplace_back(Vec2(x, y), vel, mass, radius / squishFactor, color, false);
		}

		i++;
	}

	return bodies;
}

std::vector<Body> Sim::bodyGalaxy(const float outerRadius, const float innerRadius, const int rings, const float density,
	const float centerMass, const float mass, const float radius, const Color color)
{
	std::vector<Body> bodies = {};

	for (float dist = innerRadius; dist < outerRadius; dist += (outerRadius - innerRadius) / rings)
		for (float angle = 0.0f; angle < 2.0f * PI; angle += 1.0f / (dist * density))
			bodies.emplace_back(screenCenter + Vec2(dist, 0).rotate(angle), Vec2(0, sqrt(gravConst * centerMass / dist)).rotate(angle), mass, radius, color, false);

	return bodies;
}

#ifdef QUADTREE_VIS
void Sim::vizualiseQuadtree(const NodeIndex node, const Rectangle& rec) const
{
	DrawRectangleRec(rec, { 0, 255, 255, 3 });
	
	if (tree.nodeData[node].isLeaf)
		DrawRectangleLinesEx(rec, 1, RED);
	else
		DrawRectangleLinesEx(rec, 1, { 255, 255, 255, 50 });

	if (tree.nodes[node].children[0] != nullIndex)
		vizualiseQuadtree(tree.nodes[node].children[0], Rectangle{ rec.x, rec.y, rec.width / 2.0f, rec.height / 2.0f });
	if (tree.nodes[node].children[1] != nullIndex)
		vizualiseQuadtree(tree.nodes[node].children[1], Rectangle{ rec.x + rec.width / 2.0f, rec.y, rec.width / 2.0f, rec.height / 2.0f });
	if (tree.nodes[node].children[2] != nullIndex)
		vizualiseQuadtree(tree.nodes[node].children[2], Rectangle{ rec.x, rec.y + rec.height / 2.0f, rec.width / 2.0f, rec.height / 2.0f });
	if (tree.nodes[node].children[3] != nullIndex)
		vizualiseQuadtree(tree.nodes[node].children[3], Rectangle{ rec.x + rec.width / 2.0f, rec.y + rec.height / 2.0f, rec.width / 2.0f, rec.height / 2.0f });
}
#endif

void Sim::showSimInfo() const
{
	DrawText(("N = " + std::to_string(bodies.size())).c_str(), 5, 25, 20, WHITE);
	DrawText(("THETA = " + std::to_string(accuracy)).c_str(), 5, 45, 10, WHITE);
	DrawText(("TARGET FPS = " + std::to_string(targetFPS)).c_str(), 5, 55, 10, WHITE);
	DrawText(("TIMESCALE = " + std::to_string(timeScale)).c_str(), 5, 65, 10, WHITE);
	DrawText(("DT = " + std::to_string(dt)).c_str(), 5, 75, 10, WHITE);
}
