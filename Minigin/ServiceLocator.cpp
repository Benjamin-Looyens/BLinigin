#include "SoundSystem.h"
#include "ServiceLocator.h"

dae::ISoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSysInstance;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<dae::ISoundSystem>&& soundSys)
{
	if (!soundSys) m_pSoundSysInstance = std::make_unique<NullSoundSystem>();
	else m_pSoundSysInstance = std::move(soundSys);
}

std::unique_ptr<dae::ISoundSystem> dae::ServiceLocator::m_pSoundSysInstance{ std::make_unique<dae::NullSoundSystem>() };