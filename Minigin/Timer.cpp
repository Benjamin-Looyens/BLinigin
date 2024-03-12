#include "Timer.h"
#include <chrono>
#include <deque>
#include <numeric>
#include <algorithm>

void dae::Timer::Init()
{
	m_PrevFrame = std::chrono::high_resolution_clock::now();
}

void dae::Timer::Update()
{
	auto curFrame = std::chrono::high_resolution_clock::now();

	m_DeltaTime = std::chrono::duration<float, std::ratio<1>>(curFrame - m_PrevFrame).count();

    m_DeltaTimeDeque.push_front(m_DeltaTime);
    while (m_DeltaTimeDeque.size() > m_FPSAveragingCount) m_DeltaTimeDeque.pop_back();

	m_PrevFrame = curFrame;
}

float dae::Timer::GetFPS() const
{   
    const float totalDeltaTime{ std::accumulate(m_DeltaTimeDeque.begin(), m_DeltaTimeDeque.end(), 0.f) };
    const float averagedFPS{ 1.0f / (totalDeltaTime / m_DeltaTimeDeque.size()) };
    
    return averagedFPS;
}
