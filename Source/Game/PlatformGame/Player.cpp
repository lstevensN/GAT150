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

		if (jumpCooldown)
		{
			if (jumpCooldown < jumpTimer) jumpCooldown += dt;
			else jumpCooldown = 0;
		}

		// check if position is off screen, if so wrap the position and set physics component to new position
		if (transform.position.x < 0 || transform.position.x >(float)kiko::g_renderer.GetWidth())
		{
			transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());

			m_physicsComponent->SetPosition(transform.position);
		}

		m_physicsComponent->SetGravityScale((velocity.y > 0) ? 6.0f : 1.5f);

		// check if moving
		if (std::fabs(velocity.x > 0.2f))
		{
			m_spriteAnimComponent->flipH = (velocity.x < -0.1f);
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
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Coin")
		{
			other->destroyed = true;
			kiko::g_audioSystem.PlayOneShot("coin_collect", false);
		}

		if (other->tag == "Ground" || other->tag == "Object" || other->tag == "Enemy")
		{
			groundCount++;
		}
	}

	void Player::OnCollisionExit(Actor* other)
	{
		if ((other->tag == "Ground" || other->tag == "Object" || other->tag == "Enemy"))
		{
			groundCount--;
		}
	}

	void Player::Jump(const kiko::Event& event)
	{
		if (jumpCooldown == 0 && groundCount)
		{
			jumpCooldown += 0.1f;

			vec2 pos = std::get<vec2>(event.data);

			kiko::vec2 up = kiko::vec2{ 0, -1 };
			
			vec2 jumpDistance = { (pos.x - transform.position.x) * 0.035f, (pos.y - transform.position.y) * 0.1f };

			vec2 velocity = m_physicsComponent->m_velocity;

			velocity.x = Clamp(jumpDistance.x, -maxSpeed, maxSpeed);
			velocity.y = Clamp(jumpDistance.y, -jump, jump);

			if (velocity.y > 0) velocity.y = -velocity.y;
			if (transform.position.x < 50 || transform.position.x > 750) velocity.x = -velocity.x;

			m_physicsComponent->SetVelocity(velocity);

			kiko::g_audioSystem.PlayOneShot("jump", false);

			std::cout << "Jump called" << std::endl;
			std::cout << "Current Position: " << transform.position << std::endl;
			std::cout << "Mouse Position: " << pos << std::endl;
			std::cout << "Jump Distance: " << jumpDistance.x << std::endl;
			std::cout << "Jump Height: " << jumpDistance.y << std::endl;
			std::cout << "Applied Velocity: " << (velocity + (up * jump)) << "\n" << std::endl;
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
