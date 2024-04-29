#pragma once
#include <memory>

#include "SoundSystem.h"

namespace dae
{
	struct PlaySoundData
	{
		SoundSystem::SoundType type;
		SoundSystem::SoundId id;
		float volume;
	};
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		explicit SDLSoundSystem();
		~SDLSoundSystem() override;

		//delete rule of 5
		SDLSoundSystem(const SDLSoundSystem& other) = delete;
		SDLSoundSystem(SDLSoundSystem&& other) = delete;
		SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&& other) = delete;

		SoundId AddSound(const std::string& filePath, SoundType type) override;
		void PlaySound(const SoundId id, const float volume, SoundType type) override;
	private:
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_pImpl;
	};
}
