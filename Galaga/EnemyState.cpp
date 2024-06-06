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

dae::EnemySpawnInState::EnemySpawnInState()
{
    int screenWidth;
    int screenHeight;
    SDL_GetWindowSize(SDL_GetKeyboardFocus(), &screenWidth, &screenHeight);
    float halfWidth = (static_cast<float>(screenWidth) / 2);
    float startPositionX = halfWidth + 150.0f;

    // Temporary hard coded definition
    auto bezierPath = std::make_unique<BezierPath>();

    bezierPath->AddCurve({ { startPositionX, -10.0f }, { startPositionX, -20.0f }, { startPositionX, 30.0f }, { startPositionX, 20.0f } }, 1);                    // Start straight line down
    bezierPath->AddCurve({ { startPositionX, 20.0f }, { startPositionX, 100.0f }, { halfWidth - 25.0f, 325.0f }, { halfWidth - 25.0f, 425.0f } }, 25);            // Curve to side
    bezierPath->AddCurve({ { halfWidth - 25.0f, 425.0f }, { halfWidth - 25.0f, 650.0f }, { halfWidth + 150.0f, 650.0f }, { halfWidth + 150.0f, 425.0f } }, 25);   // U-Turn
    bezierPath->AddCurve({ { halfWidth + 150.0f, 425.0f }, { halfWidth + 150.0f, 425.0f }, { 25.0f, 25.0f }, { 25.0f, 25.0f } }, 25);                             // Get into patrol location
    // TODO : Make final location dependent on formation (having set positions for each enemy in a formation)

    bezierPath->Sample(&m_Path);

    m_CurrentWayPoint = 0;
    m_StartLocation = m_Path[0];
}

dae::EnemySpawnInState::EnemySpawnInState(std::vector<glm::vec2> path, glm::vec2 startLocation)
    : m_Path{ path }, m_StartLocation{ startLocation }
{}

void dae::EnemySpawnInState::Update(EnemyLogic& enemy)
{
    float deltaTime = BL_TIMER.GetDeltaTime();
    glm::vec2 curLocation = enemy.GetLocation();

    glm::vec2 dir{ m_Path[m_CurrentWayPoint] - curLocation };
    float lenSqr{ std::powf(dir.x, 2) + std::powf(dir.y, 2) };

    if (lenSqr < EPSILON) m_CurrentWayPoint++;

    if (static_cast<size_t>(m_CurrentWayPoint) < m_Path.size()) enemy.SetLocation({ curLocation + (glm::normalize(dir) * deltaTime * enemy.GetSpawnSpeed()) });
    else enemy.SetState(std::make_unique<EnemyReturnToPatrolState>());
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

void dae::EnemyPatrolState::Update(EnemyLogic& enemy)
{
    if (m_ElapsedTime >= m_MaxTime) {
        if ((rand() % 10) == 1) enemy.SetState(std::make_unique<EnemyKamikazeState>());
        m_ElapsedTime = 0;
    }
    else {
        m_ElapsedTime += BL_TIMER.GetDeltaTime();
    }
    
    //std::cout << "[Enemy] X: " << enemy.GetLocation().x << ", Y: " << enemy.GetLocation().y << "\n";
}

void dae::EnemyPatrolState::OnEnter(EnemyLogic& enemy)
{
#ifndef NDEBUG
    std::cout << "Patrol State Entering.\n";
#endif
    constexpr float BORDER_OFFSET{ 25.0f };

    int screenWidth;
    int screenHeight;
    SDL_GetWindowSize(SDL_GetKeyboardFocus(), &screenWidth, &screenHeight);

    auto pParent = enemy.GetParent();
    auto scale = enemy.GetScale();
    auto size = pParent->GetComponent<dae::TextureComponent>()->GetTextureSize();

    // TODO : Calculate proper min and max pos using formation reference
    m_MinPos = { BORDER_OFFSET, BORDER_OFFSET };
    m_MaxPos = { static_cast<float>(screenWidth) - BORDER_OFFSET - (size.x * scale.x), static_cast<float>(screenHeight) - BORDER_OFFSET };

    //enemy.DetachFromFormation();
}

void dae::EnemyPatrolState::OnExit(EnemyLogic&)
{
#ifndef NDEBUG
    std::cout << "Patrol State Exiting.\n";
#endif
}

// TODO : Probably split Kamikaze state into 2 different states: dive & rise
void dae::EnemyKamikazeState::Update(EnemyLogic& enemy)
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
            dae::Dart dart{ 750.0f, enemy.GetLocation(), { enemy.GetXDirToClosestPlayer(), 1.0f }, "EnemyDart.png", false };
            
            // TODO : Make dart shooting system part of the dart prefab
            auto pDart = dart.AddToScene(enemy.GetParent()->GetContainingScene());
            auto parentPos = enemy.GetLocation();
            auto parentSize = enemy.GetParent()->GetComponent<dae::AnimatedTextureComponent>()->GetAppliedTextureSize() * enemy.GetScale();
            auto dartSize = pDart->GetComponent<dae::TextureComponent>()->GetTextureSize() * pDart->GetWorldScale();

            glm::vec2 dartPos{ (parentPos.x + ((parentSize.x - dartSize.x) / 2)),
                               (parentPos.y) };
            pDart->SetLocalTranslation(dartPos);

            // TODO : rework the observer assignment here to be less coupled
            auto pDartLogic = pDart->GetComponent<dae::DartLogic>();
            if (pDartLogic) {}

            m_ShootTimer = 0;
        }
    }
    else {
        if (m_DoneTimer < m_MaxDoneTimer) {
            m_DoneTimer += deltaTime;
            return;
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
            enemy.SetState(std::make_unique<EnemyReturnToPatrolState>());
        }
    }
}

void dae::EnemyKamikazeState::OnEnter(EnemyLogic& enemy)
{
#ifndef NDEBUG
    std::cout << "Kamikaze State Entering.\n";
#endif
    
    int screenWidth;
    int screenHeight;
    SDL_GetWindowSize(SDL_GetKeyboardFocus(), &screenWidth, &screenHeight);

    // TODO : Calculate kamikaze position based on player positions
    m_KamikazePosition = { static_cast<float>(screenWidth / 2), screenHeight};
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

void dae::EnemyDeathState::Update(EnemyLogic& enemy)
{
    m_ElapsedTime += BL_TIMER.GetDeltaTime();

    if (m_ElapsedTime > DEATH_TIMER) {
        enemy.Die();
    }
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

    // TODO : Play death explosion & sound effect
}

void dae::EnemyReturnToPatrolState::Update(EnemyLogic& enemy)
{
    enemy;
    enemy.SetState(std::make_unique<EnemyPatrolState>());
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
