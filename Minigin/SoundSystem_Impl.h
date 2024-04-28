#pragma once
#include "SoundSystem.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

namespace dae {
	class SDL_SoundSystem final : public SoundSystem
	{
	public:
		void PlaySound(const sound_id id, const float volume) override;
		void LoadSound(const sound_id id, const bool overwriteIfExists = false) override;
	
	private:
		std::map<sound_id, Mix_Chunk*> m_LoadedSounds;

		static std::string IDToStr(const sound_id id);
	};


}