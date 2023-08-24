#include "Enemy.h"

namespace kiko
{
	CLASS_DEFINITION(Enemy)

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
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

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			kiko::Vector2 direction = player->transform.position - transform.position;

			// Turn towards player
			float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());

			//transform.rotation += turnAngle * 5 * dt;
			m_physicsComponent->ApplyTorque(turnAngle);

			if (std::fabs(turnAngle) < kiko::DegreesToRadians(30.0))
			{

			}
		}

		//kiko::vec2::Angle(forward, direction.Normalized());
		//if (angle < kiko::DegreesToRadians(30.0f)) 
		//{
		//	// I see you
		//}

		m_physicsComponent->ApplyForce(forward * m_speed);

		//transform.position += forward * m_speed * dt;
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

		if (m_fireTimer <= 0)
		{
			if (tag == "Enemy")
			{
				auto weapon = INSTANTIATE(Weapon, "EnemyWeapon");
				weapon->transform = { transform.position, transform.rotation, weapon->transform.scale };
				weapon->Initialize();
				weapon->m_game = this->m_game;
				m_scene->Add(std::move(weapon));

				/*kiko::Transform transform{ transform.position, transform.rotation, 1 };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(350.0f, transform);
				weapon->tag = "e_Bullet";
				m_scene->Add(std::move(weapon));*/

				kiko::g_audioSystem.PlayOneShot("weapon_strong", false);
			}
			else if (tag == "fastEnemy")
			{
				/*kiko::Transform transform{ transform.position, transform.rotation, 1 };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(500.0f, transform);
				weapon->tag = "e_fastBullet";
				m_scene->Add(std::move(weapon));

				kiko::g_audioSystem.PlayOneShot("weapon_fast", false);*/
			}

			m_fireTimer = m_fireRate;
		}
		else m_fireTimer -= dt;
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		if (this->tag == "Enemy")
		{
			if (other->tag == "p_Bullet" || other->tag == "Player" || other->tag == "p_smallBullet" && m_hitCount == 2)
			{
				kiko::EventManager::Instance().DispatchEvent("OnAddPoints", 125);
				
				destroyed = true;

				kiko::EmitterData data;
				data.burst = true;
				data.burstCount = 50;
				data.spawnRate = 0;
				data.angle = 0;
				data.angleRange = kiko::Pi;
				data.lifetimeMin = 0.5f;
				data.lifetimeMax = 1.5f;
				data.speedMin = 50;
				data.speedMax = 250;
				data.damping = 0.5f;

				data.color = kiko::Color{ 1, 1, 1, 1 };

				kiko::Transform transform{ { transform.position }, 0, 1 };
				auto emitter = std::make_unique<kiko::Emitter>(transform, data);
				emitter->SetLifespan(1.0f);
				m_scene->Add(std::move(emitter));

				kiko::g_audioSystem.PlayOneShot("strong_enemy_destroy", false);
			}
			else if (other->tag == "p_smallBullet")
			{
				if (m_speed > 0) m_speed = 5;
				m_hitCount++;
			}
		}
		else if (this->tag == "power_up" && other->tag == "Player")
		{
			destroyed = true;

			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 0;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 1.0f;
			data.lifetimeMax = 1.5f;
			data.speedMin = 1000;
			data.speedMax = 1250;
			data.damping = 0.7f;

			data.color = kiko::Color{ 1, 1, 0, 1 };

			kiko::Transform transform{ { transform.position }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->SetLifespan(1.0f);
			m_scene->Add(std::move(emitter));

			kiko::g_audioSystem.PlayOneShot("power_up", false);
		}
		else if (this->tag == "fastEnemy" && other->tag != "power_up" && other->tag != "e_fastBullet")
		{
			destroyed = true;

			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 25;
			data.spawnRate = 0;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.25f;
			data.lifetimeMax = 1.0f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.9f;

			data.color = kiko::Color{ 1, 1, 1, 1 };

			kiko::Transform transform{ { this->transform.position }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->SetLifespan(1.0f);
			m_scene->Add(std::move(emitter));

			m_game->AddPoints(50);
			kiko::g_audioSystem.PlayOneShot("enemy_destroy", false);
		}
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, name);
		READ_DATA(value, m_speed);
	}
}
