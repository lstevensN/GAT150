#include "Font.h"
#include <SDL2-2.28.1/include/SDL_ttf.h>
#include "Core/Logger.h"

namespace kiko
{
	Font::Font(const std::string& filename, int fontSize)
	{
		Load(filename, fontSize);
	}

	Font::~Font()
	{
		if (m_ttfFont) TTF_CloseFont(m_ttfFont);
	}

	bool Font::Load(const std::string& filename, int fontSize)
	{
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);
		if (m_ttfFont == nullptr)
		{
			WARNING_LOG("Failed to open font: " << filename);
			return false;
		}

		return true;
	}

	bool Font::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);

		// va_arg - accesses the next variadic function arguments
		int& fontSize = va_arg(args, int);
		va_end(args);

		return Load(filename, fontSize);
	}
}
