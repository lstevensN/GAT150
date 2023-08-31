#pragma once
#include "Framework/Framework.h"

namespace kiko
{
	class Player : public Actor, kiko::IEventListener
	{
	public:
		CLASS_DECLARATION(Player);

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;

		void Jump(const kiko::Event& event);

	private:
		float speed = 0;
		float maxSpeed = 0;
		float jump = 0;
		int groundCount = 0;
		bool inAir = false;

		class PhysicsComponent* m_physicsComponent = nullptr;
		class SpriteAnimRenderComponent* m_spriteAnimComponent = nullptr;
	};
}
