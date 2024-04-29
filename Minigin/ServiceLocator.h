#pragma once
#include <memory>

#include "SDLSoundSystem.h"


namespace dae
{
	class SoundSystem;

	class ServiceLocator final
	{
	public:
		ServiceLocator() = delete;
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem);
		static SoundSystem& GetSoundSystem();
	private:
		inline static std::unique_ptr<SoundSystem> m_pSoundSystemInstance{};
	};
}
