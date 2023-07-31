#pragma once
#include "Framework/Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "SpaceGame.h"

class Weapon : public kiko::Actor
{
public:
	Weapon() = default;
	Weapon(float speed, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model },
		m_speed{ speed }
	{
		m_lifespan = 1.5f;
	}

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
};
