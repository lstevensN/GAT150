#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"

#include "PlatformGame.h"

#include <iostream>
#include <vector>
#include <thread>
#include <array>
#include <cassert>
#include <map>
#include <functional>

using vec2 = kiko::Vector2;

int main(int argc, char* argv[])
{
	//kiko::Factory::Instance().Register<kiko::SpriteComponent>("SpriteRenderComponent");

	INFO_LOG("Initialze engine...");

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets/platform");

	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	// create game
	std::unique_ptr<PlatformGame> game = std::make_unique<PlatformGame>();
	game->Initialize();

	std::vector<vec2> cursor{ {10, 10}, { -10, 10 }, { -10, -10 }, { 10, -10 }, { 10, 10 } };
	kiko::Model cursorModel{cursor};

	kiko::vec2 v{ 5, 5 };
	v.Normalize();

	kiko::Transform transform{ { 400, 300}, 0, 3 };

	kiko::vec2 position{ 400, 300 };
	float speed = 50;
	float turnRate = kiko::DegreesToRadians(180);

	// main game loop
	bool quit = false;
	while (!quit)
	{
		kiko::g_time.Tick();

		kiko::g_inputSystem.Update();
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		kiko::g_audioSystem.Update();
		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());

		// update game
		game->Update(kiko::g_time.GetDeltaTime());

		// draw game
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		game->Draw(kiko::g_renderer);

		kiko::g_particleSystem.Draw(kiko::g_renderer);
		kiko::g_renderer.EndFrame();
	}

	return 0;
}
