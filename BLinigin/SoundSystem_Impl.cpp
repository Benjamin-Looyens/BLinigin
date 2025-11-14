#include <iostream>
#include "SoundSystem_Impl.h"

void dae::SDL_SoundSystem::PlaySound(const sound_id id, const float volume)
{
#ifndef NDEBUG
	std::cout << "Adding sound [" << id << "] to queue...\n";
#endif 
	std::lock_guard<std::mutex> lock{ m_SoundSysMutex };
	m_SoundQueue.push({ id, volume });
	m_SoundAddedCondition.notify_one();
}

void dae::SDL_SoundSystem::LoadSound(const sound_id id)
{
#ifndef NDEBUG
	std::cout << "Adding sound [" << id << "] to queue...\n";
#endif 
	std::lock_guard<std::mutex> lock{ m_SoundSysMutex };
	m_SoundQueue.push({ id, 0.0f }); 
	m_SoundAddedCondition.notify_one();
}

bool dae::SDL_SoundSystem::InitSound(const sound_id id, const bool overwriteIfExists)
{
	auto sound = m_LoadedSounds[id];

	if (sound) {
#ifndef NDEBUG
		std::cout << "Sound [" << id << "] is already loaded!";
#endif 

		if (overwriteIfExists) {
#ifndef NDEBUG
			std::cout << " Overwriting current loaded version:\n";
#endif 
			Mix_FreeChunk(m_LoadedSounds[id]);
			m_LoadedSounds.erase(id);

			return InitSound(id);
		}
		else std::cout << "\n";
	}
	else {
#ifndef NDEBUG
		std::cout << "Loading sound [" << id << "]...\n";
#endif 

		const auto& soundStr{ m_SoundFileLinks[id] };

		if (!soundStr.c_str()) {
			std::cerr << "No sound was found matching the id [" << id << "]!\n";
			return false;
		}

		sound = Mix_LoadWAV(soundStr.c_str());

		if (!sound) {
			std::cerr << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << "\n";
			return false;
		}
		else {
			m_LoadedSounds[id] = sound;
			return true;
		}
	}

	return false;
}

std::thread* dae::SDL_SoundSystem::InitQueue()
{
	std::thread* soundThread{ new std::thread{ &SDL_SoundSystem::PlaySoundFromQueue, this } };

	return soundThread;
}

void dae::SDL_SoundSystem::PlaySoundFromQueue()
{
	while (m_Continue) {
		std::unique_lock<std::mutex> lock{ m_SoundSysMutex };
		m_SoundAddedCondition.wait(lock, [this] { return !m_SoundQueue.empty() || !m_Continue; });
		
		if (m_SoundQueue.empty()) break;

		auto nextSound = m_SoundQueue.front();
		m_SoundQueue.pop();
		
		lock.unlock();
		
		sound_id id{ std::get<sound_id>(nextSound) };
		float volume{ std::get<float>(nextSound) };
		bool play{ volume > 0.0f };
		auto sound = m_LoadedSounds[id];
		
		// See if sound is loaded
		if (!sound) {
			if (InitSound(id)) sound = m_LoadedSounds[id]; // reload now that it *is* loaded
		}

		if (sound && play) {
#ifndef NDEBUG
			std::cout << "Playing sound [" << id << "] at volume " << volume * 100 << "...\n";
#endif 
			Mix_Volume(-1, static_cast<int>(static_cast<float>(MIX_MAX_VOLUME) * volume)); 
			Mix_PlayChannel(-1, sound, 0);
		}
	}
}
