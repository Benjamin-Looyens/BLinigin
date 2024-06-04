#include "EnemyComponents.h"
#include "EnemyState.h"
#include "GameObject.h"
#include "CollisionComponents.h"
#include "DerivedEvents.h"
#include "DerivedComponents.h"
#include <SDL.h>
#include "ServiceLocator.h"

#include "TEMP_PlayerGlobal.h"

dae::EnemyLogic::EnemyLogic(GameObject* pParent, float spawnSpeed, float patrolSpeed, int health, sound_id damageSoundID, sound_id deathSoundID)
	: SubjectComponent(pParent), m_SpawnSpeed{ spawnSpeed }, m_PatrolSpeed{ patrolSpeed }, m_Health{ health }, m_DamageSound{ damageSoundID }, m_DeathSound{ deathSoundID }
{
	m_pSwitchState = std::make_unique<EnemySpawnInState>();
}

void dae::EnemyLogic::Update()
{
	if (m_pState.get())	m_pState->Update(*this);
}

void dae::EnemyLogic::LateUpdate()
{
	if (m_pSwitchState.get()) {
		if (m_pState.get()) m_pState->OnExit(*this);
		
		m_pState = std::move(m_pSwitchState);
		m_pSwitchState.reset();

		m_pState->OnEnter(*this);
	}
}

void dae::EnemyLogic::SetState(std::unique_ptr<EnemyState> pNewState)
{
	m_pSwitchState = std::move(pNewState);
}

void dae::EnemyLogic::SetLocation(glm::vec2 pos)
{
	m_pParent->SetLocalTranslation(pos);
}

bool dae::EnemyLogic::GetHit()
{
	// TODO : Implement more logic for get hit
	if (--m_Health <= 0) {
		if (m_pState != nullptr && dynamic_cast<EnemyKamikazeState*>(m_pState.get())) {
			NotifyObservers(Event::BOSS_DIED_KAMIKAZE);
			m_ReturnDeath = m_KamikazeDeath;
		}
		else {
			NotifyObservers(Event::BOSS_DIED_FORMATION);
			m_ReturnDeath = m_NormalDeath;
		}

		SetState(std::make_unique<dae::EnemyDeathState>());

		m_TookDamage = true;
		m_pParent->GetComponent<dae::AnimatedTextureComponent>()->StopAnimating();

		BL_SOUND.PlaySound(m_DeathSound, 0.3f);

		return true; 
	}
	else {
		m_pParent->GetComponent<dae::AnimatedTextureComponent>()->NextLayer();

		BL_SOUND.PlaySound(m_DamageSound, 0.3f);
	}

	return false; // Is still alive, return false
}

void dae::EnemyLogic::Die()
{
	m_pState->OnExit(*this);

	auto parentPos = m_pParent->GetWorldPosition();
	auto parentSize = m_pParent->GetComponent<dae::AnimatedTextureComponent>()->GetAppliedTextureSize();

	auto pDeathObj = m_DeathAnim.AddToScene(m_pParent->GetContainingScene());
	auto deathSize = pDeathObj->GetComponent<dae::AnimatedTextureComponent>()->GetAppliedTextureSize();

	glm::vec2 deathPos{ parentPos.x + ((parentSize.x - deathSize.x) / 2), parentPos.y + ((parentSize.y - deathSize.y) / 2) };

	pDeathObj->SetLocalTranslation(deathPos);
	pDeathObj->SetLocalScale(m_pParent->GetWorldScale());

	m_pParent->Destroy();
}

glm::vec2 dae::EnemyLogic::GetLocation()
{
	auto pos = m_pParent->GetWorldPosition();
	return { pos.x, pos.y };
}

glm::vec2 dae::EnemyLogic::GetScale()
{
	auto scale = m_pParent->GetWorldScale();
	return { scale.x, scale.y };
}
