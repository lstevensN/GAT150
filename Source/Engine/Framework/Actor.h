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
		CLASS_DECLARATION(Actor);

		Actor() = default;
		Actor(const kiko::Transform& transform) :
			transform{ transform }
		{}
		Actor(const Actor& other);

		virtual bool Initialize() override;
		virtual void OnDestroy() override;

		virtual void Update(float dt);
		virtual void Draw(kiko::Renderer& renderer);

		void AddComponent(std::unique_ptr<Component> component);
		template <typename T>
		T* GetComponent();

		virtual void OnCollisionEnter(Actor* other) {}
		virtual void OnCollisionExit(Actor* other) {}

		void AddForce(vec2 force) { m_velocity += force; }
		void SetDamping(float damping) { m_damping = damping; }

		friend class Scene;
		class Scene* m_scene = nullptr;

		friend class Game;
		class Game* m_game = nullptr;
		
		friend class Component;

		kiko::Transform transform;
		std::string tag;
		bool destroyed = false;
		bool persistent = false;
		bool prototype = false;
		float lifespan = -10.0f;
		std::shared_ptr<Model> m_model;

		vec2 m_velocity;
		float m_damping = 0;

	protected:
		std::vector<std::unique_ptr<Component>> components;
	};

	template<typename T>
	inline T* Actor::GetComponent()
	{
		for (auto& component : components)
		{
			T* result = dynamic_cast<T*>(component.get());
			if (result) return result;
		}

		return nullptr;
	}
}
