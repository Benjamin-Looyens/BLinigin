#include "DartLogic.h"
#include "EnemyComponents.h"
#include "Timer.h"

dae::DartLogic::DartLogic(GameObject* pParent)
	: SubjectComponent(pParent)
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

		for (auto& pOverlap : pOverlaps) {
			auto pEnemyLogic = pOverlap->GetComponent<dae::EnemyLogic>();

			if (pEnemyLogic) {
				if (pEnemyLogic->Damaged()) continue;
				
				pEnemyLogic->GetHit();
				NotifyObservers(pEnemyLogic->GetDeathEvent());
				
				if (m_pParent) m_pParent->Destroy();
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
