#pragma once
#include "Component.h"
#include "Enemy.h"
#include "ObserverComponents.h"
#include <vector>

namespace dae {
	class GameObject;

	class StageManagerLogic final : public ObservingSubjectComponent
	{
		friend class SkipLevelCommand;

	public:
		StageManagerLogic(GameObject* pParent, dae::Enemy boss, dae::Enemy butterfly, dae::Enemy bee);

		void Update() override;
		
		void OnNotify(const ISubject* pSubject, Event event) override;

		GameObject* GetRandomEnemy() const;

	private:
		dae::Enemy m_BossEnemy;
		dae::Enemy m_BeeEnemy;
		dae::Enemy m_ButterflyEnemy;

		int m_CurrentStage{};
		int m_NumFormations{ 3 };

		int m_NumberEnemies{};

		float m_TimeBetweenStages{ 1.0f };
		float m_ElapsedTime{};

		float m_SkipCoolDown{};
		static constexpr float s_MAX_SKIP_COOLDOWN = 1.0f;

		bool m_StageActive = false; // TODO : Implement state pattern here

		size_t m_CurSpawnIndex;
		float m_SpawnEnemyTimer{};
		static constexpr float s_MAX_SPAWN_TIMER{ 0.2f };

		std::vector<GameObject*> m_pFormationPositions;
		std::vector<GameObject*> m_pEnemies;

		void ParseEnemies();

		void NextStage();
		void SkipStage();
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
