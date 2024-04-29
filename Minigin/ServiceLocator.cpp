#include "ServiceLocator.h"

#include "SoundSystem.h"

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem)
{
	m_pSoundSystemInstance = std::move(pSoundSystem);
}

dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystemInstance;
}
