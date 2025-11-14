#pragma once
#include "Singleton.h"
#include <chrono>
#include <deque>

namespace dae {
	class Timer final : public Singleton<Timer> 
	{
		friend class Singleton<Timer>;

	public:
		void Init();
		void Update();
		
		inline void SetScale(float newScale) { m_TimeScale = newScale; };
		inline void SetFPSAveragingCount(unsigned int newCount) { m_FPSAveragingCount = newCount; };

		inline float GetDeltaTime() const { return m_DeltaTime; };
		inline float GetFixedTimeStep() const { return 1.0f / 60.0f * m_TimeScale; };

		std::chrono::high_resolution_clock::time_point GetPrevFrame() const { return m_PrevFrame; };

		float GetFPS() const;

	private:
		float m_TimeScale{ 1.0f };
		float m_DeltaTime{ 1.0f };

		std::chrono::high_resolution_clock::time_point m_PrevFrame;

		unsigned int m_FPSAveragingCount{ 200 };
		std::deque<float> m_DeltaTimeDeque{};
	};
}

#define BL_TIMER dae::Timer::GetInstance()