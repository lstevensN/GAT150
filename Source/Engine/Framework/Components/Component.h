#pragma once
#include "Framework/Object.h"

namespace kiko
{
	class Component : public Object
	{
	public:
		virtual void Update(float dt) = 0;

		friend class Actor;

	protected:
		class Actor* m_owner = nullptr;
	};
}
