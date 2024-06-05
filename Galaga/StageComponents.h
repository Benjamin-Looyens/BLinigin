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
		int m_NumFormations{ 3 };

		int m_NumberEnemies{};

		float m_TimeBetweenStages{ 1.0f };
		float m_ElapsedTime{};

		bool m_StageActive = false; // TODO : Implement state pattern here

		std::vector<GameObject*> m_FormationPositions;

		void NextStage();
	};

	class ScalePulseComponent final : public Component
	{
	public:
		ScalePulseComponent(GameObject* pParent, float frequency, float amplitude, float offset);
		
		void Update() override;

	private:
		float m_Frequency;
		float m_Amplitude;
		float m_Offset;
		float m_ElapsedTime{};
	};
}
