#pragma once
#include "Prefab.h"
#include "glm/vec2.hpp"
#include <string>

namespace dae {
	class IObserver;

	struct EnemyData
	{
		glm::vec2 startPos;
		std::string spriteLocation;
		float movSpeed;
	};

	class Enemy : public Prefab
	{
	public:
		Enemy(glm::vec2& startPos, std::string& spriteLocation, float& spawnMovSpeed, float& patrolSpeed, std::vector<IObserver*>& observers);
		Enemy(glm::vec2&& startPos, std::string&& spriteLocation, float&& spawnMovSpeed, float&& patrolSpeed, std::vector<IObserver*>& observers);

		virtual GameObject* AddToScene(Scene* pSceneToAddTo) override;

		Enemy(const Enemy& other) = delete;
		Enemy(const Enemy&& other) = delete;
		Enemy& operator=(const Enemy& other) = delete;
		Enemy& operator=(const Enemy&& other) = delete;

	private:
		glm::vec2 m_StartPos;
		std::string m_SpriteLocation;
		float m_SpawnMovSpeed;
		float m_PatrolMovSpeed;
		std::vector<IObserver*> m_Observers; 
	};
}


