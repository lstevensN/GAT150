#pragma once
#include "RenderComponent.h"
#include "Renderer/Model.h"

namespace kiko
{
	class ModelRenderComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(ModelRenderComponent);

		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

		virtual float GetRadius() override { return m_model->GetRadius(); }

		bool Initialize() override;

	public:
		std::string modelName;
		res_t<Model> m_model;
	};
}
