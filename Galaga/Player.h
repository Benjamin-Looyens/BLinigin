#pragma once
#include "Prefab.h"
#include "glm/vec2.hpp"
#include <string>

namespace dae {
	struct PlayerData
	{
		glm::vec2 startPos;
		glm::vec2 livesCounterPos;
		glm::vec2 scoreCounterPos;
		std::string spriteLocation;
		int playerIndex;
		bool useController;
		float movSpeed;
		int startingLives;
	};

	class Player final : public Prefab
	{
	public:
		Player(glm::vec2& playerStartPos, glm::vec2& livesCounterPos, glm::vec2& scoreCounterPos, std::string& playerSpriteLocation, int& playerIndex, float& movSpeed, int& startingLives, bool& useController);
		Player(glm::vec2&& playerStartPos, glm::vec2&& livesCounterPos, glm::vec2&& scoreCounterPos, std::string&& playerSpriteLocation, int&& playerIndex, float&& movSpeed, int&& startingLives, bool&& useController);
		Player(PlayerData& playerdata);

		virtual void AddToScene(Scene* pSceneToAddTo) override;

		Player(const Player& other) = delete;
		Player(const Player&& other) = delete;
		Player& operator=(const Player& other) = delete;
		Player& operator=(const Player&& other) = delete;

	private:
		glm::vec2 m_PlayerStartPos;
		glm::vec2 m_LivesCounterPos;
		glm::vec2 m_ScoreCounterPos;
		std::string m_PlayerSpriteLocation;
		int m_PlayerIndex;
		bool m_UseController;
		float m_SpawnMovSpeed;
		int m_StartingLives;
	};
}

