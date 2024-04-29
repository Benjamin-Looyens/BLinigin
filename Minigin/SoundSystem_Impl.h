#pragma once
#include "SoundSystem.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>
#include <thread>

namespace dae {
	class SDL_SoundSystem final : public SoundSystem
	{
	public:
		void PlaySound(const sound_id id, const float volume) override;
		void LoadSound(const sound_id id, const bool overwriteIfExists = false) override;
		void LinkIDToFilePath(const sound_id id, const char* filePath) override { m_SoundFileLinks[id] = filePath; };
		std::thread InitQueue() override;

	protected:
		void PlaySoundFromQueue() override;
		void Quit() override { m_Continue = false; m_SoundAddedCondition.notify_one(); };

	private:
		std::map<sound_id, Mix_Chunk*> m_LoadedSounds;
		std::map<sound_id, const char*> m_SoundFileLinks;

		// Event Queue
		std::queue<std::pair<sound_id, float>> m_SoundQueue;
		std::mutex m_SoundSysMutex;
		std::condition_variable m_SoundAddedCondition;

		bool m_Continue{ true };
	};
}