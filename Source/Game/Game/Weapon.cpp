#include "Weapon.h"

namespace kiko
{
	CLASS_DEFINITION(Weapon)

	bool Weapon::Initialize()
	{
		Actor::Initialize();

		auto collisionComponent = GetComponent<kiko::CollisionComponent>();

		if (collisionComponent)  // null check
		{
			auto renderComponent = GetComponent<kiko::RenderComponent>();
			if (renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}

		return true;
	}

	void Weapon::Update(float dt)
	{
		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		transform.position += forward * speed * dt;
	}

	void Weapon::OnCollision(Actor* other)
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
