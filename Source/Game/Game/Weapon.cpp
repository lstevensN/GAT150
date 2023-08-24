#include "Weapon.h"

namespace kiko
{
	CLASS_DEFINITION(Weapon)

	bool Weapon::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		auto collisionComponent = GetComponent<kiko::CollisionComponent>();

		if (collisionComponent)  // null check
		{
			
		}

		return true;
	}

	void Weapon::Update(float dt)
	{
		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		m_physicsComponent->SetVelocity(forward * speed);

		transform.position += forward * speed * dt;
	}

	void Weapon::OnCollisionEnter(Actor* other)
	{
		if (tag == "p_bigBullet" && other->tag == "Enemy" ||
			tag == "p_smallBullet" && other->tag == "Enemy" ||
			tag == "p_Bullet" && other->tag == "fastEnemy" ||
			tag == "p_smallBullet" && other->tag == "fastEnemy" ||
			tag == "e_Bullet" && other->tag == "Player" ||
			tag == "e_fastBullet" && other->tag == "Player" ||
			other->tag == "power_up") destroyed = true;
	}

	void Weapon::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, name);
		READ_DATA(value, speed);
	}
}
