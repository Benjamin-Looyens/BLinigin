#include "Player.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "DerivedCommands.h"
#include "DerivedComponents.h"
#include "DerivedEvents.h"
#include "ObserverComponents.h"

dae::Player::Player(glm::vec3& playerStartPos, glm::vec3& livesCounterPos, glm::vec3& scoreCounterPos, std::string& playerSpriteLocation, int& playerIndex, float& movSpeed, int& startingLives, bool& useController)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_MovSpeed{ movSpeed },
	m_UseController{ useController },
	m_StartingLives{ startingLives }
{}

dae::Player::Player(glm::vec3&& playerStartPos, glm::vec3&& livesCounterPos, glm::vec3&& scoreCounterPos, std::string&& playerSpriteLocation, int&& playerIndex, float&& movSpeed, int&& startingLives, bool&& useController)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_MovSpeed{ movSpeed },
	m_UseController{ useController },
	m_StartingLives{ startingLives }
{}

dae::Player::Player(PlayerData& playerdata)
	: Player(playerdata.startPos, playerdata.livesCounterPos, playerdata.scoreCounterPos, playerdata.spriteLocation, playerdata.playerIndex, playerdata.movSpeed, playerdata.startingLives, playerdata.useController)
{}

void dae::Player::AddToScene(Scene* pSceneToAddTo)
{
	// Player itself
	auto pPlayer = pSceneToAddTo->AddGameObject();
	pPlayer->AddComponent<dae::TextureComponent>(m_PlayerSpriteLocation);
	pPlayer->SetLocalTranslation(m_PlayerStartPos);
	pPlayer->AddComponent<dae::ScoreComponent>();
	pPlayer->AddComponent<dae::LivesComponent>(m_StartingLives);

	// Lives & Score counters
	auto pFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);

	const std::string scorePrefix{ "Player " + std::to_string(m_PlayerIndex + 1) + " score: " };
	auto pScoreObject = pSceneToAddTo->AddGameObject();
	pScoreObject->AddComponent<dae::TextComponent>("Score: ", pFont);
	pScoreObject->AddComponent<dae::DisplayDataComponent>(scorePrefix.c_str(), std::to_string(0));
	pScoreObject->SetLocalTranslation(m_ScoreCounterPos);

	const std::string livesPrefix{ "Player " + std::to_string(m_PlayerIndex + 1) + " lives: " };
	auto pLivesObject = pSceneToAddTo->AddGameObject();
	pLivesObject->AddComponent<dae::TextComponent>("Lives: ", pFont);
	pLivesObject->AddComponent<dae::DisplayDataComponent>(livesPrefix.c_str(), std::to_string(m_StartingLives));
	pLivesObject->SetLocalTranslation(m_LivesCounterPos);

	// Observers
	auto pSubject = pPlayer->AddComponent<dae::SubjectComponent>();
	auto pScoreObserver = pScoreObject->AddComponent<dae::ScoreObservingComponent>(pScoreObject);
	auto pLivesObserver = pLivesObject->AddComponent<dae::LivesObservingComponent>(pLivesObject);
	pSubject->AddObserver(pScoreObserver);
	pSubject->AddObserver(pLivesObserver);

	// Input
	auto& input = InputManager::GetInstance();

	switch (m_UseController) {
	case true:
		input.AddGamepad();
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_UP,		pPlayer, glm::vec3{  0, -1,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_LEFT,	pPlayer, glm::vec3{ -1,  0,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_RIGHT,	pPlayer, glm::vec3{  1,  0,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_DOWN,	pPlayer, glm::vec3{  0,  1,  0 }, m_MovSpeed);

		input.AddCommand<dae::NotifyObserverCommand>(BL_GAMEPAD_A, dae::InputState::PRESSED_THIS_FRAME, pPlayer, Event::BIG_ENEMY_DIED);
		input.AddCommand<dae::NotifyObserverCommand>(BL_GAMEPAD_B, dae::InputState::PRESSED_THIS_FRAME, pPlayer, Event::SMALL_ENEMY_DIED);
		input.AddCommand<dae::NotifyObserverCommand>(BL_GAMEPAD_X, dae::InputState::PRESSED_THIS_FRAME, pPlayer, Event::PLAYER_DIED);
		break;
	case false:
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_W, pPlayer, glm::vec3{  0, -1,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_A, pPlayer, glm::vec3{ -1,  0,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_S, pPlayer, glm::vec3{  0,  1,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_D, pPlayer, glm::vec3{  1,  0,  0 }, m_MovSpeed);

		input.AddCommand<dae::NotifyObserverCommand>(BL_KEYBOARD_E,		dae::InputState::PRESSED_THIS_FRAME, pPlayer, Event::BIG_ENEMY_DIED);
		input.AddCommand<dae::NotifyObserverCommand>(BL_KEYBOARD_Q,		dae::InputState::PRESSED_THIS_FRAME, pPlayer, Event::SMALL_ENEMY_DIED);
		input.AddCommand<dae::NotifyObserverCommand>(BL_KEYBOARD_SPACE, dae::InputState::PRESSED_THIS_FRAME, pPlayer, Event::PLAYER_DIED);
		break;
	}
}
