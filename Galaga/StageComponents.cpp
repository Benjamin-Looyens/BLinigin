#include "StageComponents.h"
#include "Timer.h"
#include "DerivedEvents.h"
#include "EnemyComponents.h"
#include <iostream>
#include "Enemy.h"
#include "GameObject.h"

dae::StageManagerLogic::StageManagerLogic(GameObject* pParent, dae::Enemy boss, dae::Enemy butterfly, dae::Enemy bee)
	: ObservingSubjectComponent(pParent), m_BossEnemy{ boss }, m_ButterflyEnemy{ butterfly }, m_BeeEnemy{ bee }
{
	// HACK : Very messy, should clean up this system
	std::vector<IObserver*> pFullObservers = m_BossEnemy.GetObservers();
	pFullObservers.push_back(this);

	m_BossEnemy.SetObservers(pFullObservers);
	m_ButterflyEnemy.SetObservers(pFullObservers);
	m_BeeEnemy.SetObservers(pFullObservers);
}

void dae::StageManagerLogic::Update()
{
	//float deltaTime = Timer::GetInstance().GetDeltaTime();
	
	if (!m_StageActive) {
		m_ElapsedTime += Timer::GetInstance().GetDeltaTime();

		if (m_ElapsedTime >= m_TimeBetweenStages) {
			m_StageActive = true;
			m_ElapsedTime = 0;
			NextStage();
		}
	}
}

void dae::StageManagerLogic::OnNotify(const ISubject* pSubject, Event event)
{
	switch (event) {
	case Event::BEE_DIED_FORMATION:
	case Event::BEE_DIED_KAMIKAZE:
	case Event::BUTTERFLY_DIED_FORMATION:
	case Event::BUTTERFLY_DIED_KAMIKAZE:
	case Event::BOSS_DIED_FORMATION:
	case Event::BOSS_DIED_KAMIKAZE:
		if (dynamic_cast<const dae::EnemyLogic*>(pSubject)) {
			std::cout << "Enemy died!\n";

			if (--m_NumberEnemies <= 0) {
				m_StageActive = false;
			}
		}

		break;
	}
}

void dae::StageManagerLogic::NextStage()
{
	std::vector<GameObject*> pEnemies;
	pEnemies.push_back(m_BossEnemy.AddToScene(m_pParent->GetContainingScene()));
	pEnemies.push_back(m_ButterflyEnemy.AddToScene(m_pParent->GetContainingScene()));
	pEnemies.push_back(m_BeeEnemy.AddToScene(m_pParent->GetContainingScene()));
	m_NumberEnemies = static_cast<int>(pEnemies.size());
}
