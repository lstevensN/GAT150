#pragma once
#include "Framework/Framework.h"
#include "Framework/Singleton.h"
#include "Framework/Event/EventManager.h"

#include "Renderer/Renderer.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"


class PlatformGame : public kiko::Game, kiko::IEventListener
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		PlayerDeadStart,
		PlayerDead,
		GameOverStart,
		GameOver
	};

public:
	virtual bool Initialize() override;

	virtual void Shutdown() override;

	virtual void Update(float dt) override;

	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }

	void OnAddPoints(const kiko::Event& event);
	void OnPlayerDead(const kiko::Event& event);

private:
	eState m_state = eState::Title;
	float m_spawnTimer = -4;
	float m_fastSpawnTimer = 0;
	float m_powerUpTimer = -30;
	float m_spawnTime = 3;
	int m_enemyCap = 0;

	float m_stateTimer = 0;
};
