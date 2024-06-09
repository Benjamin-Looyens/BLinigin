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
#include "DerivedCommands.h"
#include "InputManager.h"
#include "Defines.h"
#include <algorithm>
#include <random>

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

	BL_INPUT.AddCommand<dae::SkipLevelCommand>(BL_KEYBOARD_F1, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent);
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

	if (m_pEnemies.size() > 0) {
		if (m_SpawnEnemyTimer < s_MAX_SPAWN_TIMER) m_SpawnEnemyTimer += BL_TIMER.GetDeltaTime(); 
		else {
			if (m_CurSpawnIndex < m_pEnemies.size()) {

				m_pEnemies[m_CurSpawnIndex]->SetEnabled(true);
				m_pEnemies[m_CurSpawnIndex]->SetVisible(true);

				m_CurSpawnIndex++;
				m_SpawnEnemyTimer = 0;
			}
		}
	}

	if (m_SkipCoolDown >= 0.0f) m_SkipCoolDown -= BL_TIMER.GetDeltaTime();
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
		auto pCasted = dynamic_cast<const dae::EnemyLogic*>(pSubject);

		if (pCasted) {
#ifndef NDEBUG
			std::cout << "Enemy died!\n";
#endif 

			m_pEnemies.erase(std::remove(begin(m_pEnemies), end(m_pEnemies), pCasted->GetParent()));
			m_CurSpawnIndex--;
			
			if (--m_NumberEnemies <= 0) {
				m_StageActive = false;

				for (auto& pFormationObject : m_pFormationPositions) {
					pFormationObject->Destroy();
				}

				m_pFormationPositions.clear();
				m_pEnemies.clear();
			}

		}
		break;
	}
}

dae::GameObject* dae::StageManagerLogic::GetRandomEnemy() const
{
	if (m_pEnemies.size() > 0) return m_pEnemies[rand() % m_pEnemies.size()];
	
	return nullptr;
}

void dae::StageManagerLogic::ParseEnemies()
{
	std::string formationString{ "Formations/Formation" + std::to_string((m_CurrentStage) % (m_NumFormations) + 1) + ".json" };

	nlohmann::json* pFormation = BL_RESOURCE.LoadJSON(formationString);

	if (!pFormation) {
		std::cerr << "Formation file could not be found!\n";
		return;
	}

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
				m_pFormationPositions.push_back(pBeeFormation);

				pBee->SetEnabled(false);
				pBee->SetVisible(false);

				m_pEnemies.push_back(pBee);
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
				m_pFormationPositions.push_back(pButterflyFormation);
				
				pButterfly->SetEnabled(false);
				pButterfly->SetVisible(false);

				m_pEnemies.push_back(pButterfly);
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
				m_pFormationPositions.push_back(pBossFormation);

				pBoss->SetEnabled(false);
				pBoss->SetVisible(false);

				m_pEnemies.push_back(pBoss);
			}
		}
	}

	//std::shuffle(begin(m_pEnemies), end(m_pEnemies), std::default_random_engine(rand()));

	m_CurSpawnIndex = 0;
}

void dae::StageManagerLogic::NextStage()
{
	std::cout << "NEXT STAGE\n";

	ParseEnemies();

	m_NumberEnemies = static_cast<int>(m_pFormationPositions.size());
	
	m_CurrentStage = (m_CurrentStage + 1) % m_NumFormations;
}

void dae::StageManagerLogic::SkipStage()
{
	if (m_SkipCoolDown >= 0.0f || !m_StageActive) return;

	for (auto pEnemy : m_pEnemies) {
		if (pEnemy != nullptr) {
			auto pLogic = pEnemy->GetComponent<dae::EnemyLogic>();
			if (pLogic) pLogic->Die();
		}
	}
	for (auto pFormationObj : m_pFormationPositions) {
		if (pFormationObj) pFormationObj->Destroy();
	}

	m_StageActive = true;
	m_ElapsedTime = 0;
	m_SkipCoolDown = s_MAX_SKIP_COOLDOWN;

	m_pFormationPositions.clear();
	m_pEnemies.clear();

	NextStage();
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
