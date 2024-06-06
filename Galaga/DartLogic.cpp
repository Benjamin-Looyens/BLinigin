#include "DartLogic.h"
#include "EnemyComponents.h"
#include "PlayerComponents.h"
#include "Timer.h"

dae::DartLogic::DartLogic(GameObject* pParent, bool isPlayerDart)
	: SubjectComponent(pParent), m_IsPlayerDart{ isPlayerDart }
{
	if (pParent) {
		m_pParentCollision = pParent->GetComponent<dae::CollisionComponent>(); 
		if (m_pParentCollision == nullptr) Remove();
	}
}

void dae::DartLogic::Update()
{
	if (m_pParentCollision) {
		auto pOverlaps = m_pParentCollision->GetAllOverlappingObjects();

		if (m_IsPlayerDart) {
			for (auto& pOverlap : pOverlaps) {
				auto pEnemyLogic = pOverlap->GetComponent<dae::EnemyLogic>();

				if (pEnemyLogic) {
					if (pEnemyLogic->Damaged()) continue;

					pEnemyLogic->GetHit();
					NotifyObservers(pEnemyLogic->GetDeathEvent());

					if (m_pParent) m_pParent->Destroy();
					break;
				}
			}
		}
		else {
			for (auto& pOverlap : pOverlaps) {
				auto pPlayerLogic = pOverlap->GetComponent<dae::PlayerLogic>();

				if (pPlayerLogic) {
					pPlayerLogic->Die();

					if (m_pParent) m_pParent->Destroy();
					break;
				}
			}
		}
	}

	const float& deltaTime = BL_TIMER.GetDeltaTime();

	m_ElapsedTime += deltaTime;
	if (m_ElapsedTime > MAX_LIFETIME) {
#ifndef NDEBUG
		std::cout << "Dart Destroyed.\n";
#endif 
		if (m_pParent) m_pParent->Destroy();
	}
}
