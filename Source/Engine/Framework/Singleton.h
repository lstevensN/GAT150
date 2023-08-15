#pragma once
#include "Resource/ResourceManager.h"

#define GET_RESOURCE(type, filename, ...) kiko::ResourceManager::Instance().Get<type>(filename, __VA_ARGS__)

namespace kiko
{
	template<typename T>
	class Singleton
	{
	public:
		Singleton(const Singleton& other) = delete;
		Singleton& operator = (const Singleton& other) = delete;

		static T& Instance()
		{
			static T instance;
			return instance;
		}

	protected:
		Singleton() = default;
	};
}
