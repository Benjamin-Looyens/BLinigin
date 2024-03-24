#include "Player.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "DerivedCommands.h"
#include "DerivedObservers.h"
#include "DerivedComponents.h"

dae::Player::Player(glm::vec3& playerStartPos, glm::vec3& livesCounterPos, glm::vec3& scoreCounterPos, std::string& playerSpriteLocation, int& playerIndex, float& movSpeed, bool& useController)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_MovSpeed{ movSpeed },
	m_UseController{ useController }
{}

dae::Player::Player(glm::vec3&& playerStartPos, glm::vec3&& livesCounterPos, glm::vec3&& scoreCounterPos, std::string&& playerSpriteLocation, int&& playerIndex, float&& movSpeed, bool&& useController)
	: m_PlayerStartPos{ playerStartPos }, m_LivesCounterPos{ livesCounterPos }, m_ScoreCounterPos{ scoreCounterPos },
	m_PlayerSpriteLocation{ playerSpriteLocation },
	m_PlayerIndex{ playerIndex },
	m_MovSpeed{ movSpeed },
	m_UseController{ useController }
{}

dae::Player::Player(PlayerData& playerdata)
	: Player(playerdata.startPos, playerdata.livesCounterPos, playerdata.scoreCounterPos, playerdata.spriteLocation, playerdata.playerIndex, playerdata.movSpeed, playerdata.useController)
{}

void dae::Player::AddToScene(Scene* pSceneToAddTo)
{
	auto pPlayer = pSceneToAddTo->AddGameObject();
	pPlayer->AddComponent<dae::TextureComponent>(m_PlayerSpriteLocation);
	pPlayer->SetLocalTranslation(m_PlayerStartPos);

	auto& input = InputManager::GetInstance();

	auto pFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);

	auto livesCounterObject = pSceneToAddTo->AddGameObject();
	livesCounterObject->AddComponent<dae::TextComponent>("Lives: ", pFont);
	livesCounterObject->AddComponent<dae::LivesCounterComponent>(5, 5);
	livesCounterObject->SetLocalTranslation(m_LivesCounterPos);

	auto pLivesObserver = pSceneToAddTo->AddObserver<dae::LivesObserver>(livesCounterObject);

	auto pSubject = pPlayer->AddComponent<dae::ObserverSubjectComponent>();
	pSubject->AddObserver(pLivesObserver);

	switch (m_UseController) {
	case true:
		input.AddGamepad();
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_UP,		pPlayer, glm::vec3{  0, -1,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_LEFT,	pPlayer, glm::vec3{ -1,  0,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_RIGHT,	pPlayer, glm::vec3{  1,  0,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_GAMEPAD_DPAD_DOWN,	pPlayer, glm::vec3{  0,  1,  0 }, m_MovSpeed);

		input.AddCommand<dae::NotifyObserverCommand>(BL_GAMEPAD_A, dae::InputState::PRESSED_THIS_FRAME, pPlayer, pLivesObserver);
		break;
	case false:
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_W, pPlayer, glm::vec3{  0, -1,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_A, pPlayer, glm::vec3{ -1,  0,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_S, pPlayer, glm::vec3{  0,  1,  0 }, m_MovSpeed);
		input.AddCommand<dae::MoveCommand>(BL_KEYBOARD_D, pPlayer, glm::vec3{  1,  0,  0 }, m_MovSpeed);

		input.AddCommand<dae::NotifyObserverCommand>(BL_KEYBOARD_SPACE, dae::InputState::PRESSED_THIS_FRAME, pPlayer, pLivesObserver);
		break;
	}
}
