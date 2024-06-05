#pragma once
#include "Component.h"
#include "ObserverComponents.h"
#include "EnemyState.h"
#include <memory>
#include "EnemyDeath.h"
#include "DerivedEvents.h"
#include "SoundSystem.h"

namespace dae {
	class GameObject;

	class EnemyLogic final : public SubjectComponent
	{
	public:
		EnemyLogic(GameObject* pParent, float spawnSpeed, float patrolSpeed, int health, sound_id damageSoundID, sound_id deathSoundID);
		void Update() override;
		void LateUpdate() override;
		
		void SetState(std::unique_ptr<EnemyState> pNewState);
		void SetLocation(glm::vec2 pos);
		void AddLocation(glm::vec2 movement);
		bool GetHit(); // Returns boolean for if it died or not
		void Die();
		bool Damaged() const { return m_TookDamage; };

		void SyncToFormation();
		void DetachFromFormation();
		inline void SetFormationRoot(GameObject* pFormationRootObject) { m_pFormationLocationRoot = pFormationRootObject; };

		glm::vec2 GetLocation();
		glm::vec2 GetScale();

		float GetSpawnSpeed() const { return m_SpawnSpeed; };
		float GetPatrolSpeed() const { return m_PatrolSpeed; };

		dae::Event GetDeathEvent() const { return m_ReturnDeath; };

	private:
		std::unique_ptr<EnemyState> m_pState;
		std::unique_ptr<EnemyState> m_pSwitchState;

		float m_SpawnSpeed;
		float m_PatrolSpeed;

		bool m_TookDamage = false;

		int m_Health;

		EnemyDeath m_DeathAnim{ };

		// TODO : Implement these through constructor
		dae::Event m_NormalDeath	= dae::Event::BOSS_DIED_FORMATION;
		dae::Event m_KamikazeDeath	= dae::Event::BOSS_DIED_KAMIKAZE;
		dae::Event m_ReturnDeath	= dae::Event::NULL_EVENT;

		sound_id m_DamageSound;
		sound_id m_DeathSound;

		GameObject* m_pFormationLocationRoot;
	};
}