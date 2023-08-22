#pragma once
#include "Actor.h"
#include <list>

namespace kiko
{
	class Renderer;  // Foreward declaration
	class Actor;

	class Scene
	{
	public:
		Scene() = default;

		bool Initialize();

		void Update(float dt);
		void Draw(Renderer& renderer);

		void Add(std::unique_ptr<Actor> actor);
		void RemoveAll(bool force = false);

		bool Load(const std::string& filename);
		void Read(const json_t& value);

		template<typename T>
		inline T* GetActor()
		{
			for (auto& actor : m_actors)
			{
				T* result = dynamic_cast<T*>(actor.get());
				if (result) return result;
			}

			return nullptr;
		}

		template<typename T>
		inline T* GetActorByName(const std::string& name)
		{
			for (auto& actor : m_actors)
			{
				if (actor->name == name)
				{
					T* result = dynamic_cast<T*>(actor.get());
					if (result) return result;
				}
			}
			
			return nullptr;
		}

		template<typename T>
		inline std::array<T*, 5> GetActors()
		{
			std::array<T*, 5> actors;
			int i = 0;

			for (auto& actor : m_actors)
			{
				T* result = dynamic_cast<T*>(actor.get());

				if (result)
				{
					actors[i] = result;
					i++;
				}
			}

			return actors;
		}

		friend class Actor;

	private:
		std::list<std::unique_ptr<Actor>> m_actors;
	};
}
