#include "Enemy.h"
#include "Scene.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "EnemyComponents.h"
#include "CollisionComponents.h"

dae::Enemy::Enemy(glm::vec2& startPos, std::string& spriteLocation, float& spawnMovSpeed, float& patrolSpeed)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }
{}

dae::Enemy::Enemy(glm::vec2&& startPos, std::string&& spriteLocation, float&& spawnMovSpeed, float&& patrolSpeed)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }
{}

dae::GameObject* dae::Enemy::AddToScene(Scene* pSceneToAddTo)
{
	auto pEnemy = pSceneToAddTo->AddGameObject();
	auto pTextureComp = pEnemy->AddComponent<dae::TextureComponent>(m_SpriteLocation);
	pEnemy->AddComponent<dae::EnemyLogic>(m_SpawnMovSpeed, m_PatrolMovSpeed);
	pEnemy->AddComponent<dae::CollisionComponent>(pTextureComp->GetTextureSize());

	pEnemy->SetLocalTranslation(m_StartPos);
	pEnemy->SetLocalScale(2.0f, 2.0f);

	return pEnemy;
}   
