#pragma once
#include "State.h"

namespace dae {
	class MainMenuComponent;

	using MainMenuState = State<MainMenuComponent>;

	class TitleState final : public MainMenuState
	{
	public:
		~TitleState() override = default;

		void Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:

	};

	class PlayerSelectState final : public MainMenuState
	{
	public:
		~PlayerSelectState() override = default;

		void Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:

	};

	class ModeSelectState final : public MainMenuState
	{
	public:
		~ModeSelectState() override = default;

		void Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:

	};

	class HighScoreViewState final : public MainMenuState
	{
	public:
		~HighScoreViewState() override = default;

		void Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:

	};
}