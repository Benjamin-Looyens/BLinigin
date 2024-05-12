#include <iostream>
#include "EnemyState.h"
#include "EnemyComponents.h"
#include "GameObject.h"
#include "Timer.h"
#include <SDL.h>

dae::SpawnIn::SpawnIn()
{
    int width, height;
    SDL_GetWindowSize(SDL_GetKeyboardFocus(), &width, &height);
    float halfWidth = (static_cast<float>(width) / 2);
    float startPositionX = halfWidth + 50.0f;

    // Temporary hard coded definition
    
    //m_Path = {
    //    { halfWidth, 0.0f, 0.0f },
    //    { halfWidth, 50.0f, 0.0f },
    //    { halfWidth + 50.0f, 150.0f, 0.0f },
    //    { halfWidth + 100.0f, 250.0f, 0.0f },
    //};
    //
    //m_EndLocation = { 100.0f, 100.0f, 0.0f };

    m_Path = {};

    auto path = std::make_unique<BezierPath>();

    //path->AddCurve({ { halfWidth, 10.0f }, { halfWidth, 0.0f }, { halfWidth, 310.0f }, { halfWidth, 300.0f } }, 1);
    path->AddCurve({ { startPositionX, -10.0f }, { startPositionX, -20.0f }, { startPositionX, 30.0f }, { startPositionX, 20.0f } }, 5);
    path->AddCurve({ { startPositionX, 20.0f }, { startPositionX, 30.0f }, { halfWidth - 25.0f, 325.0f }, { halfWidth - 25.0f, 325.0f } }, 5);
    
    path->Sample(&m_Path);

    m_CurrentWayPoint = 0;
    //m_StartLocation = { startPositionX, 500.0f };
    m_StartLocation = m_Path[0];
}

dae::SpawnIn::SpawnIn(std::vector<glm::vec2> path, glm::vec2 endLocation)
    : m_Path{ path }, m_EndLocation{ endLocation }
{}

void dae::SpawnIn::Update(EnemyLogic& enemy)
{
    std::cout << "\n";

    auto dir = m_Path[m_CurrentWayPoint] - enemy.GetLocation();
    float lenSqr{ std::powf(dir.x, 2) + std::powf(dir.y, 2) };

    if (lenSqr < EPSILON)
        m_CurrentWayPoint++;

    if (m_CurrentWayPoint < m_Path.size()) {
        glm::vec2 dist = m_Path[m_CurrentWayPoint] - enemy.GetLocation();
        enemy.SetLocation({ enemy.GetLocation() + (glm::normalize(dist) * Timer::GetInstance().GetDeltaTime() * enemy.GetSpeed()), 0.0f });
    }
    else {
        enemy.SetState(std::make_unique<Patrol>());
        return;
    }
        
    //std::cout << "DeltaTime: " << deltaTime << "\n";
    //std::cout << "Enemy Prev Location: [" << curPos.x << ", " << curPos.y << "]\n";
    std::cout << "Enemy Location: [" << enemy.GetLocation().x << ", " << enemy.GetLocation().y << "]\n";
    std::cout << "Path[curWayPoint]: [" << m_Path[m_CurrentWayPoint].x << ", " << m_Path[m_CurrentWayPoint].y << "]\n";
    std::cout << "CurWayPoint: [" << m_CurrentWayPoint << "]\n";
    //std::cout << "Dir: [" << dir.x << ", " << dir.y << "]\n";
    //std::cout << "Len: [" << len << "]\n";
}

void dae::SpawnIn::OnEnter(EnemyLogic& enemy)
{
#ifndef NDEBUG
    std::cout << "SpawnIn State Entering.\n";
#endif
    enemy;
    //enemy.SetLocation({ m_StartLocation, 0.0f });
    //std::cout << "Start Location: [" << m_StartLocation.x << ", " << m_StartLocation.y << "]\n";
}

void dae::SpawnIn::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "SpawnIn State Exiting.\n";
#endif
}

void dae::Patrol::Update(EnemyLogic& enemy)
{
    enemy;
}

void dae::Patrol::OnEnter(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Patrol State Entering.\n";
#endif
}

void dae::Patrol::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Patrol State Exiting.\n";
#endif
}

void dae::Kamikaze::Update(EnemyLogic& enemy)
{
    enemy;
}

void dae::Kamikaze::OnEnter(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Kamikaze State Entering.\n";
#endif
}

void dae::Kamikaze::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Kamikaze State Exiting.\n";
#endif
}
