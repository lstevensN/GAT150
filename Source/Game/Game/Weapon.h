#pragma once
#include "Framework/Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "SpaceGame.h"

namespace kiko
{
	class Weapon : public Actor
	{
	public:
		CLASS_DECLARATION(Weapon)

		/*
		Weapon() = default;
		Weapon(float speed, const kiko::Transform& transform) :
			Actor{ transform },
			m_speed{ speed }
		{
			lifespan = 1.5f;
		}
		*/

		bool Initialize() override;

		void Update(float dt) override;
		void OnCollision(Actor* other);

	private:
		float speed = 0;
	};
}
