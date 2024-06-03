#include "Enemy.h"
#include "Scene.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "EnemyComponents.h"
#include "CollisionComponents.h"
#include "Observer.h"

dae::Enemy::Enemy(glm::vec2& startPos, std::string& spriteLocation, float& spawnMovSpeed, float& patrolSpeed, std::vector<IObserver*>& observers)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }, m_Observers{ observers }
{}

dae::Enemy::Enemy(glm::vec2&& startPos, std::string&& spriteLocation, float&& spawnMovSpeed, float&& patrolSpeed, std::vector<IObserver*>& observers)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }, m_Observers{ observers }
{}

dae::GameObject* dae::Enemy::AddToScene(Scene* pSceneToAddTo)
{
	auto pEnemy = pSceneToAddTo->AddGameObject();
	auto pTextureComp = pEnemy->AddComponent<dae::AnimatedTextureComponent>(m_SpriteLocation, 2, 4.0f); // pPlayer->AddComponent<dae::AnimatedTextureComponent>(m_PlayerSpriteLocation, 2, 5.0f);
	auto pLogic = pEnemy->AddComponent<dae::EnemyLogic>(m_SpawnMovSpeed, m_PatrolMovSpeed);
	pEnemy->AddComponent<dae::CollisionComponent>(pTextureComp->GetAppliedTextureSize());

	pEnemy->SetLocalTranslation(m_StartPos);
	pEnemy->SetLocalScale(2.0f, 2.0f);

	for (auto& observer : m_Observers) {
		pLogic->AddObserver(observer);
	}

	return pEnemy;
}   
