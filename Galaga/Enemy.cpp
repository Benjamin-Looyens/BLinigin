#include "Enemy.h"
#include "Scene.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "EnemyComponents.h"

dae::Enemy::Enemy(glm::vec3& startPos, std::string& spriteLocation, float& spawnMovSpeed, float& patrolSpeed)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }
{}

dae::Enemy::Enemy(glm::vec3&& startPos, std::string&& spriteLocation, float&& spawnMovSpeed, float&& patrolSpeed)
	: m_StartPos{ startPos }, m_SpriteLocation{ spriteLocation }, m_SpawnMovSpeed{ spawnMovSpeed }, m_PatrolMovSpeed{ patrolSpeed }
{}

void dae::Enemy::AddToScene(Scene* pSceneToAddTo)
{
	auto pEnemy = pSceneToAddTo->AddGameObject();
	pEnemy->AddComponent<dae::TextureComponent>(m_SpriteLocation);
	pEnemy->AddComponent<dae::EnemyLogic>(m_SpawnMovSpeed, m_PatrolMovSpeed);
	pEnemy->SetLocalTranslation(m_StartPos);
	pEnemy->SetLocalScale(2.0f, 2.0f, 2.0f);
}   
