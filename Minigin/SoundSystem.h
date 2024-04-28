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
	public:
		virtual ~SoundSystem() = default;
		virtual void PlaySound(const sound_id id, const float volume) = 0;
		virtual void LoadSound(const sound_id id, const bool overwriteIfExists = false) = 0;
		virtual void LinkIDToFilePath(const sound_id id, const char* filePath) = 0;
		virtual std::thread InitQueue() = 0;
		void Quit() { m_Continue = false; m_SoundAddedCondition.notify_one(); };

	protected:
		virtual void PlaySoundFromQueue() = 0;

		std::map<sound_id, const char*> m_SoundFileLinks;

		// Event Queue
		std::queue<std::pair<sound_id, float>> m_SoundQueue;
		std::mutex m_SoundSysMutex;
		std::condition_variable m_SoundAddedCondition;

		bool m_Continue{ true };
	};
}