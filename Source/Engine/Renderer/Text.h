#pragma once
#include "Renderer/Renderer.h"
#include "Font.h"
#include "Core/Core.h"
#include <string>
#include <memory>

namespace kiko
{
	class Text
	{
	public:
		Text() = default;
		Text(std::shared_ptr<kiko::Font> font) : m_font{ font } {}
		~Text();

		void Create(Renderer& renderer, const std::string& text, const Color& color);
		void Draw(Renderer& renderer, int x, int y);

		friend class Renderer;

		friend class Font;

	private:
		std::shared_ptr<kiko::Font> m_font;
		struct SDL_Texture* m_texture = nullptr;
	};
}
