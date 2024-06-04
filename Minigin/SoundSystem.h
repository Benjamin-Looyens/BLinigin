#pragma once
#include <thread>

namespace dae {
	using sound_id = unsigned short;

	class ISoundSystem
	{
		friend class Minigin; // So that it can initialize queue and quit

	public:
		virtual ~ISoundSystem() = default;
		virtual void PlaySound(const sound_id id, const float volume) = 0;
		virtual void LoadSound(const sound_id id) = 0;
		virtual void LinkIDToFilePath(const sound_id id, const char* filePath) = 0;

	private:
		virtual bool InitSound(const sound_id id, const bool overwriteIfExists = false) = 0;
		virtual void PlaySoundFromQueue() = 0;
		virtual std::thread* InitQueue() = 0;
		virtual void Quit() = 0;
	};
}