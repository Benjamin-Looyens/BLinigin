#pragma once
#include "Prefab.h"
#include "glm/vec3.hpp"
#include <string>

namespace dae {
	struct PlayerData
	{
		glm::vec3 startPos;
		glm::vec3 livesCounterPos;
		glm::vec3 scoreCounterPos;
		std::string spriteLocation;
		int playerIndex;
		bool useController;
		float movSpeed;
		int startingLives;
	};

	class Player : public Prefab
	{
	public:
		Player(glm::vec3& playerStartPos, glm::vec3& livesCounterPos, glm::vec3& scoreCounterPos, std::string& playerSpriteLocation, int& playerIndex, float& movSpeed, int& startingLives, bool& useController);
		Player(glm::vec3&& playerStartPos, glm::vec3&& livesCounterPos, glm::vec3&& scoreCounterPos, std::string&& playerSpriteLocation, int&& playerIndex, float&& movSpeed, int&& startingLives, bool&& useController);
		Player(PlayerData& playerdata);

		virtual void AddToScene(Scene* pSceneToAddTo) override;

		Player(const Player& other) = delete;
		Player(const Player&& other) = delete;
		Player& operator=(const Player& other) = delete;
		Player& operator=(const Player&& other) = delete;

	private:
		glm::vec3 m_PlayerStartPos;
		glm::vec3 m_LivesCounterPos;
		glm::vec3 m_ScoreCounterPos;
		std::string m_PlayerSpriteLocation;
		int m_PlayerIndex;
		bool m_UseController;
		float m_MovSpeed;
		int m_StartingLives;
	};
}

