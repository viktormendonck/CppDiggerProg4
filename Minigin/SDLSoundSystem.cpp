#include "SDLSoundSystem.h"
#include <cassert>
#include <SDL_mixer.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace dae
{
	class SDLSoundSystem::SDLSoundSystemImpl
	{
		public:
		explicit SDLSoundSystemImpl();
		~SDLSoundSystemImpl();

		SoundId AddSound(const std::string& filePath, SoundType type);
		void PlaySound(const SoundId id, const float volume, SoundType type);
	private:
		void PlaySoundThread();
		std::vector<Mix_Chunk*> m_pSfx;
		std::vector<Mix_Music*> m_pMusic;
		std::queue<PlaySoundData> m_SoundQueue;

		std::mutex m_Mutex;
		std::jthread m_SoundThread;
		std::condition_variable m_ConditionVariable;
		bool m_IsRunning = true;
	};
	//impl

	SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl()
	{
		Mix_Init(MIX_INIT_MP3);
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
		{
			throw SoundInitFailed{};
		}
		m_SoundThread = std::jthread([this] { SDLSoundSystemImpl::PlaySoundThread(); });
	}

	SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
	{
		m_IsRunning = false;
		m_ConditionVariable.notify_all();
		m_SoundThread.join();
		for (auto pChunk : m_pSfx)
		{
			Mix_FreeChunk(pChunk);
			pChunk = nullptr;
		}
		for (auto pMusic : m_pMusic)
		{
			Mix_FreeMusic(pMusic);
			pMusic = nullptr;
		}
		Mix_CloseAudio();
		Mix_Quit();
	}

	SoundSystem::SoundId SDLSoundSystem::SDLSoundSystemImpl::AddSound(const std::string& filePath, SoundType type)
	{
		std::lock_guard lock(m_Mutex);
		switch (type)
		{
		case SoundType::Music:
			{
			Mix_Music* pMusic = Mix_LoadMUS(filePath.c_str());
			if (pMusic == nullptr)
			{
				throw SoundLoadFailed();
			}
			m_pMusic.emplace_back(pMusic);
			return m_pMusic.size() - 1;
			}
		case SoundType::SoundEffect:
			{
			Mix_Chunk* pSfx = Mix_LoadWAV(filePath.c_str());
			if (pSfx == nullptr)
			{
				throw SoundLoadFailed();
			}
			m_pSfx.emplace_back(pSfx);
			return m_pSfx.size() - 1;
			}
		}
		throw SoundLoadFailed();
	}

	void SDLSoundSystem::SDLSoundSystemImpl::PlaySound(const SoundId id, const float volume, SoundType type)
	{
		std::lock_guard lock(m_Mutex);
		m_SoundQueue.push({ type,id,volume });
		m_ConditionVariable.notify_all();
	}

	void SDLSoundSystem::SDLSoundSystemImpl::PlaySoundThread()
	{
		while (m_IsRunning)
		{
			std::unique_lock lock(m_Mutex);
			m_ConditionVariable.wait(lock); //instead of constantly checking the queue, we wait until we are notified

			while (!m_SoundQueue.empty())
			{
				
				const PlaySoundData& currentRequest{m_SoundQueue.front()};
				m_SoundQueue.pop();
				

				switch (currentRequest.type)
				{
					case SoundType::Music:
						Mix_VolumeMusic(static_cast<int>(currentRequest.volume * MIX_MAX_VOLUME));
						Mix_PlayMusic(m_pMusic.at(currentRequest.id),-1);
						break;
					case SoundType::SoundEffect:
						Mix_Volume(-1,static_cast<int>(currentRequest.volume * MIX_MAX_VOLUME));
						Mix_PlayChannel(-1, m_pSfx[currentRequest.id], 0);
						break;
				}
			}
		}
	}

	//pimpl
	SDLSoundSystem::SDLSoundSystem()
	: m_pImpl(std::make_unique<SDLSoundSystemImpl>())
	{
	}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::PlaySound(const SoundId id, const float volume, SoundType type)
	{
		m_pImpl->PlaySound(id, volume,type);
	}

	SoundSystem::SoundId SDLSoundSystem::AddSound(const std::string& filePath, SoundType type)
	{
		return m_pImpl->AddSound(filePath,type);
	}
}
