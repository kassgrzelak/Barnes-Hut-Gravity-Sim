#pragma once

#include "common.hpp"
#include "CoM.hpp"

#include <algorithm>
#include <cstdint>

using NodeIndex = uint32_t;
static constexpr NodeIndex nullIndex = -1;

using BodyIt = std::vector<Body>::iterator;

struct Node
{
	std::array<NodeIndex, 4> children =
	{
		nullIndex, nullIndex,  // UL, UR,
		nullIndex, nullIndex   // BL, BR
	};
};

struct NodeData
{
	CoM com;
	uint8_t isLeaf;
};

class Simtree
{
public:
	NodeIndex root;
	std::vector<Node> nodes;
	std::vector<NodeData> nodeData;

	Simtree(std::vector<Body>& bodies);

	inline Vec2 accelAt(const Vec2 pos) const
	{
		return _accelAt(pos, root, 0);
	}

private:
	NodeIndex nodeCounter = 0;

	NodeIndex buildTree(BodyIt begin, BodyIt end, const Vec2 dims, const Vec2 center);
	Vec2 _accelAt(const Vec2 pos, const NodeIndex node, const int depth) const;
};