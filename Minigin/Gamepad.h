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
		static int TranslateToUsedGamepadCode(const int& code)
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
			}

			std::cerr << "Gamepad code provided does not equate to any define!\n";
			return -1;
		}
	};
}