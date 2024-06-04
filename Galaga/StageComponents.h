#pragma once
#include "Component.h"
#include "Enemy.h"
#include "ObserverComponents.h"
#include <vector>

namespace dae {
	class GameObject;

	class StageManagerLogic final : public ObservingSubjectComponent
	{
	public:
		StageManagerLogic(GameObject* pParent, dae::Enemy boss, dae::Enemy butterfly, dae::Enemy bee);

		void Update() override;
		
		void OnNotify(const ISubject* pSubject, Event event) override;

	private:
		dae::Enemy m_BossEnemy;
		dae::Enemy m_BeeEnemy;
		dae::Enemy m_ButterflyEnemy;

		int m_CurrentStage{};
		int m_NumberEnemies{};

		float m_TimeBetweenStages{ 1.0f };
		float m_ElapsedTime{};

		bool m_StageActive = false;

		void NextStage();
	};
}
