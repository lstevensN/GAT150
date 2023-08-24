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

		bool Initialize() override;

		void Update(float dt) override;
		void OnCollisionEnter(Actor* other);

	private:
		float speed = 0;
		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}
