#pragma once
#include "Object.h"
#include "Core/Core.h"
#include "Scene.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Components/Component.h"
#include <memory>

namespace kiko
{
	class Actor : public Object
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

		virtual bool Initialize() override;
		virtual void OnDestroy() override;

		virtual void Update(float dt);
		virtual void Draw(kiko::Renderer& renderer);

		void AddComponent(std::unique_ptr<Component> component);
		template <typename T>
		T* GetComponent();

		inline float GetRadius() { return (m_model) ? m_model->GetRadius() * m_transform.scale : -10000; }
		virtual void OnCollision(Actor* other) {}

		void AddForce(vec2 force) { m_velocity += force; }
		void SetDamping(float damping) { m_damping = damping; }

		friend class Scene;
		class Scene* m_scene = nullptr;

		friend class Game;
		class Game* m_game = nullptr;
		
		friend class Component;

		kiko::Transform m_transform;
		std::string m_tag;

	protected:
		std::vector<std::unique_ptr<Component>> m_components;

		bool m_destroyed = false;
		float m_lifespan = -10.0f;
		std::shared_ptr<Model> m_model;

		vec2 m_velocity;
		float m_damping = 0;
	};

	template<typename T>
	inline T* Actor::GetComponent()
	{
		for (auto& component : m_components)
		{
			T* result = dynamic_cast<T*>(component.get());
			if (result) return result;
		}

		return nullptr;
	}
}
