#pragma once
#include "Component.h"
#include "ObserverComponents.h"
#include "EnemyState.h"
#include <memory>

namespace dae {
	class GameObject;

	class EnemyLogic final : public SubjectComponent
	{
	public:
		EnemyLogic(GameObject* pParent, float spawnSpeed, float patrolSpeed);
		void Update() override;
		void LateUpdate() override;
		
		void SetState(std::unique_ptr<EnemyState> pNewState);
		void SetLocation(glm::vec2 pos);
		void GetHit();
		void Die();
		bool Damaged() const { return m_TookDamage; };

		glm::vec2 GetLocation();
		glm::vec2 GetScale();

		float GetSpawnSpeed() const { return m_SpawnSpeed; };
		float GetPatrolSpeed() const { return m_PatrolSpeed; };

	private:
		std::unique_ptr<EnemyState> m_pState;
		std::unique_ptr<EnemyState> m_pSwitchState;

		float m_SpawnSpeed;
		float m_PatrolSpeed;

		bool m_TookDamage = false;

		int m_Health;
	};
}