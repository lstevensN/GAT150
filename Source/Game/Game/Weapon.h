#pragma once
#include "Framework/Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "SpaceGame.h"

class Weapon : public kiko::Actor
{
public:
	Weapon() = default;
	Weapon(float speed, const kiko::Transform& transform) :
		Actor{ transform },
		m_speed{ speed }
	{
		m_lifespan = 1.5f;
	}

	bool Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
};
