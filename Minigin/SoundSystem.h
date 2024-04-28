#pragma once

namespace dae {
	using sound_id = unsigned short;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void PlaySound(const sound_id id, const float volume) = 0;
		virtual void LoadSound(const sound_id id, const bool overwriteIfExists = false) = 0;
	};
}