#include "MainMenuState.h"
#include <iostream>
#include "MainMenu.h"
#include "SceneManager.h"
#include "InputManager.h"

#pragma region TitleState
void dae::TitleState::Update(MainMenuComponent& stateObject)
{
	switch (m_CurrentPos) {
	case TitlePos::PLAY:
		if (stateObject.WasConfirmPressed()) {
			stateObject.ChangeState(std::make_unique<dae::PlayerSelectState>());
		}
		if (stateObject.WasDownPressed()) {
			stateObject.SetSelectorElevation(stateObject.GetBotElevation());
			m_CurrentPos = TitlePos::HIGHSCORE;
		}
		break;
	case TitlePos::HIGHSCORE:
		if (stateObject.WasConfirmPressed()) {
			stateObject.ChangeState(std::make_unique<dae::HighScoreViewState>());
		}
		if (stateObject.WasUpPressed()) {
			stateObject.SetSelectorElevation(stateObject.GetTopElevation());
			m_CurrentPos = TitlePos::PLAY;
		}
		break;
	}

	if (stateObject.WasBackPressed()) {
		BL_INPUT.QuitGame();
	}
}

void dae::TitleState::OnEnter(MainMenuComponent& stateObject)
{
#ifndef NDEBUG
	std::cout << "TitleState enter.\n";
#endif

	stateObject.SetSelectorElevation(stateObject.GetTopElevation());
	stateObject.SetTopText("Play");
	stateObject.SetBotText("High Scores");
}

void dae::TitleState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "TitleState exit.\n";
#endif
}
#pragma endregion TitleState

#pragma region PlayerSelectState
void dae::PlayerSelectState::Update(MainMenuComponent& stateObject)
{
	switch (m_CurrentPos) {
	case PlayerSelectPos::ONE:
		if (stateObject.WasConfirmPressed()) {
			BL_SCENE_MANAGER.SetActiveScene("SoloGalaga");
		}

		if (stateObject.WasDownPressed()) {
			stateObject.SetSelectorElevation(stateObject.GetBotElevation());
			m_CurrentPos = PlayerSelectPos::TWO;
		}
		break;
	case PlayerSelectPos::TWO:
		if (stateObject.WasConfirmPressed()) {
			stateObject.ChangeState(std::make_unique<dae::ModeSelectState>());
		}
		if (stateObject.WasUpPressed()) {
			stateObject.SetSelectorElevation(stateObject.GetTopElevation());
			m_CurrentPos = PlayerSelectPos::ONE;
		}
		break;
	}

	if (stateObject.WasBackPressed()) {
		stateObject.ChangeState(std::make_unique<dae::TitleState>());
	}
}

void dae::PlayerSelectState::OnEnter(MainMenuComponent& stateObject)
{
#ifndef NDEBUG
	std::cout << "PlayerSelectState enter.\n";
#endif

	stateObject.SetSelectorElevation(stateObject.GetTopElevation());
	stateObject.SetTopText("1 Player");
	stateObject.SetBotText("2 Players");
}

void dae::PlayerSelectState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "PlayerSelectState exit.\n";
#endif
}
#pragma endregion PlayerSelectState

#pragma region ModeSelectState
void dae::ModeSelectState::Update(MainMenuComponent& stateObject)
{
	switch (m_CurrentPos) {
	case ModeSelectPos::COOP:
		if (stateObject.WasDownPressed()) {
			stateObject.SetSelectorElevation(stateObject.GetBotElevation());
			m_CurrentPos = ModeSelectPos::VERSUS;
		}
		
		if (stateObject.WasConfirmPressed()) {
			BL_SCENE_MANAGER.SetActiveScene("CoopGalaga");
		}
		break;
	case ModeSelectPos::VERSUS:
		if (stateObject.WasUpPressed()) {
			stateObject.SetSelectorElevation(stateObject.GetTopElevation());
			m_CurrentPos = ModeSelectPos::COOP;
		}
		if (stateObject.WasConfirmPressed()) {
			BL_SCENE_MANAGER.SetActiveScene("VersusGalaga");
		}
		break;
	}

	if (stateObject.WasBackPressed()) {
		stateObject.ChangeState(std::make_unique<dae::PlayerSelectState>());
	}
}

void dae::ModeSelectState::OnEnter(MainMenuComponent& stateObject)
{
#ifndef NDEBUG
	std::cout << "ModeSelectState enter.\n";
#endif

	stateObject.SetSelectorElevation(stateObject.GetTopElevation());
	stateObject.SetTopText("Coop");
	stateObject.SetBotText("Versus");
}

void dae::ModeSelectState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "ModeSelectState exit.\n";
#endif
}
#pragma endregion ModeSelectState

#pragma region HighScoreViewState
void dae::HighScoreViewState::Update(MainMenuComponent& stateObject)
{
	if (stateObject.WasBackPressed()) {
		stateObject.ChangeState(std::make_unique<dae::TitleState>());
	}
}

void dae::HighScoreViewState::OnEnter(MainMenuComponent& stateObject)
{
#ifndef NDEBUG
	std::cout << "HighScoreState enter.\n";
#endif

	stateObject.SetSelectorElevation(-50.0f); // Put it off screen because no selections here
	stateObject.SetTopText("");
	stateObject.SetBotText("");
}

void dae::HighScoreViewState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "HighScoreState exit.\n";
#endif
}
#pragma endregion HighScoreViewState
