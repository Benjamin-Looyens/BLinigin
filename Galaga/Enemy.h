#pragma once
#include "Prefab.h"
#include "glm/vec3.hpp"
#include <string>

namespace dae {
	struct EnemyData
	{
		glm::vec3 startPos;
		std::string spriteLocation;
		float movSpeed;
	};

	class Enemy : public Prefab
	{
	public:
		Enemy(glm::vec3& startPos, std::string& spriteLocation, float& spawnMovSpeed, float& patrolSpeed);
		Enemy(glm::vec3&& startPos, std::string&& spriteLocation, float&& spawnMovSpeed, float&& patrolSpeed);

		virtual void AddToScene(Scene* pSceneToAddTo) override;

		Enemy(const Enemy& other) = delete;
		Enemy(const Enemy&& other) = delete;
		Enemy& operator=(const Enemy& other) = delete;
		Enemy& operator=(const Enemy&& other) = delete;

	private:
		glm::vec3 m_StartPos;
		std::string m_SpriteLocation;
		float m_SpawnMovSpeed;
		float m_PatrolMovSpeed;
	};
}


