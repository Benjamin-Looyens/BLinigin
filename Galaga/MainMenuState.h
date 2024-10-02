#pragma once
#include "State.h"

namespace dae {
	class MainMenuComponent;

	using MainMenuState = State<MainMenuComponent>;

	class TitleState final : public MainMenuState
	{
		enum class TitlePos
		{
			PLAY,
			HIGHSCORE
		};

	public:
		~TitleState() override = default;

		std::unique_ptr<dae::MainMenuState> Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:
		TitlePos m_CurrentPos = TitlePos::PLAY;
	};

	class PlayerSelectState final : public MainMenuState
	{
		enum class PlayerSelectPos
		{
			ONE,
			TWO
		};

	public:
		~PlayerSelectState() override = default;

		std::unique_ptr<MainMenuState> Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:
		PlayerSelectPos m_CurrentPos = PlayerSelectPos::ONE;
	};

	class ModeSelectState final : public MainMenuState
	{
		enum class ModeSelectPos
		{
			COOP,
			VERSUS
		};

	public:
		~ModeSelectState() override = default;

		std::unique_ptr<MainMenuState> Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:
		ModeSelectPos m_CurrentPos = ModeSelectPos::COOP;
	};

	class HighScoreViewState final : public MainMenuState
	{
	public:
		~HighScoreViewState() override = default;

		std::unique_ptr<MainMenuState> Update(MainMenuComponent& stateObject) override;
		void OnEnter(MainMenuComponent& stateObject) override;
		void OnExit(MainMenuComponent& stateObject) override;

	private:
		static bool s_LoadedScores;
	};
}