#include "Player.h"

namespace kiko
{
	CLASS_DEFINITION(Player);

	bool Player::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		auto collisionComponent = GetComponent<kiko::CollisionComponent>();

		if (collisionComponent)  // null check
		{
			auto renderComponent = GetComponent<kiko::SpriteComponent>();
			if (renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}

		return true;
	}

	void Player::Update(float dt)
	{
		Actor::Update(dt);

		// movement
		float rotate = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		//transform.rotation += rotate * m_turnRate * dt;
		m_physicsComponent->ApplyTorque(rotate * m_turnRate);

		float thrust = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);

		m_physicsComponent->ApplyForce(forward * m_speed * thrust);

		transform.position += forward * m_speed * thrust * dt;
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

		// shield
		if (m_powerUpTimer <= m_powerUpTime && m_shieldOn) m_powerUpTimer += dt;
		else
		{
			m_shieldOn = false;
			m_powerUpTimer = 0;
			m_speed = 15.0f;
		}

		// switch weapons
		if (kiko::g_inputSystem.GetMouseButtonDown(2) && !kiko::g_inputSystem.GetMousePreviousButton(2)) (m_smallWeapon) ? m_smallWeapon = false : m_smallWeapon = true;

		// fire weapon(s)
		if (kiko::g_inputSystem.GetMouseButtonDown(0) && !kiko::g_inputSystem.GetMousePreviousButton(0) && !m_shieldOn)
		{
			// small weapon
			if (m_smallWeapon)
			{
				/*
				kiko::Transform transform1{ transform.position, transform.rotation + kiko::DegreesToRadians(25), 0.5 };

				kiko::Transform transform2{ transform.position, transform.rotation + kiko::DegreesToRadians(0), 0.5 };

				kiko::Transform transform3{ transform.position, transform.rotation + kiko::DegreesToRadians(-25), 0.5 };

				kiko::g_audioSystem.PlayOneShot("weapon_small", false);
				*/
			}
			else
			{
				auto weapon = INSTANTIATE(Weapon, "Rocket");
				weapon->transform = { transform.position, transform.rotation + kiko::DegreesToRadians(10), weapon->transform.scale };
				weapon->Initialize();
				weapon->m_game = this->m_game;
				m_scene->Add(std::move(weapon));

				weapon = INSTANTIATE(Weapon, "Rocket");
				weapon->transform = { transform.position, transform.rotation + kiko::DegreesToRadians(-10), weapon->transform.scale };
				weapon->Initialize();
				weapon->m_game = this->m_game;
				m_scene->Add(std::move(weapon));

				kiko::g_audioSystem.PlayOneShot("weapon_big", false);
			}
		}

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(0.5f);
		else kiko::g_time.SetTimeScale(1.0f);
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (!m_shieldOn)
		{
			if (other->tag == "e_Bullet" || other->tag == "Enemy")
			{
				kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
				//m_game->SetLives(m_game->GetLives() - 1);
				//dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
				destroyed = true;

				kiko::EmitterData data;
				data.burst = true;
				data.burstCount = 50;
				data.spawnRate = 0;
				data.angle = 0;
				data.angleRange = kiko::Pi;
				data.lifetimeMin = 0.5f;
				data.lifetimeMax = 1.5f;
				data.speedMin = 150;
				data.speedMax = 250;
				data.damping = 0.9f;

				data.color = kiko::Color{ 0, 1, 0, 1 };

				kiko::Transform transform{ { transform.position }, 0, 1 };
				auto emitter = std::make_unique<kiko::Emitter>(transform, data);
				emitter->SetLifespan(1.0f);
				m_scene->Add(std::move(emitter));

				kiko::g_audioSystem.PlayOneShot("player_destroy", false);
			}

			if (other->tag == "e_smallBullet" || other->tag == "fastEnemy")
			{
				if (m_speed > 0) m_speed -= 5.0f;
				if (m_speed < 0) m_speed = 1.0f;
			}
		}

		if (other->tag == "power_up")
		{
			m_shieldOn = true;
			//m_model = kiko::g_manager.Get("ship_shield.txt");
			m_speed = 20.0f;
			m_game->AddPoints(500);
		}
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, name);
		READ_DATA(value, m_speed);
		READ_DATA(value, m_turnRate);
	}
}
