#include "Enemy.h"
#include "Player.h"

namespace kiko
{
	CLASS_DEFINITION(Enemy)

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();

		return true;
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2{ 1, 0 }.Rotate(transform.rotation);

		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			kiko::vec2 direction = player->transform.position - transform.position;
			m_physicsComponent->ApplyForce(direction.Normalized() * m_speed);

			/*float direction = player->transform.position.x - transform.position.x;

			float dir = (direction > 0) ? 2.0 : -2.0;

			m_physicsComponent->ApplyForce(forward * m_speed * dir);

			if (direction <= 200.0 || direction >= -200.0)
			{
				transform.position += forward * m_speed * dir * dt;
			}*/
		}
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, name);
		READ_DATA(value, m_speed);
		READ_DATA(value, jump);
	}
}
