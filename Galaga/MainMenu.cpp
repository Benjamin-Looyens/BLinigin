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
		//auto pScene = m_pHighScoreObj->GetContainingScene();
		m_pHighScoreComp->AddHighScoreListAsChildren(m_pUsedMenuFont);
	}
}

void dae::MainMenuComponent::SetHighScoreVisible(bool newVisible)
{
	if (m_pHighScoreComp) m_pHighScoreComp->SetVisible(newVisible);
}