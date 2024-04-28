#include "ServiceLocator.h"
#include "SoundSystem.h"

dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSysInstance;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<dae::SoundSystem>&& soundSys)
{
	m_pSoundSysInstance = std::move(soundSys);
}

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_pSoundSysInstance{ std::make_unique<dae::NullSoundSystem>() };