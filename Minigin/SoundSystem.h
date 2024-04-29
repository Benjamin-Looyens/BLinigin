#pragma once
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <thread>

namespace dae {
	using sound_id = unsigned short;

	class SoundSystem
	{
		friend class Minigin; // So that it can initialize queue and quit

	public:
		virtual ~SoundSystem() = default;
		virtual void PlaySound(const sound_id id, const float volume) = 0;
		virtual void LoadSound(const sound_id id, const bool overwriteIfExists = false) = 0;
		virtual void LinkIDToFilePath(const sound_id id, const char* filePath) = 0;

	protected:
		virtual void PlaySoundFromQueue() = 0;

		virtual void Quit() = 0;

	private:
		virtual std::thread InitQueue() = 0;
	};
}