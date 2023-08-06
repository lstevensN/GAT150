#include "Actor.h"
#include "Components/RenderComponent.h"

namespace kiko
{
	void Actor::Update(float dt)
	{
		if (m_lifespan != -10.0f)
		{
			m_lifespan -= dt;
			if (m_lifespan < 0) m_destroyed = true;
		}

		m_transform.position += m_velocity * dt;
		m_velocity *= std::pow(1.0f - m_damping, dt);
	}

	void Actor::Draw(kiko::Renderer& renderer)
	{
		//m_model->Draw(renderer, m_transform);

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
