#pragma once
#include <memory>
#include "SoundSystem.h"
#include <iostream>

namespace dae {
	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void PlaySound(const sound_id, const float) override { std::cerr << "Unable to play sound because the SoundSystem is uninitialized!\n"; };
		void LinkIDToFilePath(const sound_id, const char*) override { std::cerr << "Unable to add sound-file link because the SoundSystem is uninitialized!\n"; };
		std::thread* InitQueue() override { std::cerr << "Unable to start queue because the SoundSystem is uninitialized!\n"; return nullptr; };

	private:
		bool LoadSound(const sound_id, const bool) override { std::cerr << "Unable to load sound because the SoundSystem is uninitialized!\n"; return 0; };
		void PlaySoundFromQueue() override { std::cerr << "Unable to play sound from queue because the SoundSystem is uninitialized!\n"; };
		void Quit() override { std::cerr << "Unable to quit because SoundSystem is uninitialized!\n"; };
	};

	class ServiceLocator final
	{
	public:
		static dae::ISoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<dae::ISoundSystem>&& soundSys);

	private:
		static std::unique_ptr<dae::ISoundSystem> m_pSoundSysInstance;
	};
}
