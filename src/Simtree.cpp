#include "Simtree.hpp"
#include <cmath>

Simtree::Simtree(std::vector<Body>& bodies)
{
	const size_t reserveSize = 2 * bodies.size();

	nodes.resize(reserveSize);
	nodeData.resize(reserveSize);

	root = buildTree(bodies.begin(), bodies.end(), sizeWithMargin, screenCenter);
}

inline static Vec2 gravAccel(const Vec2 pos, const Vec2 srcPos, const float srcMass)
{
	const Vec2 rel = srcPos - pos;
	const float sqrDist = rel.sqrMag();

	if (sqrDist == 0.0f)
		return Vec2::zero();

	const Vec2 dir = rel / sqrt(sqrDist);

#ifdef SMOOTH_GRAV
	return dir * (gravConst * srcMass / (gravSmoothness + sqrDist));
#else
	return dir * (gravConst * srcMass / sqrDist);
#endif
}

// Doesn't check for zero dist as that's already done in the loop.
inline static Vec2 gravAccel(const Vec2 pos, const Vec2 rel, const float sqrDist, const float srcMass)
{
	const Vec2 dir = rel / sqrt(sqrDist);

#ifdef SMOOTH_GRAV
	return dir * (gravConst * srcMass / (gravSmoothness + sqrDist));
#else
	return dir * (gravConst * srcMass / sqrDist);
#endif
}

NodeIndex Simtree::buildTree(BodyIt begin, BodyIt end, const Vec2 dims, const Vec2 center)
{
	// If range is empty, no node needs to be created.
	if (begin == end)
		return nullIndex;

	// Otherwise, node does need to be created.
	// All attributes are defaulted for now.
	NodeIndex result = nodeCounter++;
	NodeData& data = nodeData[result];

	if (begin + 1 == end)  // Leaf node.
	{
		data.com.set(*begin);
		data.isLeaf = true;
		return result;
	}

	data.com.add(begin, end);

#ifndef UNSAFE_TREE_BUILD
	constexpr auto samePos = [](const Body& a, const Body& b) { return a.pos == b.pos; };

	// If all bodies present are in the same position, sod it all and just put them in one node.
	if (std::equal(begin + 1, end, begin, samePos))
	{
		data.com.add(begin, end);
		data.isLeaf = true;
		return result;
	}
#endif

	// Otherwise, split the node into children.
	auto top = [center](const Body& body) { return body.pos.y < center.y; };
	auto left = [center](const Body& body) { return body.pos.x < center.x; };

	BodyIt ySplit = std::partition(begin, end, top);
	BodyIt xSplitUpper = std::partition(begin, ySplit, left);
	BodyIt xSplitLower = std::partition(ySplit, end, left);

	Node& thisNode = nodes[result];
	const Vec2 halfDims = dims / 2.0f;

	thisNode.children[0] = buildTree(begin, xSplitUpper, halfDims, { center.x - halfDims.x, center.y - halfDims.y });
	thisNode.children[1] = buildTree(xSplitUpper, ySplit, halfDims, { center.x + halfDims.x, center.y - halfDims.y });
	thisNode.children[2] = buildTree(ySplit, xSplitLower, halfDims, { center.x - halfDims.x, center.y + halfDims.y });
	thisNode.children[3] = buildTree(xSplitLower, end, halfDims, { center.x + halfDims.x, center.y + halfDims.y });

	return result;
}

Vec2 Simtree::_accelAt(const Vec2 pos, const NodeIndex node, const int depth) const
{
	const Node& thisNode = nodes[node];
	const CoM& thisCom = nodeData[node].com;

	if (nodeData[node].isLeaf)
		return gravAccel(pos, thisCom.pos, thisCom.mass);

	const Vec2 rel = thisCom.pos - pos;
	const float sqrDist = rel.sqrMag();

	if (sqrDist == 0.0f)
		return Vec2::zero();

	const float sqrQuotient = precomputedSqrWidths[depth] / sqrDist;

	if (sqrQuotient < sqrAccuracy)
		return gravAccel(pos, rel, sqrDist, thisCom.mass);
	else
	{
		Vec2 accelSum = Vec2::zero();

		for (const NodeIndex child : thisNode.children)
			if (child != nullIndex)
				accelSum += _accelAt(pos, child, depth + 1);

		return accelSum;
	}
}