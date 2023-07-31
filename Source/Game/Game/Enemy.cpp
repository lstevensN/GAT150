#include "Enemy.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player)
	{
		kiko::Vector2 direction = player->m_transform.position - m_transform.position;
		
		// Turn towards player
		float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());

		m_transform.rotation += turnAngle * 5 * dt;

		if (std::fabs(turnAngle) < kiko::DegreesToRadians(30.0)) 
		{

		}
	}

	//kiko::vec2::Angle(forward, direction.Normalized());
	//if (angle < kiko::DegreesToRadians(30.0f)) 
	//{
	//	// I see you
	//}

	m_transform.position += forward * m_speed * dt;
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());

	if (m_fireTimer <= 0)
	{
		if (m_tag == "Enemy")
		{
			kiko::Transform transform{ m_transform.position, m_transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(350.0f, transform, kiko::g_manager.Get("weapon_strong.txt"));
			weapon->m_tag = "e_Bullet";
			m_scene->Add(std::move(weapon));

			kiko::g_audioSystem.PlayOneShot("weapon_strong", false);
		}
		else if (m_tag == "fastEnemy")
		{
			kiko::Transform transform{ m_transform.position, m_transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(500.0f, transform, m_model);
			weapon->m_tag = "e_fastBullet";
			m_scene->Add(std::move(weapon));

			kiko::g_audioSystem.PlayOneShot("weapon_fast", false);
		}

		m_fireTimer = m_fireRate;
	}
	else m_fireTimer -= dt;
}

void Enemy::OnCollision(Actor* other)
{
	if (this->m_tag == "Enemy")
	{
		if (other->m_tag == "p_Bullet" || other->m_tag == "Player" || other->m_tag == "p_smallBullet" && m_hitCount == 2)
		{
			m_destroyed = true;

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

			kiko::Transform transform{ { m_transform.position }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->SetLifespan(1.0f);
			m_scene->Add(std::move(emitter));

			m_game->AddPoints(125);
			kiko::g_audioSystem.PlayOneShot("strong_enemy_destroy", false);
		}
		else if (other->m_tag == "p_smallBullet")
		{
			if (m_speed > 0) m_speed = 5;
			m_hitCount++;
		}
	}
	else if (this->m_tag == "power_up" && other->m_tag == "Player")
	{
		m_destroyed = true;

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

		kiko::Transform transform{ { m_transform.position }, 0, 1 };
		auto emitter = std::make_unique<kiko::Emitter>(transform, data);
		emitter->SetLifespan(1.0f);
		m_scene->Add(std::move(emitter));

		kiko::g_audioSystem.PlayOneShot("power_up", false);
	}
	else if (this->m_tag == "fastEnemy" && other->m_tag != "power_up" && other->m_tag != "e_fastBullet")
	{
		m_destroyed = true;

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

		kiko::Transform transform{ { m_transform.position }, 0, 1 };
		auto emitter = std::make_unique<kiko::Emitter>(transform, data);
		emitter->SetLifespan(1.0f);
		m_scene->Add(std::move(emitter));

		m_game->AddPoints(50);
		kiko::g_audioSystem.PlayOneShot("enemy_destroy", false);
	}
}
