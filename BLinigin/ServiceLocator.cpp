#include "SoundSystem.h"
#include "ServiceLocator.h"

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<dae::ISoundSystem>&& soundSys)
{
	if (!soundSys) m_pSoundSysInstance = std::make_unique<NullSoundSystem>();
	else m_pSoundSysInstance = std::move(soundSys);
}

std::unique_ptr<dae::ISoundSystem> dae::ServiceLocator::m_pSoundSysInstance{ std::make_unique<dae::NullSoundSystem>() };