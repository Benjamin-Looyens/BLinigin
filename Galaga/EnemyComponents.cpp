#include "EnemyComponents.h"
#include "EnemyState.h"
#include "GameObject.h"
#include "TEMP_PlayerGlobal.h"
#include "CollisionComponents.h"

dae::EnemyLogic::EnemyLogic(GameObject* pParent, float spawnSpeed, float patrolSpeed)
	: Component(pParent), m_SpawnSpeed{ spawnSpeed }, m_PatrolSpeed{ patrolSpeed }
{
	m_pSwitchState = std::make_unique<EnemySpawnInState>();
}

void dae::EnemyLogic::Update()
{
	if (m_pState.get())	m_pState->Update(*this);

	auto pCollision = m_pParent->GetComponent<dae::CollisionComponent>();
	if (pCollision) {
		auto pPlayers = PlayerLocator::GetPlayers(); 

		for (auto& pPlayer : pPlayers) {
			auto pPlayerCollision = pPlayer->GetComponent<dae::CollisionComponent>();

			if (pPlayerCollision) {
				if (pCollision->DoesOverlap(pPlayerCollision)) {
					//std::cout << "OVERLAP!\n";
					SetState(std::make_unique<dae::EnemyDeathState>());
				}
			}
		}
	}
}

void dae::EnemyLogic::LateUpdate()
{
	if (m_pSwitchState.get()) {
		if (m_pState.get()) m_pState->OnExit(*this);
		
		m_pState = std::move(m_pSwitchState);
		m_pSwitchState.reset();

		m_pState->OnEnter(*this);
	}
}

void dae::EnemyLogic::SetState(std::unique_ptr<EnemyState> pNewState)
{
	m_pSwitchState = std::move(pNewState);
}

void dae::EnemyLogic::SetLocation(glm::vec2 pos)
{
	m_pParent->SetLocalTranslation(pos);
}

glm::vec2 dae::EnemyLogic::GetLocation()
{
	auto pos = m_pParent->GetWorldPosition();
	return { pos.x, pos.y };
}

glm::vec2 dae::EnemyLogic::GetScale()
{
	auto scale = m_pParent->GetWorldScale();
	return { scale.x, scale.y };
}
