#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"
#include "Framework/Factory.h"

namespace kiko
{
	class SpriteComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(SpriteComponent);

		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

		bool Initialize() override;

	public:
		Rect source;
		std::string textureName;
		res_t<Texture> m_texture;
	};
}
