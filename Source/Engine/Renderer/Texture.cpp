#include "Texture.h"
#include "Renderer.h"
#include <SDL2-2.28.1/include/SDL_image.h>

namespace kiko
{
	Texture::~Texture()
	{
		if (m_texture) SDL_DestroyTexture(m_texture);
	}

	bool Texture::Create(Renderer& renderer, const std::string& filename)
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			WARNING_LOG("Surface not found");
			return false;
		}

		m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);
		SDL_FreeSurface(surface);
		if (!m_texture)
		{
			WARNING_LOG("Surface not found");
			return false;
		}

		return true;
	}

	vec2 Texture::GetSize()
	{
		ASSERT_LOG(m_texture, "Texture is null");
		SDL_Point point;
		SDL_QueryTexture(m_texture, nullptr, nullptr, &point.x, &point.y);

		return vec2{ point.x, point.y };
	}
}
