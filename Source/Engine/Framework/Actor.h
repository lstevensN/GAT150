#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include "Framework/Scene.h"
#include "Renderer/Renderer.h"
#include "Renderer/ModelManager.h"
#include "Audio/AudioSystem.h"
#include <memory>

namespace kiko
{
	class Actor
	{
	public:
		Actor() = default;
		Actor(const kiko::Transform& transform, std::shared_ptr<Model> model) :
			m_transform{ transform },
			m_model{ model }
		{}
		Actor(const kiko::Transform& transform) :
			m_transform{ transform }
		{}

		virtual void Update(float dt);
		virtual void Draw(kiko::Renderer& renderer);

		inline float GetRadius() { return (m_model) ? m_model->GetRadius() * m_transform.scale : -10000; }
		virtual void OnCollision(Actor* other) {}

		void AddForce(vec2 force) { m_velocity += force; }
		void SetDamping(float damping) { m_damping = damping; }

		friend class Scene;
		class Scene* m_scene = nullptr;

		friend class Game;
		class Game* m_game = nullptr;

		kiko::Transform m_transform;
		std::string m_tag;

	protected:
		bool m_destroyed = false;
		float m_lifespan = -10.0f;
		std::shared_ptr<Model> m_model;

		vec2 m_velocity;
		float m_damping = 0;
	};
}
