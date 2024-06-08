#include "MainMenu.h"
#include "InputManager.h"
#include "DerivedCommands.h"
#include "GameObject.h"
#include "TextureComponents.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "HighScore.h"
#include "Renderer.h"

dae::MainMenuComponent::MainMenuComponent(GameObject* pParent)
	: Component(pParent)
{
	ChangeState(std::make_unique<dae::TitleState>());

	// Keyboard
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_W, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::UP);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_S, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::DOWN);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_LSHIFT, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::BACK);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_SPACE, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::CONFIRM);

	// Gamepad 1
	auto index = BL_INPUT.AddGamepad();
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_GAMEPAD_DPAD_UP, index, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::UP);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_GAMEPAD_DPAD_DOWN, index, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::DOWN);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_GAMEPAD_B, index, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::BACK);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_GAMEPAD_A, index, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::CONFIRM);


	auto pScene = m_pParent->GetContainingScene();
	m_pUsedMenuFont = BL_RESOURCE.LoadFont("Emulogic.ttf", 20);
	m_pSelectionObj = pScene->AddGameObject();
	m_pSelectionObj->AddComponent<dae::TextComponent>(">", m_pUsedMenuFont, "MenuSelector", true);

	auto pTop = pScene->AddGameObject();
	m_pTopOptionComp = pTop->AddComponent<dae::TextComponent>("N", m_pUsedMenuFont, "MenuTopOption", false);
	pTop->SetLocalTranslation({ s_OPTION_X, s_TOP_ELEVATION });

	auto pBot = pScene->AddGameObject();
	m_pBottomOptionComp = pBot->AddComponent<dae::TextComponent>("N", m_pUsedMenuFont, "MenuBotOption", false);
	pBot->SetLocalTranslation({ s_OPTION_X, s_BOT_ELEVATION });

	m_pSelectionObj->SetLocalTranslation({ s_SELECTION_X, m_SelectionElevation });
}

void dae::MainMenuComponent::Update()
{
	if (m_pState.get())	m_pState->Update(*this);

	// Reset input refs for state
	m_UpPressedThisFrame = false;
	m_DownPressedThisFrame = false;
	m_BackPressedThisFrame = false;
	m_ConfirmPressedThisFrame = false;
}

void dae::MainMenuComponent::LateUpdate()
{
	if (m_pSwitchState.get()) {
		if (m_pState.get()) m_pState->OnExit(*this);

		m_pState = std::move(m_pSwitchState);
		m_pSwitchState.reset();

		m_pState->OnEnter(*this);
	}
}

void dae::MainMenuComponent::ChangeState(std::unique_ptr<MainMenuState> pNewState)
{
	m_pSwitchState = std::move(pNewState);
}

void dae::MainMenuComponent::SetSelectorElevation(float newElevation)
{
	m_SelectionElevation = newElevation;
	m_pSelectionObj->SetLocalTranslation({ s_SELECTION_X, m_SelectionElevation });
}

void dae::MainMenuComponent::SetTopText(std::string newText)
{
	if (m_pTopOptionComp) {
		m_pTopOptionComp->SetText(newText);
	}
}

void dae::MainMenuComponent::SetBotText(std::string newText)
{
	if (m_pBottomOptionComp) {
		m_pBottomOptionComp->SetText(newText);
	}
}

void dae::MainMenuComponent::InitHighScore(const std::string& scoreFile)
{
	if (!m_pHighScoreObj) {
		m_pHighScoreObj = m_pParent->GetContainingScene()->AddGameObject();
	}
	if (!m_pHighScoreComp) {
		m_pHighScoreComp = m_pHighScoreObj->AddComponent<dae::HighScoreComponent>(scoreFile);
	}

	if (m_pHighScoreObj && m_pHighScoreComp) {
		auto pScene = m_pHighScoreObj->GetContainingScene();

		glm::vec2 windowSize = BL_RENDERER.GetWindowSize();
		float xWindowSeperation = windowSize.x / 4;
		float yWindowSeperation = windowSize.y / 6;
		float hsLadderElevation = yWindowSeperation * 2;

		m_pHighScoreObj->SetLocalTranslation({ windowSize.x / 2, 0 });

		std::vector<std::string> scoreStrings;
		scoreStrings.resize(5);
		std::vector<std::string> nameStrings;
		nameStrings.resize(5);
		auto pHighScores = m_pHighScoreComp->Get5HighestScores();

		// Score to string data
		std::transform(pHighScores.begin(), pHighScores.end(), scoreStrings.begin(),
			[](const ScoreData* pScore) 
			{ 
				if (pScore) return std::to_string(pScore->score);
				else return std::string("-----");
			});

		// Initials stored
		std::transform(pHighScores.begin(), pHighScores.end(), nameStrings.begin(),
			[](const ScoreData* pScore) 
			{ 
				if (pScore) return pScore->initials;
				else return std::string("---");
			});

		// Number (1st, 2nd, ...)
		auto pEmptyNumber = pScene->AddGameObject(); // ROOT of the numbers
		m_pHighScoreObj->AddChild(pEmptyNumber);
		pEmptyNumber->SetLocalTranslation({ - xWindowSeperation , hsLadderElevation });

		auto pFirst		= pScene->AddGameObject();
		auto pSecond	= pScene->AddGameObject();
		auto pThird		= pScene->AddGameObject();
		auto pFourth	= pScene->AddGameObject();
		auto pFifth		= pScene->AddGameObject();

		pFirst->AddComponent<dae::TextComponent> ("1ST", m_pUsedMenuFont, "HighScore1stLabel", true);
		pSecond->AddComponent<dae::TextComponent>("2ND", m_pUsedMenuFont, "HighScore2ndLabel", true);
		pThird->AddComponent<dae::TextComponent> ("3RD", m_pUsedMenuFont, "HighScore3rdLabel", true);
		pFourth->AddComponent<dae::TextComponent>("4TH", m_pUsedMenuFont, "HighScore4thLabel", true);
		pFifth->AddComponent<dae::TextComponent> ("5TH", m_pUsedMenuFont, "HighScore5thLabel", true);

		pEmptyNumber->AddChild(pFirst);
		pEmptyNumber->AddChild(pSecond);
		pEmptyNumber->AddChild(pThird);
		pEmptyNumber->AddChild(pFourth);
		pEmptyNumber->AddChild(pFifth);

		pFirst->SetLocalTranslation ({ 0, 25.0f * 1 });
		pSecond->SetLocalTranslation({ 0, 25.0f * 2 });
		pThird->SetLocalTranslation ({ 0, 25.0f * 3 });
		pFourth->SetLocalTranslation({ 0, 25.0f * 4 });
		pFifth->SetLocalTranslation ({ 0, 25.0f * 5 });

		// Score
		auto pScoreRoot = pScene->AddGameObject(); // ROOT of the scores
		pScoreRoot->AddComponent<dae::TextComponent>("SCORE", m_pUsedMenuFont, "HighScore_ScoreRoot", true);
		m_pHighScoreObj->AddChild(pScoreRoot);
		pScoreRoot->SetLocalTranslation({ 0, hsLadderElevation });

		pFirst	= pScene->AddGameObject();
		pSecond	= pScene->AddGameObject();
		pThird	= pScene->AddGameObject();
		pFourth	= pScene->AddGameObject();
		pFifth	= pScene->AddGameObject();

		pFirst->AddComponent<dae::TextComponent> (scoreStrings[0], m_pUsedMenuFont, "HighScore1stScore", true);
		pSecond->AddComponent<dae::TextComponent>(scoreStrings[1], m_pUsedMenuFont, "HighScore2ndScore", true);
		pThird->AddComponent<dae::TextComponent> (scoreStrings[2], m_pUsedMenuFont, "HighScore3rdScore", true);
		pFourth->AddComponent<dae::TextComponent>(scoreStrings[3], m_pUsedMenuFont, "HighScore4thScore", true);
		pFifth->AddComponent<dae::TextComponent> (scoreStrings[4], m_pUsedMenuFont, "HighScore5thScore", true);

		pScoreRoot->AddChild(pFirst);
		pScoreRoot->AddChild(pSecond);
		pScoreRoot->AddChild(pThird);
		pScoreRoot->AddChild(pFourth);
		pScoreRoot->AddChild(pFifth);

		pFirst->SetLocalTranslation ({ 0, 25.0f * 1 });
		pSecond->SetLocalTranslation({ 0, 25.0f * 2 });
		pThird->SetLocalTranslation ({ 0, 25.0f * 3 });
		pFourth->SetLocalTranslation({ 0, 25.0f * 4 });
		pFifth->SetLocalTranslation ({ 0, 25.0f * 5 });

		// Name
		auto pNameRoot = pScene->AddGameObject(); // ROOT of the names/initials
		pNameRoot->AddComponent<dae::TextComponent>("NAME", m_pUsedMenuFont, "HighScore_NameRoot", true);
		m_pHighScoreObj->AddChild(pNameRoot);
		pNameRoot->SetLocalTranslation({ xWindowSeperation, hsLadderElevation });

		pFirst	= pScene->AddGameObject();
		pSecond	= pScene->AddGameObject();
		pThird	= pScene->AddGameObject();
		pFourth	= pScene->AddGameObject();
		pFifth	= pScene->AddGameObject();

		pFirst->AddComponent<dae::TextComponent> (nameStrings[0], m_pUsedMenuFont, "HighScore1stName", true);
		pSecond->AddComponent<dae::TextComponent>(nameStrings[1], m_pUsedMenuFont, "HighScore2ndName", true);
		pThird->AddComponent<dae::TextComponent> (nameStrings[2], m_pUsedMenuFont, "HighScore3rdName", true);
		pFourth->AddComponent<dae::TextComponent>(nameStrings[3], m_pUsedMenuFont, "HighScore4thName", true);
		pFifth->AddComponent<dae::TextComponent> (nameStrings[4], m_pUsedMenuFont, "HighScore5thName", true);

		pNameRoot->AddChild(pFirst);
		pNameRoot->AddChild(pSecond);
		pNameRoot->AddChild(pThird);
		pNameRoot->AddChild(pFourth);
		pNameRoot->AddChild(pFifth);

		pFirst->SetLocalTranslation ({ 0, 25.0f * 1 });
		pSecond->SetLocalTranslation({ 0, 25.0f * 2 });
		pThird->SetLocalTranslation ({ 0, 25.0f * 3 });
		pFourth->SetLocalTranslation({ 0, 25.0f * 4 });
		pFifth->SetLocalTranslation ({ 0, 25.0f * 5 });

	}
}

void dae::MainMenuComponent::SetHighScoreVisible(bool newVisible)
{
	if (m_pHighScoreComp) m_pHighScoreComp->SetVisible(newVisible);
}