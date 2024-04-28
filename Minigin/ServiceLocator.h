#pragma once
#include <memory>
#include "SoundSystem.h"
#include <iostream>

namespace dae {
	class NullSoundSystem final : public SoundSystem
	{
		void PlaySound(const sound_id, const float) override { std::cerr << "SoundSystem is uninitialized!\n"; };
		void LoadSound(const sound_id, const bool)  override { std::cerr << "SoundSystem is uninitialized!\n"; };
	};

	class ServiceLocator final
	{
	public:
		static dae::SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<dae::SoundSystem>&& soundSys);

	private:
		static std::unique_ptr<dae::SoundSystem> m_pSoundSysInstance;
	};
}
