#pragma once
#include "Core/Math/Vector2.h"
#include <variant>
#include <string>

namespace kiko
{
	class Event
	{
	public:
		using id_t = std::string;
		using data_t = std::variant<int, bool, float, std::string, kiko::vec2>;

	public:
		Event() = default;
		Event(const id_t& id, data_t data) : 
			id{ id }, 
			data{data} 
		{}

	public:
		id_t id;
		data_t data;
	};
}
