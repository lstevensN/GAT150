#pragma once
#include "Resource.h"
#include "Framework/Singleton.h"
#include "Core/Logger.h"
#include <map>
#include <memory>
#include <string>

namespace kiko
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		template<typename T, typename ... TArgs>
		res_t<T> Get(const std::string& filename, TArgs ... args);

	private:
		std::map<std::string, res_t<Resource>> m_resources;
	};

	template<typename T, typename ...TArgs>
	inline res_t<T> ResourceManager::Get(const std::string& filename, TArgs ... args)
	{
		// find resource in resource manager
		if (m_resources.find(filename) != m_resources.end())
		{
			// return resource
			return std::dynamic_pointer_cast<T>(m_resources[filename]);
		}

		// resource not in resource manager, create resource
		res_t<T> resource = std::make_shared<T>();
		if (!resource->Create(filename, args...))
		{
			WARNING_LOG("Could not create resource: " << filename);
			return res_t<T>();
		}

		// return resource
		m_resources[filename] = resource;
		return resource;
	}
}
