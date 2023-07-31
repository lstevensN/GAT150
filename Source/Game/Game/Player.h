#pragma once
#include "Framework/Actor.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Input/InputSystem.h"
#include "SpaceGame.h"
#include "Framework/Emitter.h"

class Player : public kiko::Actor
{
public:
	Player() = default;
	Player(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{}

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnRate = 0;
	float m_powerUpTimer = 0;
	float m_powerUpTime = 18;

	bool m_smallWeapon = false;
	bool m_shieldOn = false;
};
