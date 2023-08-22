#pragma once
#include "Framework/Actor.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Input/InputSystem.h"
#include "SpaceGame.h"
#include "Framework/Emitter.h"
#include "Framework/Components/SpriteComponent.h"
#include "Framework/Components/Component.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Player : public Actor
	{
	public:
		CLASS_DECLARATION(Player);

		/*Player() = default;
		Player(float speed, float turnRate, const kiko::Transform& transform) :
			Actor{ transform },
			m_speed{ speed },
			m_turnRate{ turnRate }
		{}*/

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollision(Actor* other) override;

	private:
		float m_speed = 0;
		float m_turnRate = kiko::Pi;
		float m_powerUpTimer = 0;
		float m_powerUpTime = 18;

		bool m_smallWeapon = false;
		bool m_shieldOn = false;

		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}
