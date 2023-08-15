#include "Player.h"

bool Player::Initialize()
{
	Actor::Initialize();

	m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
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

void Player::Update(float dt)
{
	Actor::Update(dt);

	// movement
	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * dt;

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);

	m_physicsComponent->ApplyForce(forward * m_speed * thrust);

	m_transform.position += forward * m_speed * thrust * dt;
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());

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
			kiko::Transform transform1{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(25), 0.5 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(500.0f, transform1);
			weapon->m_tag = "p_smallBullet";

			std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "rocket.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));

			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;
			weapon->AddComponent(std::move(collisionComponent));

			weapon->Initialize();
			m_scene->Add(std::move(weapon));

			kiko::Transform transform2{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(0), 0.5 };
			weapon = std::make_unique<Weapon>(500.0f, transform2);
			weapon->m_tag = "p_smallBullet";

			component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "rocket.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));

			collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;
			weapon->AddComponent(std::move(collisionComponent));

			weapon->Initialize();
			m_scene->Add(std::move(weapon));

			kiko::Transform transform3{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(-25), 0.5 };
			weapon = std::make_unique<Weapon>(500.0f, transform3);
			weapon->m_tag = "p_smallBullet";

			component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "rocket.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));

			collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;
			weapon->AddComponent(std::move(collisionComponent));

			weapon->Initialize();
			m_scene->Add(std::move(weapon));

			kiko::g_audioSystem.PlayOneShot("weapon_small", false);
		}
		else
		{
			kiko::Transform transform1{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(10), 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(350.0f, transform1);
			weapon->m_tag = "p_Bullet";

			auto component = std::make_unique<kiko::SpriteComponent>();
			component->m_texture = GET_RESOURCE(kiko::Texture, "rocket.png", kiko::g_renderer);
			weapon->AddComponent(std::move(component));

			auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;
			weapon->AddComponent(std::move(collisionComponent));

			weapon->Initialize();
			m_scene->Add(std::move(weapon));

			kiko::Transform transform3{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(-10), 1 };
			weapon = std::make_unique<Weapon>(350.0f, transform3);
			weapon->m_tag = "p_Bullet";

			collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;
			weapon->AddComponent(std::move(collisionComponent));

			weapon->Initialize();
			m_scene->Add(std::move(weapon));

			kiko::g_audioSystem.PlayOneShot("weapon_big", false);
		}
		
	}

	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(0.5f);
	else kiko::g_time.SetTimeScale(1.0f);

	// targeting system
	/*if (kiko::g_inputSystem.GetMouseButtonDown(0))
	{
		for (Enemy* enemy : m_scene->GetActors<Enemy>())
		{
			if (m_scene->GetActors<Enemy>()[0] == NULL) break;

			if (enemy->m_transform.position.x && enemy->m_transform.position.Distance(kiko::g_inputSystem.GetMousePosition()) <= 20.0f)
			{
				m_transform.rotation = kiko::Vector2::Angle(m_transform.position, enemy->m_transform.position);
				std::cout << "Target aquired" << std::endl;

				return;
			}
		}
	}*/
}

void Player::OnCollision(Actor* other)
{
	if (!m_shieldOn)
	{
		if (other->m_tag == "e_Bullet" || other->m_tag == "Enemy")
		{
			m_game->SetLives(m_game->GetLives() - 1);
			dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
			m_destroyed = true;

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

			kiko::Transform transform{ { m_transform.position }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->SetLifespan(1.0f);
			m_scene->Add(std::move(emitter));

			kiko::g_audioSystem.PlayOneShot("player_destroy", false);
		}

		if (other->m_tag == "e_smallBullet" || other->m_tag == "fastEnemy")
		{
			if (m_speed > 0) m_speed -= 5.0f;
			if (m_speed < 0) m_speed = 1.0f;
		}
	}

	if (other->m_tag == "power_up")
	{
		m_shieldOn = true;
		//m_model = kiko::g_manager.Get("ship_shield.txt");
		m_speed = 20.0f;
		m_game->AddPoints(500);
	}
}
