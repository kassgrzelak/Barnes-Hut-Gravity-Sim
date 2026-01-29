#pragma once

#define SHOW_FPS  // Display FPS in upper left corner.
#define SHOW_SIM_INFO 
//#define QUADTREE_VIS  // Visualize quadtree structure.
#define SMOOTH_GRAV  // Smooth gravity out so bodies very close together won't fling each other around so much.

//#define UNSAFE_TREE_BUILD  // Skip check for bodies in same position when building quadtree.
#define FORCE_REGIONS_SQUARE  // Force regions to be square.

//#define FPS_DIAGNOSTIC  // Runs for a set amount of frames (configurable by parameter below), computes average frametime while running and outputs average FPS at end.

#include "raylib.h"
#include "logging.hpp"
#include "Vec2.hpp"

#include <vector>
#include <array>
#include <cmath>

constexpr Vec2 screenDims = { 1600.0f, 900.0f };
constexpr Vec2 screenCenter = { screenDims.x / 2, screenDims.y / 2 };

// Grav sim parameters.
constexpr float accuracy = 0.5f;
constexpr float sqrAccuracy = accuracy * accuracy;
constexpr float gravConst = 1.0f;
constexpr float gravSmoothness = 1.0f;

// Tree size margin parameters.
constexpr Vec2 simTreeMargin = { 2000.0f, 2000.0f };  // Extra space to leave around the visible screen size when building tree so bodies aren't culled immediately upon leaving.
#ifdef FORCE_REGIONS_SQUARE
constexpr Vec2 correctedMargin = { screenDims.y > screenDims.x ? simTreeMargin.x + screenDims.y - screenDims.x : simTreeMargin.x,
                                   screenDims.x > screenDims.y ? simTreeMargin.y + screenDims.x - screenDims.y : simTreeMargin.y };
constexpr Vec2 sizeWithMargin = { screenDims.x + correctedMargin.x * 2.0f, screenDims.y + correctedMargin.y * 2.0f };
constexpr Vec2 minCoordsWithMargin = { -correctedMargin.x, -correctedMargin.y };
constexpr Vec2 maxCoordsWithMargin = { screenDims.x + correctedMargin.x, screenDims.y + correctedMargin.y };
#else
constexpr Vec2 sizeWithMargin = { screenDims.x + simTreeMargin.x * 2.0f, screenDims.y + simTreeMargin.y * 2.0f };
constexpr Vec2 minCoordsWithMargin = { -simTreeMargin.x, -simTreeMargin.y };
constexpr Vec2 maxCoordsWithMargin = { screenDims.x + simTreeMargin.x, screenDims.y + simTreeMargin.y };
#endif

constexpr int numPrecomputedSqrWidths = 50;

static constexpr float powf_(float base, int exp)
{
    return exp == 0 ? 1.0f : base * powf_(base, exp - 1);
}

static constexpr std::array<float, numPrecomputedSqrWidths> precomputeSqrWidths()
{
    std::array<float, numPrecomputedSqrWidths> sqrWidths = {};

    for (int i = 0; i < numPrecomputedSqrWidths; ++i)
    {
        const float val = sizeWithMargin.x / powf_(2.0f, i);
        sqrWidths[i] = val * val;
    }

    return sqrWidths;
}

constexpr std::array<float, numPrecomputedSqrWidths> precomputedSqrWidths = precomputeSqrWidths();

// Sim timestep parameters.
constexpr int targetFPS = 60;
constexpr float timeScale = 1.0f;
constexpr float dt = timeScale / targetFPS;
constexpr float dt05 = dt * 0.5f;  // dt * 0.5f.

// FPS Diagnostic parameters.
constexpr float timeToRun = 10.0f;
constexpr int numFrames = static_cast<int>(timeToRun / dt);

extern Texture2D circleTex;