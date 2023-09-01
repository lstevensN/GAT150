#include "Enemy.h"
#include "Player.h"

#define EVENT_SUBSCRIBE(id, function)	kiko::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1));
#define EVENT_UNSUBSCRIBE(id)			kiko::EventManager::Instance().Unsubscribe(id, this);
#define EVENT_DISPATCH(id, value)       kiko::EventManager::Instance().DispatchEvent(id, value);

namespace kiko
{
	CLASS_DEFINITION(Enemy)

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		EVENT_SUBSCRIBE("RMouseClicked", Enemy::Clicked);

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		m_spriteAnimComponent = GetComponent<kiko::SpriteAnimRenderComponent>();

		return true;
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		if (stunCooldown)
		{
			if (stunCooldown < stunTimer) stunCooldown += dt;
			else
			{
				stunCooldown = 0;
				EVENT_DISPATCH("EnemyDead", "F");
				destroyed = true;
			}
		}
		else
		{
			kiko::vec2 forward = kiko::vec2{ 1, 0 }.Rotate(transform.rotation);

			Player* player = m_scene->GetActor<Player>();
			if (player)
			{
				kiko::vec2 direction = { player->transform.position.x - transform.position.x,
					((transform.position.y < 350.0f) ? player->transform.position.y - transform.position.y : 0) };
				m_physicsComponent->ApplyForce(direction.Normalized() * m_speed);

				if (fly && transform.position.y > 150.0f)
				{
					kiko::vec2 up = kiko::vec2{ 0.0f, -1.0f };
					m_physicsComponent->SetVelocity(m_physicsComponent->m_velocity + (up * jump));
				}
			}
		}
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Player")
		{
			fly = true;
		}

		if (other->tag == "Ground")
		{
			m_physicsComponent->ApplyForce({ 0.0f, -50.0f * m_speed });
		}
		
		if (other->tag == "Cursor")
		{
			touched = true;
		}
	}
	
	void Enemy::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Player")
		{
			fly = false;
		}

		if (other->tag == "Cursor")
		{
			touched = false;
		}
	}

	void Enemy::Clicked(const kiko::Event& event)
	{
		vec2 pos = std::get<vec2>(event.data);

		if (touched && !stunCooldown)
		{
			stunCooldown += 0.1f;

			m_spriteAnimComponent->SetSequence("stunned");
			m_physicsComponent->SetVelocity({ 0, 0 });

			kiko::g_audioSystem.PlayOneShot("stun", false);
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
