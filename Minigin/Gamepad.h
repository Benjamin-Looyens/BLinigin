#pragma once
#include "Defines.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include <iostream>

namespace dae 
{
	class Gamepad
	{
	public:
		Gamepad(int controllerIndex);

		void UpdateControllerState();

		bool ButtonPressedThisFrame(const int& buttonCode) const;
		bool ButtonReleasedThisFrame(const int& buttonCode) const;
		bool ButtonHeld(const int& buttonCode) const;

		int GetPlayerIndex() const { return m_ControllerIndex; };

		static int TranslateToUsedGamepadCode(const int& code);

	private:
		const int m_ControllerIndex;

		XINPUT_STATE m_CurrentState;
		XINPUT_STATE m_PreviousState;
	};
}