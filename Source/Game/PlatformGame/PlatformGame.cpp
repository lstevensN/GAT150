#include "PlatformGame.h"
#include "Player.h"
#include "Enemy.h"

#define EVENT_SUBSCRIBE(id, function)	kiko::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1));
#define EVENT_UNSUBSCRIBE(id)			kiko::EventManager::Instance().Unsubscribe(id, this);
#define EVENT_DISPATCH(id, value)       kiko::EventManager::Instance().DispatchEvent(id, value);

bool PlatformGame::Initialize()
{
	// Create the scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("scenes/title.json");
	m_scene->Initialize();

	// add events
	EVENT_SUBSCRIBE("OnAddPoints", PlatformGame::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", PlatformGame::OnPlayerDead);
	EVENT_SUBSCRIBE("EnemyDead", PlatformGame::OnEnemyDead);

	// load audio
	kiko::g_audioSystem.AddSound("jump", "audio/cartoon-jump.mp3");
	kiko::g_audioSystem.AddSound("coin_collect", "audio/coin-collect.mp3");
	kiko::g_audioSystem.AddSound("stun", "audio/stunned.mp3");
	kiko::g_audioSystem.AddSound("bgm", "audio/egg_reverie.mp3");

	return true;
}

void PlatformGame::Shutdown()
{

}

void PlatformGame::Update(float dt)
{
	switch (m_state)
	{
	case eState::Title:

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_RETURN))
		{
			kiko::g_audioSystem.PlayOneShot("bgm", true);

			m_state = eState::StartGame;
		}
	
		break;

	case eState::StartGame:
		m_scene->RemoveAll();

		m_state = eState::StartLevel;
		break;

	case eState::StartLevel:
		m_scene->Load("scenes/scene.json");
		m_scene->Load("scenes/tilemap2.json");
		m_scene->Initialize();

		// spawn first enemies
		for (int i = 0; i < 6; i++)
		{
			auto enemy = INSTANTIATE(Enemy, "Enemy");
			enemy->Initialize();
			enemy->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ kiko::random(kiko::g_renderer.GetWidth()), -100 });
			m_scene->Add(std::move(enemy));
		}
		{
			auto enemy = INSTANTIATE(Enemy, "Enemy");
			enemy->transform.scale = 1.0f;
			enemy->SetSpeed(10.0f);
			enemy->Initialize();
			enemy->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ -kiko::random(200), 350 });
			m_scene->Add(std::move(enemy));

			enemy = INSTANTIATE(Enemy, "Enemy");
			enemy->transform.scale = 1.0f;
			enemy->SetSpeed(10.0f);
			enemy->Initialize();
			enemy->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ kiko::random(200) + 800, 350 });
			m_scene->Add(std::move(enemy));
		}

		// place coins
		{
			auto coin = INSTANTIATE(Actor, "Coin");
			coin->Initialize();
			coin->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ 100, 480 });
			m_scene->Add(std::move(coin));

			coin = INSTANTIATE(Actor, "Coin");
			coin->Initialize();
			coin->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ 775, 420 });
			m_scene->Add(std::move(coin));

			coin = INSTANTIATE(Actor, "Coin");
			coin->Initialize();
			coin->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ 520, 330 });
			m_scene->Add(std::move(coin));

			coin = INSTANTIATE(Actor, "Coin");
			coin->Initialize();
			coin->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ 271, 330 });
			m_scene->Add(std::move(coin));

			coin = INSTANTIATE(Actor, "Coin");
			coin->Initialize();
			coin->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ 53, 203 });
			m_scene->Add(std::move(coin));

			coin = INSTANTIATE(Actor, "Coin");
			coin->Initialize();
			coin->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ 400, 75 });
			m_scene->Add(std::move(coin));
		}

		m_state = eState::Game;
		break;

	case eState::Game:
	{
		auto cursor = m_scene->GetActorByName<kiko::Actor>("Cursor");
		cursor->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition(kiko::g_inputSystem.GetMousePosition());
		cursor->GetComponent<kiko::SpriteAnimRenderComponent>()->SetSequence("cursor", true);

		if (kiko::g_inputSystem.GetMouseButtonDown(0))
		{
			cursor->GetComponent<kiko::SpriteAnimRenderComponent>()->SetSequence("clicked", false);
			EVENT_DISPATCH("MouseClicked", kiko::g_inputSystem.GetMousePosition());
		}
		else if (kiko::g_inputSystem.GetMouseButtonDown(2))
		{
			cursor->GetComponent<kiko::SpriteAnimRenderComponent>()->SetSequence("right_clicked", false);
			EVENT_DISPATCH("RMouseClicked", kiko::g_inputSystem.GetMousePosition());
		}

		// I am going insane!!!! I love it!
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

	m_scene->Update(1.0f/ 60);
}

void PlatformGame::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);

	kiko::g_particleSystem.Draw(renderer);
}

void PlatformGame::OnAddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}

void PlatformGame::OnPlayerDead(const kiko::Event& event)
{
	m_lives--;
	m_state = eState::PlayerDeadStart;
}

void PlatformGame::OnEnemyDead(const kiko::Event& event)
{
	auto enemy = INSTANTIATE(Enemy, "Enemy");
	enemy->Initialize();
	enemy->GetComponent<kiko::Box2DPhysicsComponent>()->SetPosition({ kiko::random(kiko::g_renderer.GetWidth()), -100 });
	m_scene->Add(std::move(enemy));
}
