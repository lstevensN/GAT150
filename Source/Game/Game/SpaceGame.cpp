#include "SpaceGame.h"

#define EVENT_SUBSCRIBE(id, function)	kiko::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1));
#define EVENT_UNSUBSCRIBE(id)			kiko::EventManager::Instance().Unsubscribe(id, this);

bool SpaceGame::Initialize()
{
	// Create the scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/SpaceScene.json");
	m_scene->Initialize();

	// Create font / text objects 
	m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "data-latin.ttf", 24));
	m_scoreText->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });

	m_gameOverText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "data-latin.ttf", 24));
	m_gameOverText->Create(kiko::g_renderer, "GAME OVER", kiko::Color{ 1, 1, 1, 1 });

	m_livesText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "data-unifon.ttf", 36));
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

	//kiko::g_audioSystem.PlayOneShot("bgm", true);

	// add events
	EVENT_SUBSCRIBE("OnAddPoints", SpaceGame::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", SpaceGame::OnPlayerDead);

	//kiko::EventManager::Instance().Subscribe("AddPoints", this, std::bind(&SpaceGame::OnAddPoints, this, std::placeholders::_1));
	//kiko::EventManager::Instance().Subscribe("OnPlayerDead", this, std::bind(&SpaceGame::OnPlayerDead, this, std::placeholders::_1));

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
			auto actor = m_scene->GetActorByName<kiko::Actor>("Background");
			if (actor) actor->active = false;

			actor = m_scene->GetActorByName<kiko::Actor>("Title");
			if (actor) actor->active = false;
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
			auto player = INSTANTIATE(Player, "Player");
			player->Initialize();
			player->m_game = this;
			player->m_damping = 0.9f;
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

			auto enemy = INSTANTIATE(Enemy, "Enemy");
			enemy->Initialize();
			enemy->transform = { { 0, kiko::random(200, 600) }, kiko::randomf(kiko::TwoPi), kiko::randomf(2.5f, 4.0f) };
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));
		}

		if (m_fastSpawnTimer >= m_spawnTime)
		{
			/*m_fastSpawnTimer = 0;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(kiko::randomf(100, 225), kiko::Pi, kiko::Transform{ { 0, kiko::random(200, 600) }, kiko::randomf(kiko::TwoPi), kiko::randomf(0.25f, 1.0f)});
			enemy->tag = "fastEnemy";
			enemy->m_game = this;

			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "leaCheese-remastered.png", kiko::g_renderer);
			enemy->AddComponent(std::move(component));

			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 10.0f;
			enemy->AddComponent(std::move(collisionComponent));

			enemy->Initialize();
			m_scene->Add(std::move(enemy));*/
		}

		// Spawn powerup
		m_powerUpTimer += dt;
		if (m_powerUpTimer >= m_spawnTime)
		{
			/*m_powerUpTimer = -40;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(50.0f, kiko::Pi, kiko::Transform{ { 0, kiko::random(200, 600) }, kiko::randomf(kiko::TwoPi), 5.0f });
			enemy->tag = "power_up";
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));*/
		}

		break;

	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		std::cout << m_lives << std::endl;
		if (m_lives <= 0) m_state = eState::GameOver;
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
	//if (m_state == eState::Title) m_scene->GetActorByName<kiko::Actor>("Title")->active = true;

	if (m_state == eState::GameOver) m_gameOverText->Draw(renderer, 350, 300);

	m_scoreText->Draw(renderer, 40, 40);

	if (m_lives >= 3) m_livesText->Draw(renderer, 650, 40);
	if (m_lives >= 2) m_livesText->Draw(renderer, 700, 40);
	if (m_lives >= 1) m_livesText->Draw(renderer, 750, 40);

	m_scene->Draw(renderer);

	kiko::g_particleSystem.Draw(renderer);
}

void SpaceGame::OnAddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}

void SpaceGame::OnPlayerDead(const kiko::Event& event)
{
	m_lives--;
	m_state = eState::PlayerDeadStart;
}
