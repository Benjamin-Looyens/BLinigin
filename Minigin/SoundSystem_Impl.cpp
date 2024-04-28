#include <iostream>
#include "SoundSystem_Impl.h"

void dae::SDL_SoundSystem::PlaySound(const sound_id id, const float volume)
{
	auto sound = m_LoadedSounds[id];

	if (!sound) {
		LoadSound(id);
		sound = m_LoadedSounds[id]; // reload
	}

	std::cout << "Playing sound [" << id << "]...\n";
    Mix_Volume(-1, static_cast<int>(static_cast<float>(MIX_MAX_VOLUME) * volume));
    Mix_PlayChannel(-1, sound, 0);
}

// WARNING: overwriting will cancel any currently playing sounds of the same id!
void dae::SDL_SoundSystem::LoadSound(const sound_id id, const bool overwriteIfExists)
{
	auto sound = m_LoadedSounds[id];

	if (sound) {
		std::cout << "Sound [" << id << "] is already loaded!";

		if (overwriteIfExists) {
			std::cout << " Overwriting current loaded version:\n";
			Mix_FreeChunk(m_LoadedSounds[id]);
			m_LoadedSounds.erase(id);

			LoadSound(id);
		}
		else std::cout << "\n";
	}
	else {
		std::cout << "Loading sound [" << id << "]...\n";

		std::string soundStr{ IDToStr(id) };
		if (soundStr.empty()) {
			std::cerr << "No sound was found matching the id [" << id << "]!\n";
			return;
		}

		sound = Mix_LoadWAV(soundStr.c_str());

		if (!sound) {
			std::cerr << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << "\n";
			return;
		}
		else {
			m_LoadedSounds[id] = sound;
		}
	}
}

// TODO : somehow make the following not defined in the library but reliant on engine user decision
std::string dae::SDL_SoundSystem::IDToStr(const sound_id id)
{
	std::string str{};

	switch (id)
	{
	case 0:
		str = "../Data/Sound/01. Stage Intro.mp3";
		break;
	}

	return str;
}
