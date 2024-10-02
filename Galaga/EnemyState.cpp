#include <iostream>
#include "EnemyState.h"
#include "EnemyComponents.h"
#include "GameObject.h"
#include "Timer.h"
#include <SDL.h>
#include "DerivedComponents.h"
#include "TextureComponents.h"
#include "Dart.h"
#include "DartLogic.h"
#include "Renderer.h"

dae::EnemySpawnInState::EnemySpawnInState()
{
    glm::vec2 windowSize = BL_RENDERER.GetWindowSize();
    float halfWidth = (static_cast<float>(windowSize.x) / 2);
    float halfHeight = (static_cast<float>(windowSize.y) / 2);

    float dirModifier{ 1.0f };
    //if (rand() % 2) dirModifier = -1.0f;

    float startPositionX = halfWidth + (150.0f * dirModifier);

    // Temporary hard coded definition
    auto bezierPath = std::make_unique<BezierPath>();

    bezierPath->AddCurve({ { startPositionX, -10.0f }, { startPositionX, -20.0f }, { startPositionX, 30.0f }, { startPositionX, 20.0f } }, 1);                              // Start straight line down
    bezierPath->AddCurve({ { startPositionX, 20.0f }, { startPositionX, 100.0f }, { halfWidth - 25.0f, 225.0f }, { halfWidth - 25.0f, 325.0f } }, 25);                      // Curve to side
    bezierPath->AddCurve({ { halfWidth - (25.0f), 325.0f }, { halfWidth - (25.0f), 550.0f }, { halfWidth + (150.0f), 550.0f }, { halfWidth + (150.0f), 325.0f } }, 25);     // U-Turn
    bezierPath->AddCurve({ { halfWidth + (150.0f), 325.0f }, { halfWidth + (150.0f), 325.0f }, { halfWidth, halfHeight / 2 }, { halfWidth, halfHeight / 2 } }, 25);         // Go to center

    // TODO : Make state where the enemy goes to their formation state (from the middle)

    bezierPath->Sample(&m_Path);

    m_CurrentWayPoint = 0;
    m_StartLocation = m_Path[0];
}

dae::EnemySpawnInState::EnemySpawnInState(std::vector<glm::vec2> path, glm::vec2 startLocation)
    : m_Path{ path }, m_StartLocation{ startLocation }
{}

std::unique_ptr<dae::EnemyState> dae::EnemySpawnInState::Update(EnemyLogic& enemy)
{
    float deltaTime = BL_TIMER.GetDeltaTime();
    glm::vec2 curLocation = enemy.GetLocation();

    glm::vec2 dir{ m_Path[m_CurrentWayPoint] - curLocation };
    float lenSqr{ std::powf(dir.x, 2) + std::powf(dir.y, 2) };

    if (lenSqr < EPSILON) m_CurrentWayPoint++;

    if (static_cast<size_t>(m_CurrentWayPoint) < m_Path.size()) enemy.SetLocation({ curLocation + (glm::normalize(dir) * deltaTime * enemy.GetSpawnSpeed()) });
    else return std::make_unique<EnemyReturnToPatrolState>();

    return nullptr;
}

void dae::EnemySpawnInState::OnEnter(EnemyLogic& enemy)
{
#ifndef NDEBUG
    std::cout << "SpawnIn State Entering.\n";
#endif

    enemy.SetLocation({ m_StartLocation.x, m_StartLocation.y - 10.0f });
}

void dae::EnemySpawnInState::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "SpawnIn State Exiting.\n";
#endif
}

std::unique_ptr<dae::EnemyState> dae::EnemyPatrolState::Update(EnemyLogic&)
{
    if (m_ElapsedTime >= m_MaxTime) {
        m_ElapsedTime = 0;
        if ((rand() % 100) < s_PERCENT_CHANCE_TO_KAMIKAZE) return std::make_unique<EnemyKamikazeState>();
    }
    else {
        m_ElapsedTime += BL_TIMER.GetDeltaTime();
    }
    
    //std::cout << "[Enemy] X: " << enemy.GetLocation().x << ", Y: " << enemy.GetLocation().y << "\n";

    return nullptr;
}

void dae::EnemyPatrolState::OnEnter(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Patrol State Entering.\n";
#endif
}

void dae::EnemyPatrolState::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Patrol State Exiting.\n";
#endif
}

// TODO : Probably split Kamikaze state into 2 different states: dive & rise
std::unique_ptr<dae::EnemyState> dae::EnemyKamikazeState::Update(EnemyLogic& enemy)
{
    float deltaTime = BL_TIMER.GetDeltaTime();
    glm::vec2 curLocation = enemy.GetLocation();
    
    glm::vec2 dir{};
    float lenSqr{};

    // TODO : Rework this
    if (m_NotDone) {
        dir = { m_KamikazePosition - curLocation };
        
        m_ShootTimer += deltaTime;
        if (m_ShootTimer > s_TIME_TO_SHOOT) {
            enemy.Fire();

            m_ShootTimer = 0;
        }
    }
    else {
        if (m_DoneTimer < m_MaxDoneTimer) {
            m_DoneTimer += deltaTime;
            return nullptr;
        }

        dir = { m_StartPosition - curLocation };
    }
    
    lenSqr = std::powf(dir.x, 2) + std::powf(dir.y, 2);

    if (lenSqr > EPSILON) {
        enemy.SetLocation({ curLocation + (glm::normalize(dir) * deltaTime * enemy.GetSpawnSpeed()) });
    }
    else {
        if (m_NotDone) {
            m_NotDone = false;

            auto newPos = enemy.GetLocation();

            enemy.SetLocation({ m_StartPosition.x, newPos.y });
        }
        else {
            return std::make_unique<EnemyReturnToPatrolState>();
        }
    }

    return nullptr;
}

void dae::EnemyKamikazeState::OnEnter(EnemyLogic& enemy)
{
#ifndef NDEBUG
    std::cout << "Kamikaze State Entering.\n";
#endif
    
    glm::vec2 windowSize = BL_RENDERER.GetWindowSize();

    // TODO : Calculate kamikaze position based on player positions
    m_KamikazePosition = { static_cast<float>(windowSize.x / 2), windowSize.y};
    m_StartPosition = enemy.GetLocation();

    enemy.DetachFromFormation();

    m_ShootTimer = s_TIME_TO_SHOOT;
}

void dae::EnemyKamikazeState::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Kamikaze State Exiting.\n";
#endif
}

std::unique_ptr<dae::EnemyState> dae::EnemyDeathState::Update(EnemyLogic& enemy)
{
    m_ElapsedTime += BL_TIMER.GetDeltaTime();

    if (m_ElapsedTime > DEATH_TIMER) {
        enemy.Die();
    }

    return nullptr;
}

void dae::EnemyDeathState::OnEnter(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Death State Entering.\n";
#endif
}

void dae::EnemyDeathState::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Death State Exiting.\n";
#endif
}

std::unique_ptr<dae::EnemyState> dae::EnemyReturnToPatrolState::Update(EnemyLogic&)
{
    return std::make_unique<EnemyPatrolState>();
}

void dae::EnemyReturnToPatrolState::OnEnter(EnemyLogic& enemy)
{
#ifndef NDEBUG
    std::cout << "ReturnToPatrol State Entering.\n";
#endif

    enemy.SyncToFormation();
}

void dae::EnemyReturnToPatrolState::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "ReturnToPatrol State Exiting.\n";
#endif
}
