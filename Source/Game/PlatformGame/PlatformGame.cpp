#include "PlatformGame.h"

#define EVENT_SUBSCRIBE(id, function)	kiko::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1));
#define EVENT_UNSUBSCRIBE(id)			kiko::EventManager::Instance().Unsubscribe(id, this);
#define EVENT_DISPATCH(id, value)       kiko::EventManager::Instance().DispatchEvent(id, value);

bool PlatformGame::Initialize()
{
	// Create the scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("scenes/scene.json");
	m_scene->Load("scenes/tilemap2.json");
	m_scene->Initialize();

	// add events
	EVENT_SUBSCRIBE("OnAddPoints", PlatformGame::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", PlatformGame::OnPlayerDead);


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
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_RETURN)) m_state = eState::StartGame;
		if (kiko::g_inputSystem.GetMouseButtonDown(0))
		{
			EVENT_DISPATCH("MouseClicked", kiko::g_inputSystem.GetMousePosition());
		}

	//{
	//	auto actor = INSTANTIATE(Actor, "Crate");
	//	actor->transform.position = { kiko::random(kiko::g_renderer.GetWidth()), 100 };
	//	actor->Initialize();
	//	//m_scene->Add(std::move(actor));
	//}

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
