#include "Enemy.h"
#include "Scene.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "EnemyComponents.h"
#include "CollisionComponents.h"
#include "Observer.h"
#include "ServiceLocator.h"
#include "TextureComponents.h"

dae::Enemy::Enemy(glm::vec2& startPos, std::string& spriteLocation, float& spawnMovSpeed, float& patrolSpeed, std::vector<IObserver*>& observers, float& animFPS, int& animFrames, int& health, sound_id& damageSoundID, sound_id& deathSoundID)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }, m_Observers{ observers }, 
	  m_AnimFPS{ animFPS }, m_AnimFrames{ animFrames }, m_Health{ health }, m_DeathSound{ deathSoundID }, m_DamageSound{ damageSoundID }
{}

dae::Enemy::Enemy(glm::vec2&& startPos, std::string&& spriteLocation, float&& spawnMovSpeed, float&& patrolSpeed, std::vector<IObserver*>& observers, float&& animFPS, int&& animFrames, int&& health, sound_id&& damageSoundID, sound_id&& deathSoundID)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }, m_Observers{ observers },
	  m_AnimFPS{ animFPS }, m_AnimFrames{ animFrames }, m_Health{ health }, m_DeathSound{ deathSoundID }, m_DamageSound{ damageSoundID }
{}

dae::GameObject* dae::Enemy::AddToScene(Scene* pSceneToAddTo)
{
	auto pEnemy = pSceneToAddTo->AddGameObject();
	auto pTextureComp = pEnemy->AddComponent<dae::AnimatedTextureComponent>(m_SpriteLocation, m_AnimFrames, m_AnimFPS, m_Health);
	auto pLogic = pEnemy->AddComponent<dae::EnemyLogic>(m_SpawnMovSpeed, m_PatrolMovSpeed, m_Health, m_DamageSound, m_DeathSound);
	pEnemy->AddComponent<dae::CollisionComponent>(pTextureComp->GetAppliedTextureSize());

	pEnemy->SetLocalTranslation(m_StartPos);
	pEnemy->SetLocalScale(2.0f, 2.0f);

	for (auto& observer : m_Observers) {
		pLogic->AddObserver(observer);
	}

	return pEnemy;
}   
