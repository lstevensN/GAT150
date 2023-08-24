#pragma once
#include "Framework/Actor.h"
#include "Framework/Emitter.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"

namespace kiko
{
	class Enemy : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Enemy)

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;

	private:
		float m_speed = 0;
		float m_turnRate = kiko::Pi;
		int m_hitCount = 0;

		float m_fireTimer = 3;
		float m_fireRate = 2;

		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}
