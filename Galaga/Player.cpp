#include "Player.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "DerivedCommands.h"
#include "DerivedComponents.h"
#include "DerivedEvents.h"
#include "ObserverComponents.h"
#include "CollisionComponents.h"
#include "PlayerComponents.h"
#include "GameplayCommands.h"
#include "TextureComponents.h"

dae::Player::Player(glm::vec2& playerStartPos, glm::vec2& livesCounterPos, glm::vec2& scoreCounterPos, std::string& playerSpriteLocation, int& playerIndex, float& movSpeed, int& startingLives, dae::InputType& inputType)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_SpawnMovSpeed{ movSpeed },
	m_InputType{ inputType },
	m_StartingLives{ startingLives }
{}

dae::Player::Player(glm::vec2&& playerStartPos, glm::vec2&& livesCounterPos, glm::vec2&& scoreCounterPos, std::string&& playerSpriteLocation, int&& playerIndex, float&& movSpeed, int&& startingLives, dae::InputType&& inputType)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_SpawnMovSpeed{ movSpeed },
	m_InputType{ inputType },
	m_StartingLives{ startingLives }
{}

dae::GameObject* dae::Player::AddToScene(Scene* pSceneToAddTo)
{
	// Player itself
	auto pPlayer = pSceneToAddTo->AddGameObject();
	pPlayer->SetLocalScale(2.0f, 2.0f);
	auto pTextureComp = pPlayer->AddComponent<dae::TextureComponent>(m_PlayerSpriteLocation);
	pPlayer->SetLocalTranslation(m_PlayerStartPos);
	auto pPlayerLogic = pPlayer->AddComponent<dae::PlayerLogic>();
	pPlayer->AddComponent<dae::CollisionComponent>(pTextureComp->GetTextureSize());

	// Lives Counter
	dae::LivesComponent* pLivesComp;
	if (m_PlayerIndex)	pLivesComp = pPlayer->AddComponent<dae::LivesComponent>(m_StartingLives, m_LivesCounterPos, glm::vec2{ -1, 0 });
	else				pLivesComp = pPlayer->AddComponent<dae::LivesComponent>(m_StartingLives, m_LivesCounterPos, glm::vec2{ 1, 0 });

	// Score Counter
	auto pFont = BL_RESOURCE.LoadFont("Emulogic.ttf", 18);
	auto pScore = pPlayer->AddComponent<dae::ScoreComponent>(m_ScoreCounterPos, pFont, std::string{ "playerScore" + std::to_string(m_PlayerIndex) });

	// Observers
	pPlayerLogic->AddObserver(pLivesComp);
	pScore->AddObserver(pLivesComp);

	// Input
	switch (m_InputType) {
	case InputType::GAMEPAD:
		AddGamepadInput(pPlayer);
		break;
	case InputType::KEYBOARD:
		AddKeyboardInput(pPlayer);
		break;
	case InputType::KEYBOARD_AND_GAMEPAD:
		AddGamepadInput(pPlayer);
		AddKeyboardInput(pPlayer);
		break;
	}

	return pPlayer;
}

void dae::Player::AddKeyboardInput(GameObject* pPlayerObj) const
{
	auto& input = BL_INPUT;

	input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_A, 0, pPlayerObj, glm::vec2{ -1.0f,  0.0f }, m_SpawnMovSpeed);
	input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_D, 0, pPlayerObj, glm::vec2{ 1.0f,  0.0f }, m_SpawnMovSpeed);

	input.AddCommand<dae::FireCommand>(BL_KEYBOARD_SPACE, 0, dae::InputState::PRESSED_THIS_FRAME, pPlayerObj);

#ifndef NDEBUG
	input.AddCommand<dae::KillPlayerCommand>(BL_KEYBOARD_F10, 0, dae::InputState::PRESSED_THIS_FRAME, pPlayerObj);
#endif
}

void dae::Player::AddGamepadInput(GameObject* pPlayerObj) const
{
	auto& input = BL_INPUT;

	dae::input_index index = static_cast<dae::input_index>(m_PlayerIndex + 1);

	input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_LEFT, index, pPlayerObj, glm::vec2{ -1.0f,  0.0f }, m_SpawnMovSpeed);
	input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_RIGHT, index, pPlayerObj, glm::vec2{ 1.0f,  0.0f }, m_SpawnMovSpeed);

	input.AddCommand<dae::FireCommand>(BL_GAMEPAD_R_BUMPER, index, dae::InputState::PRESSED_THIS_FRAME, pPlayerObj);

#ifndef NDEBUG
	input.AddCommand<dae::KillPlayerCommand>(BL_GAMEPAD_START, index, dae::InputState::PRESSED_THIS_FRAME, pPlayerObj);
#endif
}
