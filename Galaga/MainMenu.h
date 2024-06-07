#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include "MainMenuState.h"

namespace dae {
	class GameObject;

	class MainMenuComponent final : public Component
	{
		friend class MainMenuCommand;

	public:
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		void ChangeState(std::unique_ptr<MainMenuState> pNewState);

		inline bool WasUpPressed() const { return m_UpPressedThisFrame; };
		inline bool WasDownPressed() const { return m_DownPressedThisFrame; };
		inline bool WasBackPressed() const { return m_BackPressedThisFrame; };
		inline bool WasConfirmPressed() const { return m_ConfirmPressedThisFrame; }

		MainMenuComponent(GameObject* pParent);

	private:
		//static constexpr glm::vec2 t = { 5, 5 };

		void UpPressed() { m_UpPressedThisFrame = true; };
		void DownPressed() { m_DownPressedThisFrame = true; };
		void ConfirmPressed() { m_ConfirmPressedThisFrame = true; };
		void BackPressed() { m_BackPressedThisFrame = true; };

		bool m_UpPressedThisFrame{};
		bool m_DownPressedThisFrame{};
		bool m_BackPressedThisFrame{};
		bool m_ConfirmPressedThisFrame{};

		std::unique_ptr<MainMenuState> m_pState;
		std::unique_ptr<MainMenuState> m_pSwitchState;
	};
}

