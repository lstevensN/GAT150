#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Player.h"
#include "Enemy.h"

#include "SpaceGame.h"

#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <map>


using vec2 = kiko::Vector2;

class Star
{
public:
	Star(const vec2 pos, const vec2 vel) : 
		m_pos{ pos }, m_vel{ vel } {}

	void Update(int width, int height)
	{
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
		if (m_pos.x > width) m_pos.x = 0;
		if (m_pos.y > height) m_pos.y = 0;
	}

	void Draw(kiko::Renderer& renderer)
	{
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}

public:
	vec2 m_pos;
	vec2 m_vel;
};

int main(int argc, char* argv[])
{
	INFO_LOG("Initialze engine...");

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();

	std::unique_ptr<SpaceGame> game = std::make_unique<SpaceGame>();
	game->Initialize();

	// kiko::res_t<kiko::Texture> texture = kiko::g_resources.Get<kiko::Texture>("bagel.png", kiko::g_renderer);

	std::vector<vec2> cursor{ {10, 10}, { -10, 10 }, { -10, -10 }, { 10, -10 }, { 10, 10 } };
	kiko::Model cursorModel{cursor};

	kiko::vec2 v{ 5, 5 };
	v.Normalize();

	std::vector<Star> stars;
	for (int i = 0; i < 1000; i++)
	{
		vec2 pos(kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight()));
		vec2 vel(kiko::randomf(100, 200), 0.0f);

		stars.push_back(Star(pos, vel));
	}

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

		// update game
		game->Update(kiko::g_time.GetDeltaTime());

		// draw game
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		//text->Draw(kiko::g_renderer, 400, 300);

		for (auto& star : stars)
		{
			star.Update(kiko::g_renderer.GetWidth(), kiko::g_renderer.GetHeight());

			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			star.Draw(kiko::g_renderer);
		}

		game->Draw(kiko::g_renderer);
		//kiko::g_renderer.DrawTexture(texture.get(), 200.0f, 200.0f, 0.0f);

		kiko::g_renderer.EndFrame();
	}

	stars.clear();

	return 0;
}
