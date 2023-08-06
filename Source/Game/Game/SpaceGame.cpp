#include "SpaceGame.h"

bool SpaceGame::Initialize()
{
	// Create font / text objects 
	m_scoreText = std::make_unique<kiko::Text>(kiko::g_resources.Get<kiko::Font>("data-latin.ttf", 24));
	m_scoreText->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });

	m_gameOverText = std::make_unique<kiko::Text>(kiko::g_resources.Get<kiko::Font>("data-latin.ttf", 24));
	m_gameOverText->Create(kiko::g_renderer, "GAME OVER", kiko::Color{ 1, 1, 1, 1 });

	m_titleText = std::make_unique<kiko::Text>(kiko::g_resources.Get<kiko::Font>("data-latin.ttf", 36));
	m_titleText->Create(kiko::g_renderer, "SHIPPING FUEL", kiko::Color{ 1, 1, 1, 1 });

	m_livesText = std::make_unique<kiko::Text>(kiko::g_resources.Get<kiko::Font>("data-unifon.ttf", 36));
	m_livesText->Create(kiko::g_renderer, "A ", kiko::Color{ 1, 1, 1, 1 });

	// Load audio
	kiko::g_audioSystem.AddSound("bgm", "bgm.mp3");
	kiko::g_audioSystem.AddSound("weapon_small", "hit.wav");
	kiko::g_audioSystem.AddSound("weapon_big", "weapon_big.mp3");
	kiko::g_audioSystem.AddSound("weapon_fast", "weapon_fast.mp3");
	kiko::g_audioSystem.AddSound("weapon_strong", "weapon_strong.mp3");
	kiko::g_audioSystem.AddSound("player_destroy", "player_destroy.mp3");
	kiko::g_audioSystem.AddSound("enemy_destroy", "enemy_destroy.mp3");
	kiko::g_audioSystem.AddSound("strong_enemy_destroy", "strong_enemy_destroy.mp3");
	kiko::g_audioSystem.AddSound("power_up", "power_up.mp3");

	// Create the scene
	m_scene = std::make_unique<kiko::Scene>();
	kiko::g_audioSystem.PlayOneShot("bgm", true);

	return true;
}

void SpaceGame::Shutdown()
{

}

void SpaceGame::Update(float dt)
{
	switch (m_state)
	{
	case eState::Title:
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			m_state = eState::StartGame;
		}
		break;

	case eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;

	case eState::StartLevel:
		m_scene->RemoveAll();
		{
			// Create the player
			std::unique_ptr<Player> player = std::make_unique<Player>(15.0f, kiko::Pi, kiko::Transform{ { 400, 300 }, 0, 4.0f }, kiko::g_manager.Get("ship.txt"));
			player->m_tag = "Player";
			player->m_game = this;
			player->SetDamping(0.95f);

			// create components
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = kiko::g_resources.Get<kiko::Texture>("red_ball.png", kiko::g_renderer);
			player->AddComponent(std::move(component));

			m_scene->Add(std::move(player));
		}

		m_enemyCap = 0;
		m_state = eState::Game;
		break;

	case eState::Game:
		// Spawn enemies
		m_spawnTimer += dt;
		m_fastSpawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime)
		{
			m_spawnTimer = -3;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(kiko::randomf(30, 125), kiko::Pi, kiko::Transform{ { 0, kiko::random(200, 600) }, kiko::randomf(kiko::TwoPi), kiko::randomf(3.0f, 5.0f)}, kiko::g_manager.Get("enemy.txt"));
			enemy->m_tag = "Enemy";
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));

			// create components
			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = kiko::g_resources.Get<kiko::Texture>("leaCheese.png", kiko::g_renderer);
			enemy->AddComponent(std::move(component));
		}

		if (m_fastSpawnTimer >= m_spawnTime)
		{
			m_fastSpawnTimer = 0;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(kiko::randomf(100, 225), kiko::Pi, kiko::Transform{ { 0, kiko::random(200, 600) }, kiko::randomf(kiko::TwoPi), kiko::randomf(1.5f, 2.0f)}, kiko::g_manager.Get("enemy_fast.txt"));
			enemy->m_tag = "fastEnemy";
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));
		}

		// Spawn powerup
		m_powerUpTimer += dt;
		if (m_powerUpTimer >= m_spawnTime)
		{
			m_powerUpTimer = -40;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(50.0f, kiko::Pi, kiko::Transform{ { 0, kiko::random(200, 600) }, kiko::randomf(kiko::TwoPi), 5.0f }, kiko::g_manager.Get("power_up.txt"));
			enemy->m_tag = "power_up";
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));
		}

		break;

	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		std::cout << m_lives << std::endl;
		if (m_lives == 0) m_state = eState::GameOver;
		else m_state = eState::PlayerDead;
		break;

	case eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			m_state = eState::StartLevel;
		}
		break;

	case eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			m_state = eState::Title;
		}
		break;

	default:
		break;
	}

	m_scoreText->Create(kiko::g_renderer, std::to_string(m_score), { 1, 1, 1, 1 });
	m_scene->Update(dt);
	kiko::g_particleSystem.Update(dt);
}

void SpaceGame::Draw(kiko::Renderer& renderer)
{
	if (m_state == eState::Title) m_titleText->Draw(renderer, 400, 300);

	if (m_state == eState::GameOver) m_gameOverText->Draw(renderer, 350, 300);

	m_scoreText->Draw(renderer, 40, 40);

	if (m_lives >= 3) m_livesText->Draw(renderer, 650, 40);
	if (m_lives >= 2) m_livesText->Draw(renderer, 700, 40);
	if (m_lives >= 1) m_livesText->Draw(renderer, 750, 40);

	m_scene->Draw(renderer);

	kiko::g_particleSystem.Draw(renderer);
}
