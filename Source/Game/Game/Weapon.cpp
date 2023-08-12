#include "Weapon.h"

bool Weapon::Initialize()
{
	Actor::Initialize();

	auto collisionComponent = GetComponent<kiko::CollisionComponent>();

	if (collisionComponent)  // null check
	{
		auto renderComponent = GetComponent<kiko::RenderComponent>();
		if (renderComponent)
		{
			float scale = m_transform.scale;
			collisionComponent->m_radius = renderComponent->GetRadius() * scale;
		}
	}

	return true;
}

void Weapon::Update(float dt)
{
	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * dt;

	Actor::Update(dt);
}

void Weapon::OnCollision(Actor* other)
{
	if (this->m_tag == "p_bigBullet" && other->m_tag == "Enemy" ||
		this->m_tag == "p_smallBullet" && other->m_tag == "Enemy" ||
		this->m_tag == "p_Bullet" && other->m_tag == "fastEnemy" ||
		this->m_tag == "p_smallBullet" && other->m_tag == "fastEnemy" ||
		this->m_tag == "e_Bullet" && other->m_tag == "Player" ||
		this->m_tag == "e_fastBullet" && other->m_tag == "Player" ||
		other->m_tag == "power_up") m_destroyed = true;
}
