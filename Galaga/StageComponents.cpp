#include "StageComponents.h"
#include "Timer.h"
#include "DerivedEvents.h"
#include "EnemyComponents.h"
#include <iostream>
#include "Enemy.h"
#include "GameObject.h"
#include <string>
#include "Scene.h"

// TODO : Fix Math Defines
#define _USE_MATH_DEFINES
#include <math.h>

dae::StageManagerLogic::StageManagerLogic(GameObject* pParent, dae::Enemy boss, dae::Enemy butterfly, dae::Enemy bee)
	: ObservingSubjectComponent(pParent), m_BossEnemy{ boss }, m_ButterflyEnemy{ butterfly }, m_BeeEnemy{ bee }
{
	// HACK : Very messy, should clean up this system
	std::vector<IObserver*> pFullObservers = m_BossEnemy.GetObservers();
	pFullObservers.push_back(this);

	m_BossEnemy.SetObservers(pFullObservers);
	m_ButterflyEnemy.SetObservers(pFullObservers);
	m_BeeEnemy.SetObservers(pFullObservers);

	m_pParent->AddComponent<dae::ScalePulseComponent>(0.1f, 0.25f, 1.25f);
}

void dae::StageManagerLogic::Update()
{
	//float deltaTime = BL_TIMER.GetDeltaTime();
	
	if (!m_StageActive) {
		m_ElapsedTime += BL_TIMER.GetDeltaTime();

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

				for (auto& pFormationObject : m_FormationPositions) {
					pFormationObject->Destroy();
				}
			}
		}

		break;
	}
}

void dae::StageManagerLogic::NextStage()
{
	std::cout << "NEXT STAGE\n";
	//std::string formationString{ "Formation" + std::to_string((m_CurrentStage + 1) % m_NumFormations) + ".json" };

	m_FormationPositions.clear();

	std::cout << "[StageManager Formation] X: " << m_pParent->GetWorldPosition().x << ", Y: " << m_pParent->GetWorldPosition().y << "\n";

	// TODO : Parse information from file
	// TODO : pass formation object to the enemies directly
	auto pBoss = m_BossEnemy.AddToScene(m_pParent->GetContainingScene());
	auto pBossFormation = m_pParent->GetContainingScene()->AddGameObject();
	m_pParent->AddChild(pBossFormation);
	pBossFormation->SetLocalTranslation({ 0, 50 });
	std::cout << "[BossForm] X: " << pBossFormation->GetWorldPosition().x << ", Y: " << pBossFormation->GetWorldPosition().y << "\n";
	pBoss->GetComponent<dae::EnemyLogic>()->SetFormationRoot(pBossFormation);
	m_FormationPositions.push_back(pBossFormation);

	auto pButterfly = m_ButterflyEnemy.AddToScene(m_pParent->GetContainingScene());
	auto pButterflyFormation = m_pParent->GetContainingScene()->AddGameObject();
	m_pParent->AddChild(pButterflyFormation);
	pButterflyFormation->SetLocalTranslation({ 0, 100 });
	std::cout << "[BFlyForm] X: " << pButterflyFormation->GetWorldPosition().x << ", Y: " << pButterflyFormation->GetWorldPosition().y << "\n";
	pButterfly->GetComponent<dae::EnemyLogic>()->SetFormationRoot(pButterflyFormation);
	m_FormationPositions.push_back(pButterflyFormation);

	auto pButterfly2 = m_ButterflyEnemy.AddToScene(m_pParent->GetContainingScene());
	auto pButterflyFormation2 = m_pParent->GetContainingScene()->AddGameObject();
	m_pParent->AddChild(pButterflyFormation2);
	pButterflyFormation2->SetLocalTranslation({ 30, 100 });
	std::cout << "[BFlyForm2] X: " << pButterflyFormation2->GetWorldPosition().x << ", Y: " << pButterflyFormation2->GetWorldPosition().y << "\n";
	pButterfly2->GetComponent<dae::EnemyLogic>()->SetFormationRoot(pButterflyFormation2);
	m_FormationPositions.push_back(pButterflyFormation2);

	auto pBee = m_BeeEnemy.AddToScene(m_pParent->GetContainingScene());
	auto pBeeFormation = m_pParent->GetContainingScene()->AddGameObject();
	m_pParent->AddChild(pBeeFormation);
	pBeeFormation->SetLocalTranslation({ 0, 150 });
	std::cout << "[BeeForm] X: " << pBeeFormation->GetWorldPosition().x << ", Y: " << pBeeFormation->GetWorldPosition().y << "\n";
	pBee->GetComponent<dae::EnemyLogic>()->SetFormationRoot(pBeeFormation);
	m_FormationPositions.push_back(pBeeFormation);

	m_NumberEnemies = static_cast<int>(m_FormationPositions.size());
	

	m_CurrentStage = (m_CurrentStage + 1) % m_NumFormations;
}

dae::ScalePulseComponent::ScalePulseComponent(GameObject* pParent, float frequency = 0.1f, float amplitude = 0.5f, float offset = 1.5f)
	: Component(pParent), m_Frequency{ frequency }, m_Amplitude{ amplitude }, m_Offset{ offset }
{}

void dae::ScalePulseComponent::Update()
{
	//auto parentScale = m_pParent->GetLocalScale();

	m_ElapsedTime += BL_TIMER.GetDeltaTime();
	//m_ElapsedTime = std::fmod(m_ElapsedTime, 1.0f);

	float sinVal = (m_Amplitude * static_cast<float>(std::cos(2 * M_PI * m_Frequency * m_ElapsedTime))) + m_Offset;
	m_pParent->SetLocalScale((glm::vec2{ 1.0f, 1.0f } * sinVal));
	
	//std::cout << "SinVal: " << sinVal << "\n";
}
