#include "Weapon.h"

void Weapon::Update(float dt)
{
	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * dt;
	//m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	//m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());

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
