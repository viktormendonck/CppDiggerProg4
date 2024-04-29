#pragma once
#include <string>

namespace dae
{
	class SoundInitFailed{};
	class SoundLoadFailed{};

	class SoundSystem
	{
	public:
		using SoundId = size_t;
		enum class SoundType
		{
			Music,
			SoundEffect
		};
		virtual ~SoundSystem() = default;

		virtual SoundId AddSound(const std::string& filePath, SoundType type) = 0;
		virtual void PlaySound(const SoundId id, const float volume,SoundType type) = 0;
	};
};
