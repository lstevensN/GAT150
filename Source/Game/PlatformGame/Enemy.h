#pragma once
#include "Framework/Framework.h"

namespace kiko
{
	class Enemy : public kiko::Actor, kiko::IEventListener
	{
	public:
		CLASS_DECLARATION(Enemy)

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;
		void Clicked(const kiko::Event& event);

		void SetSpeed(const float& speed) { m_speed = speed; }

	private:
		float m_speed = 0;
		float jump = 0;
		int groundCount = 0;
		bool fly = false;
		bool touched = false;

		float stunTimer = 3;
		float stunCooldown = 0;

		kiko::PhysicsComponent* m_physicsComponent = nullptr;
		kiko::SpriteAnimRenderComponent* m_spriteAnimComponent = nullptr;
	};
}
