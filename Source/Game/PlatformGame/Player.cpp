#include "Player.h"
#include "Input/InputSystem.h"

#define EVENT_SUBSCRIBE(id, function)	kiko::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1));
#define EVENT_UNSUBSCRIBE(id)			kiko::EventManager::Instance().Unsubscribe(id, this);
#define EVENT_DISPATCH(id, value)       kiko::EventManager::Instance().DispatchEvent(id, value);

namespace kiko
{
	CLASS_DEFINITION(Player);

	bool Player::Initialize()
	{
		Actor::Initialize();

		EVENT_SUBSCRIBE("MouseClicked", Player::Jump);

		m_physicsComponent = GetComponent<PhysicsComponent>();
		m_spriteAnimComponent = GetComponent<SpriteAnimRenderComponent>();

		return true;
	}

	void Player::Update(float dt)
	{
		Actor::Update(dt);
		
		bool onGround = (groundCount > 0);
		vec2 velocity = m_physicsComponent->m_velocity;

		//kiko::g_inputSystem.GetMousePosition

		// movement
		float dir = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LEFT)) dir = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_RIGHT)) dir = 1;

		if (dir)
		{
			velocity.x += speed * dir * ((onGround) ? 1 : 0.25f) * dt;
			velocity.x = Clamp(velocity.x, -maxSpeed, maxSpeed);
			m_physicsComponent->SetVelocity(velocity);
		}

		// jump
		if (onGround && kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKey(SDL_SCANCODE_SPACE))
		{
			kiko::vec2 up = kiko::vec2{ 0, -1 };
			m_physicsComponent->SetVelocity(velocity + (up * jump));
		}

		m_physicsComponent->SetGravityScale((velocity.y > 0) ? 7.0f : 2.0f);

		// check if moving
		if (std::fabs(velocity.x > 0.2f))
		{
			if (dir != 0) m_spriteAnimComponent->flipH = (dir < -0.1f);
			m_spriteAnimComponent->SetSequence("run");
		}
		else
		{
			m_spriteAnimComponent->SetSequence("idle");
		}

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T))
		{
			kiko::g_time.SetTimeScale(0.5f);
		}
		else kiko::g_time.SetTimeScale(1.0f);

		//std::cout << groundCount << " " << onGround << std::endl;
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Enemy")
		{
			//destroyed = true;
			// EVENT
		}

		if (other->tag == "Ground" || other->tag == "Object" || other->tag == "Enemy") groundCount++;
	}

	void Player::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground" || other->tag == "Object" || other->tag == "Enemy")
		{
			groundCount--;
		}
	}

	void Player::Jump(const kiko::Event& event)
	{
		if (groundCount && !inAir)
		{
			vec2 pos = std::get<vec2>(event.data);

			float jumpDistance = pos.x - transform.position.x;

			vec2 velocity = m_physicsComponent->m_velocity;

			kiko::vec2 up = kiko::vec2{ 0, -1 };

			velocity.x = Clamp(jumpDistance, -maxSpeed, maxSpeed);

			m_physicsComponent->SetVelocity(velocity + (up * jump));

			std::cout << "Jump called" << std::endl;
			std::cout << "Current Position: " << transform.position << std::endl;
			std::cout << "Mouse Position: " << pos << std::endl;
			std::cout << "Jump Distance: " << jumpDistance << std::endl;
			std::cout << "Applied Velocity: " << m_physicsComponent->m_velocity << "\n" << std::endl;
		}
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, name);
		READ_DATA(value, speed);
		READ_DATA(value, maxSpeed);
		READ_DATA(value, jump);
	}
}
