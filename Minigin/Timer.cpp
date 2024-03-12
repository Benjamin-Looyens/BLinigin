#include "Timer.h"
#include <chrono>

using hires_clock = std::chrono::high_resolution_clock;

//hires_clock::time_point g_Before = hires_clock::now();
//float g_DeltaTime = -1.0f;
//
//float g_TimeScale = 1.0f;
//
//void dae::Timer::Update()
//{
//    auto now = hires_clock::now();
//
//    g_DeltaTime = std::chrono::duration<float, std::milli>(now - g_Before).count();
//
//    g_Before = now;
//}
//
//float dae::Timer::DeltaTime()
//{
//    return g_DeltaTime * g_TimeScale;
//}
//
//void dae::Timer::SetTimeScale(float timeScale)
//{
//    g_TimeScale = timeScale;
//}