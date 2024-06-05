#include "StageComponents.h"
#include "Timer.h"
#include "DerivedEvents.h"
#include "EnemyComponents.h"
#include <iostream>
#include "Enemy.h"
#include "GameObject.h"
#include <string>
#include "Scene.h"
#include "ResourceManager.h"
#include <json.hpp>

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

void dae::StageManagerLogic::ParseEnemies()
{
	std::string formationString{ "Formations/Formation" + std::to_string((m_CurrentStage) % (m_NumFormations) + 1) + ".json" };
	//std::string formationString{ "Formations/Formation1.json" };

	nlohmann::json* pFormation = BL_RESOURCE.LoadJSON(formationString);

	if (pFormation->contains("Bees") && pFormation->at("Bees").is_array() && !pFormation->at("Bees").empty()) {
		auto& jsonDataBees = pFormation->at("Bees");

		for (auto& jsonDataBee : jsonDataBees) {
			if (jsonDataBee.contains("X") && jsonDataBee.contains("Y")) {
				auto x = jsonDataBee.at("X");
				auto y = jsonDataBee.at("Y");

				if (!x.is_number() || !y.is_number()) {
#ifndef NDEBUG
					std::cerr << "X or Y in a bee in [" << formationString << "] was not a number!\n";
#endif 
					continue;
				}

				auto pBee = m_BeeEnemy.AddToScene(m_pParent->GetContainingScene());
				auto pBeeFormation = m_pParent->GetContainingScene()->AddGameObject();
				m_pParent->AddChild(pBeeFormation);
				pBeeFormation->SetLocalTranslation({ x, y });
				pBee->GetComponent<dae::EnemyLogic>()->SetFormationRoot(pBeeFormation);
				m_FormationPositions.push_back(pBeeFormation);
			}
			
		}
	}

	if (pFormation->contains("Butterflies") && pFormation->at("Butterflies").is_array() && !pFormation->at("Butterflies").empty()) {
		auto& jsonDataBFlies = pFormation->at("Butterflies");

		for (auto& jsonDataBFly : jsonDataBFlies) {
			if (jsonDataBFly.contains("X") && jsonDataBFly.contains("Y")) {
				auto x = jsonDataBFly.at("X");
				auto y = jsonDataBFly.at("Y");

				if (!x.is_number() || !y.is_number()) {
#ifndef NDEBUG
					std::cerr << "X or Y in a butterfly in [" << formationString << "] was not a number!\n";
#endif 
					continue;
				}

				auto pButterfly = m_ButterflyEnemy.AddToScene(m_pParent->GetContainingScene());
				auto pButterflyFormation = m_pParent->GetContainingScene()->AddGameObject();
				m_pParent->AddChild(pButterflyFormation);
				pButterflyFormation->SetLocalTranslation({ x, y });
				pButterfly->GetComponent<dae::EnemyLogic>()->SetFormationRoot(pButterflyFormation);
				m_FormationPositions.push_back(pButterflyFormation);
			}
		}
	}

	if (pFormation->contains("Bosses") && pFormation->at("Bosses").is_array() && !pFormation->at("Bosses").empty()) {
		auto& jsonDataBosses = pFormation->at("Bosses");

		for (auto& jsonDataBoss : jsonDataBosses) {
			if (jsonDataBoss.contains("X") && jsonDataBoss.contains("Y")) {
				auto x = jsonDataBoss.at("X");
				auto y = jsonDataBoss.at("Y");

				if (!x.is_number() || !y.is_number()) {
#ifndef NDEBUG
					std::cerr << "X or Y in a boss in [" << formationString << "] was not a number!\n";
#endif 
					continue;
				}

				auto pBoss = m_BossEnemy.AddToScene(m_pParent->GetContainingScene());
				auto pBossFormation = m_pParent->GetContainingScene()->AddGameObject();
				m_pParent->AddChild(pBossFormation);
				pBossFormation->SetLocalTranslation({ x, y });
				pBoss->GetComponent<dae::EnemyLogic>()->SetFormationRoot(pBossFormation);
				m_FormationPositions.push_back(pBossFormation);
			}
		}
	}
}

void dae::StageManagerLogic::NextStage()
{
	std::cout << "NEXT STAGE\n";
	m_FormationPositions.clear();

	ParseEnemies();


	// TODO : Parse information from file
	// TODO : pass formation object to the enemies directly

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
