#include "SoundSystem.h"
#include "ServiceLocator.h"

dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSysInstance;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<dae::SoundSystem>&& soundSys)
{
	m_pSoundSysInstance = soundSys == nullptr ? std::make_unique<NullSoundSystem>() : std::move(soundSys);
}

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_pSoundSysInstance{ std::make_unique<dae::NullSoundSystem>() };