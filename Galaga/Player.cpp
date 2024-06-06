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

dae::Player::Player(glm::vec2& playerStartPos, glm::vec2& livesCounterPos, glm::vec2& scoreCounterPos, std::string& playerSpriteLocation, int& playerIndex, float& movSpeed, int& startingLives, bool& useController)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_SpawnMovSpeed{ movSpeed },
	m_UseController{ useController },
	m_StartingLives{ startingLives }
{}

dae::Player::Player(glm::vec2&& playerStartPos, glm::vec2&& livesCounterPos, glm::vec2&& scoreCounterPos, std::string&& playerSpriteLocation, int&& playerIndex, float&& movSpeed, int&& startingLives, bool&& useController)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_SpawnMovSpeed{ movSpeed },
	m_UseController{ useController },
	m_StartingLives{ startingLives }
{}

dae::Player::Player(PlayerData& playerdata)
	: Player(playerdata.startPos, playerdata.livesCounterPos, playerdata.scoreCounterPos, playerdata.spriteLocation, playerdata.playerIndex, playerdata.movSpeed, playerdata.startingLives, playerdata.useController)
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
	auto& input = BL_INPUT;

	switch (m_UseController) {
	case true:
		input.AddGamepad();
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_LEFT,	pPlayer, glm::vec2{ -1.0f,  0.0f }, m_SpawnMovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_RIGHT,	pPlayer, glm::vec2{  1.0f,  0.0f }, m_SpawnMovSpeed);

		input.AddCommand<dae::FireCommand>(BL_GAMEPAD_R_BUMPER, dae::InputState::PRESSED_THIS_FRAME, pPlayer);

#ifndef NDEBUG
		input.AddCommand<dae::KillPlayerCommand>(BL_GAMEPAD_START, dae::InputState::PRESSED_THIS_FRAME, pPlayer);
#endif
		break;
	case false:
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_A, pPlayer, glm::vec2{ -1.0f,  0.0f }, m_SpawnMovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_D, pPlayer, glm::vec2{  1.0f,  0.0f }, m_SpawnMovSpeed);

		input.AddCommand<dae::FireCommand>(BL_KEYBOARD_SPACE, dae::InputState::PRESSED_THIS_FRAME, pPlayer);

#ifndef NDEBUG
		input.AddCommand<dae::KillPlayerCommand>(BL_KEYBOARD_F10, dae::InputState::PRESSED_THIS_FRAME, pPlayer);
#endif
		break;
	}

	return pPlayer;
}
