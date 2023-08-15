#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"

#include "Framework/Framework.h"
#include "Framework/Singleton.h"
#include "Renderer/Renderer.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"


class SpaceGame : public kiko::Game
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

	// Inherited via Game
	virtual bool Initialize() override;

	virtual void Shutdown() override;

	virtual void Update(float dt) override;

	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }

	//void SetMarkedEnemy(Enemy* enemy) { m_markedEnemy = enemy; }
	//Enemy* GetMarkedEnemy() const { return m_markedEnemy; }

private:
	eState m_state = eState::Title;
	float m_spawnTimer = -4;
	float m_fastSpawnTimer = 0;
	float m_powerUpTimer = -30;
	float m_spawnTime = 3;
	int m_enemyCap = 0;

	//Enemy* m_markedEnemy = nullptr;

	float m_stateTimer = 0;

	std::shared_ptr<kiko::Font> m_font;
	std::unique_ptr<kiko::Text> m_scoreText;
	std::unique_ptr<kiko::Text> m_titleText;
	std::unique_ptr<kiko::Text> m_gameOverText;
	std::unique_ptr<kiko::Text> m_livesText;
};
