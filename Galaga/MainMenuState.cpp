#include "MainMenuState.h"
#include <iostream>
#include "MainMenu.h"
#include "SceneManager.h"

#pragma region TitleState
void dae::TitleState::Update(MainMenuComponent& stateObject)
{
	if (stateObject.WasConfirmPressed()) {
		BL_SCENE_MANAGER.SetActiveScene("CoopGalaga");
	}
}

void dae::TitleState::OnEnter(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "TitleState enter.\n";
#endif
}

void dae::TitleState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "TitleState exit.\n";
#endif
}
#pragma endregion TitleState

#pragma region PlayerSelectState
void dae::PlayerSelectState::Update(MainMenuComponent&)
{
}

void dae::PlayerSelectState::OnEnter(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "PlayerSelectState enter.\n";
#endif
}

void dae::PlayerSelectState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "PlayerSelectState exit.\n";
#endif
}
#pragma endregion PlayerSelectState

#pragma region ModeSelectState
void dae::ModeSelectState::Update(MainMenuComponent&)
{
}

void dae::ModeSelectState::OnEnter(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "ModeSelectState enter.\n";
#endif
}

void dae::ModeSelectState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "ModeSelectState exit.\n";
#endif
}
#pragma endregion ModeSelectState

#pragma region HighScoreViewState
void dae::HighScoreViewState::Update(MainMenuComponent&)
{
}

void dae::HighScoreViewState::OnEnter(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "HighScoreState enter.\n";
#endif
}

void dae::HighScoreViewState::OnExit(MainMenuComponent&)
{
#ifndef NDEBUG
	std::cout << "HighScoreState exit.\n";
#endif
}
#pragma endregion HighScoreViewState
