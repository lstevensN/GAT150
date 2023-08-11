#include "Actor.h"
#include "Components/RenderComponent.h"

namespace kiko
{
	bool Actor::Initialize()
	{
		for (auto& component : m_components)
		{
			component->Initialize();
		}

		return true;
	}

	void Actor::OnDestroy()
	{
		for (auto& component : m_components)
		{
			component->OnDestroy();
		}
	}

	void Actor::Update(float dt)
	{
		if (m_lifespan != -10.0f)
		{
			m_lifespan -= dt;
			if (m_lifespan < 0) m_destroyed = true;
		}

		for (auto& component : m_components)
		{
			component->Update(dt);
		}
	}

	void Actor::Draw(kiko::Renderer& renderer)
	{
		for (auto& component : m_components)
		{
			RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component.get());
			if (renderComponent)
			{
				renderComponent->Draw(renderer);
			}
		}
	}

	void Actor::AddComponent(std::unique_ptr<Component> component)
	{
		component->m_owner = this;
		m_components.push_back(std::move(component));
	}
}
