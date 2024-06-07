#include "MainMenu.h"
#include "InputManager.h"
#include "DerivedCommands.h"

dae::MainMenuComponent::MainMenuComponent(GameObject* pParent)
	: Component(pParent)
{
	ChangeState(std::make_unique<dae::TitleState>());

	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_W, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::UP);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_S, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::DOWN);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_LSHIFT, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::BACK);
	BL_INPUT.AddCommand<dae::MainMenuCommand>(BL_KEYBOARD_SPACE, 0, dae::InputState::PRESSED_THIS_FRAME, m_pParent, dae::MainMenuCommand::MainMenuInput::CONFIRM);
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

void dae::MainMenuComponent::Render() const
{
}

void dae::MainMenuComponent::ChangeState(std::unique_ptr<MainMenuState> pNewState)
{
	m_pSwitchState = std::move(pNewState);
}