#include "Gamepad.h"

dae::Gamepad::Gamepad(int controllerIndex)
	: m_ControllerIndex{ controllerIndex }
{}

void dae::Gamepad::UpdateControllerState()
{
	ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
	m_PreviousState = m_CurrentState;

	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);
}

bool dae::Gamepad::ButtonPressedThisFrame(const int& buttonCode) const
{
	bool pressed{};

	if (m_CurrentState.Gamepad.wButtons & buttonCode) pressed = true;

	if (pressed)
		if (m_PreviousState.Gamepad.wButtons & buttonCode) pressed = false;

    return pressed;
}

bool dae::Gamepad::ButtonReleasedThisFrame(const int& buttonCode) const
{
	bool released{};

	if (!(m_CurrentState.Gamepad.wButtons & buttonCode))
		released = true;

	if (released)
		if (!(m_PreviousState.Gamepad.wButtons & buttonCode))
			released = false;

	return released;
}

bool dae::Gamepad::ButtonHeld(const int& buttonCode) const
{
	return (m_CurrentState.Gamepad.wButtons & buttonCode);
}

int dae::Gamepad::TranslateToUsedGamepadCode(const int& code)
{
    switch (code) {
    case BL_GAMEPAD_DPAD_UP:
        return XINPUT_GAMEPAD_DPAD_UP;
        break;
    case BL_GAMEPAD_DPAD_DOWN:
        return XINPUT_GAMEPAD_DPAD_DOWN;
        break;
    case BL_GAMEPAD_DPAD_LEFT:
        return XINPUT_GAMEPAD_DPAD_LEFT;
        break;
    case BL_GAMEPAD_DPAD_RIGHT:
        return XINPUT_GAMEPAD_DPAD_RIGHT;
        break;
    case BL_GAMEPAD_A:
        return XINPUT_GAMEPAD_A;
        break;
    case BL_GAMEPAD_B:
        return XINPUT_GAMEPAD_B;
        break;
    case BL_GAMEPAD_X:
        return XINPUT_GAMEPAD_X;
        break;
    case BL_GAMEPAD_Y:
        return XINPUT_GAMEPAD_Y;
        break;
    case BL_GAMEPAD_START:
        return XINPUT_GAMEPAD_START;
        break;
    case BL_GAMEPAD_SELECT:
        return XINPUT_GAMEPAD_BACK;
        break;
    case BL_GAMEPAD_L_THUMB:
        return XINPUT_GAMEPAD_LEFT_THUMB;
        break;
    case BL_GAMEPAD_R_THUMB:
        return XINPUT_GAMEPAD_RIGHT_THUMB;
        break;
    case BL_GAMEPAD_L_BUMPER:
        return XINPUT_GAMEPAD_LEFT_SHOULDER;
        break;
    case BL_GAMEPAD_R_BUMPER:
        return XINPUT_GAMEPAD_RIGHT_SHOULDER;
        break;
    }

	std::cerr << "Gamepad code provided does not equate to any define!\n";
	return -1;
}
