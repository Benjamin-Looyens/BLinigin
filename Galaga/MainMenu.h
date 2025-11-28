#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include "MainMenuState.h"

namespace dae {
	class GameObject;
	class TextComponent;
	class HighScoreComponent;
	class Font;

	class MainMenuComponent final : public Component
	{
		friend class MainMenuCommand;

	public:
		void Update() override;
		void LateUpdate() override;


		inline bool WasUpPressed() const { return m_UpPressedThisFrame; };
		inline bool WasDownPressed() const { return m_DownPressedThisFrame; };
		inline bool WasBackPressed() const { return m_BackPressedThisFrame; };
		inline bool WasConfirmPressed() const { return m_ConfirmPressedThisFrame; }

		void SetSelectorElevation(float newElevation);

		void SetTopText(std::string newText);
		void SetBotText(std::string newText);

		static constexpr float GetTopElevation() { return s_TOP_ELEVATION; };
		static constexpr float GetBotElevation() { return s_BOT_ELEVATION; };

		void InitHighScore(const std::string& scoreFile);
		void SetHighScoreVisible(bool newVisible);
		inline HighScoreComponent* GetHighScoreComp() const { return m_pHighScoreComp; };

		MainMenuComponent(GameObject* pParent);

	private:
		void ChangeState(std::unique_ptr<MainMenuState> pNewState);
		//static constexpr glm::vec2 t = { 5, 5 };

		void UpPressed() { m_UpPressedThisFrame = true; };
		void DownPressed() { m_DownPressedThisFrame = true; };
		void ConfirmPressed() { m_ConfirmPressedThisFrame = true; };
		void BackPressed() { m_BackPressedThisFrame = true; };

		bool m_UpPressedThisFrame{};
		bool m_DownPressedThisFrame{};
		bool m_BackPressedThisFrame{};
		bool m_ConfirmPressedThisFrame{};

		std::unique_ptr<MainMenuState> m_pState{};
		std::unique_ptr<MainMenuState> m_pSwitchState{};

		float m_SelectionElevation{};
		static constexpr float s_SELECTION_X{ 200.0f };

		static constexpr float s_OPTION_X{ s_SELECTION_X + 25.0f };
		static constexpr float s_TOP_ELEVATION{ 500.0f };
		static constexpr float s_BOT_ELEVATION{ 550.0f };

		Font* m_pUsedMenuFont{};

		GameObject* m_pSelectionObj{};

		TextComponent* m_pTopOptionComp{};
		TextComponent* m_pBottomOptionComp{};

		GameObject* m_pHighScoreObj{};
		HighScoreComponent* m_pHighScoreComp{};
	};
}

