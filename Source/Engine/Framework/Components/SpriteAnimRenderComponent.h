#pragma once
#include "SpriteComponent.h"

namespace kiko
{
	struct AnimSequence
	{
		std::string name;

		float fps = 0;
		int numColumns = 0;
		int numRows = 0;

		int startFrame = 0;
		int endFrame = 0;

		bool loop = true;

		std::shared_ptr<Texture> texture;
	};

	class SpriteAnimRenderComponent : public SpriteComponent
	{
	public:
		CLASS_DECLARATION(SpriteAnimRenderComponent);

		void Update(float dt) override;
		bool Initialize() override;

		void SetSequence(const std::string& name);
		void UpdateSource();

	public:
		int frame = 0;
		float frameTimer = 0;

	private:
		std::string defaultSequenceName;
		std::map<std::string, AnimSequence> m_sequences;
		AnimSequence* m_sequence = nullptr;
	};
}
