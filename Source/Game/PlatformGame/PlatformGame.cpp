#include "PlatformGame.h"

#define EVENT_SUBSCRIBE(id, function)	kiko::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1));
#define EVENT_UNSUBSCRIBE(id)			kiko::EventManager::Instance().Unsubscribe(id, this);

bool PlatformGame::Initialize()
{
	// Create the scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("scenes/scene.json");
	m_scene->Initialize();

	// add events
	EVENT_SUBSCRIBE("OnAddPoints", PlatformGame::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", PlatformGame::OnPlayerDead);

	//kiko::EventManager::Instance().Subscribe("AddPoints", this, std::bind(&PlatformGame::OnAddPoints, this, std::placeholders::_1));
	//kiko::EventManager::Instance().Subscribe("OnPlayerDead", this, std::bind(&PlatformGame::OnPlayerDead, this, std::placeholders::_1));

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
		m_state = eState::StartLevel;
		break;

	case eState::StartLevel:
		m_scene->RemoveAll();

		m_state = eState::Game;
		break;

	case eState::Game:

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

	m_scene->Update(dt);
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
