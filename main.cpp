#include "include/common.hpp"
#include "include/Sim.hpp"

static bool paused = false;

#ifdef FPS_DIAGNOSTIC
static float frameTimes[numFrames];
static int frameCounter = 0;
#endif

int main()
{
	InitWindow(static_cast<int>(screenDims.x), static_cast<int>(screenDims.y), "Sample Window");

	circleTex = LoadTexture("assets/circle.png");

	SetTargetFPS(targetFPS);

	//std::vector<Body> bodies = Sim::bodyGrid({ 170, 465 }, { 30, 70 }, 100, 100, { 0, 160 }, 10.0f, 1.0f, WHITE);
	//std::vector<Body> moreBodies = Sim::bodyGrid({ 800, 465 }, { 30, 70 }, 100, 100, { 0, -160 }, 10.0f, 1.0f, WHITE);
	//bodies.insert(bodies.end(), moreBodies.begin(), moreBodies.end());
	

	// Ball
	//std::vector<Body> bodies = Sim::bodyCirclePack(screenCenter, 100, { 0, 0 }, 1.0f, 1.0f, WHITE, 1.0f);

	// Galaxy
	std::vector<Body> bodies = Sim::bodyGalaxy(500.0f, 200.0f, 400, 0.1f, 1e7f, 10.0f, 1.0f, WHITE);
	bodies.emplace_back(screenCenter, Vec2::zero(), 1e7f, 10.0f, Color{0, 255, 255, 255}, false);

	// Binary galaxy
	//std::vector<Body> bodies = Sim::bodyGalaxy(500.0f, 200.0f, 200, 0.1f, 2e7f, 10.0f, 1.0f, WHITE);
	//bodies.emplace_back(screenCenter - Vec2(50, 0), Vec2(0, -210), 1e7f, 10.0f, Color{ 0, 255, 255, 255 }, false);
	//bodies.emplace_back(screenCenter + Vec2(50, 0), Vec2(0, 210), 1e7f, 10.0f, Color{ 0, 255, 255, 255 }, false);

	Sim sim(bodies);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

#ifdef FPS_DIAGNOSTIC
		frameTimes[frameCounter++] = GetFrameTime();

		if (frameCounter == numFrames)
			break;
#endif

		if (IsKeyPressed(KEY_SPACE))
			paused = !paused;

		if (paused)
			sim.draw();
		else
			sim.updateDraw();
		
		EndDrawing();
	}

#ifdef FPS_DIAGNOSTIC
	float frameTimeSum = 0.0f;

	for (float frameTime : frameTimes)
		frameTimeSum += frameTime;

	const float avgFrameTime = frameTimeSum / numFrames;
	const float avgFPS = 1.0f / avgFrameTime;

	logMsg("AVG FPS", avgFPS);
#endif

	CloseWindow();
	return 0;
}