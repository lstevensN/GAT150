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

			if (fly && transform.position.y > 200.0f)
			{
				kiko::vec2 up = kiko::vec2{ 0.0f, -1.0f };
				m_physicsComponent->SetVelocity(m_physicsComponent->m_velocity + (up * jump));
			}
		}
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Player")
		{
			fly = true;
		}
	}
	
	void Enemy::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Player")
		{
			fly = false;
		}
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, name);
		READ_DATA(value, m_speed);
		READ_DATA(value, jump);
	}
}
