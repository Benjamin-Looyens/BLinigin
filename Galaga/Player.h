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

	enum class InputType
	{
		KEYBOARD,
		GAMEPAD,
		KEYBOARD_AND_GAMEPAD
	};

	class Player final : public Prefab
	{
	public:
		Player(glm::vec2& playerStartPos, glm::vec2& livesCounterPos, glm::vec2& scoreCounterPos, std::string& playerSpriteLocation, int& playerIndex, float& movSpeed, int& startingLives, InputType& inputType);
		Player(glm::vec2&& playerStartPos, glm::vec2&& livesCounterPos, glm::vec2&& scoreCounterPos, std::string&& playerSpriteLocation, int&& playerIndex, float&& movSpeed, int&& startingLives, InputType&& inputType);

		GameObject* AddToScene(Scene* pSceneToAddTo) override;

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
		float m_SpawnMovSpeed;
		int m_StartingLives;

		dae::InputType m_InputType;

		void AddKeyboardInput(GameObject* pPlayerObj) const;
		void AddGamepadInput(GameObject* pPlayerObj) const;
	};
}

