#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include "DerivedCommands.h"
#include "Defines.h"
#include "Gamepad.h"

bool dae::InputManager::ProcessInput()
{
	// Keyboard & Mouse Input
	SDL_Event e;

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	// XInput Controller Input
	XINPUT_STATE currentState{};
	int controllerIndex{ };

	// TODO : Some sort of system to make sure the controller does exist and stuff
	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	XInputGetState(controllerIndex, &currentState);

	// HACK : Please if you have time, find some other way to do this because good lord
	for (size_t index{}; index < m_Commands.size(); ++index) {
		if (m_Commands[index].isController) {
			if (currentState.Gamepad.wButtons & m_Commands[index].commandCode) {
				m_Commands[index].pCommand->Execute();
			}
		}
		else {
			if (pStates[static_cast<SDL_Scancode>(m_Commands[index].commandCode)]) {
				m_Commands[index].pCommand->Execute();
			}
		}
	}

	// TODO : Add system for button presses, releases and being held down
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			//if (e.key.keysym.scancode == SDL_SCANCODE_H) std::cout << "H"; // Example
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}

		// ImGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

 int dae::InputManager::TranslateCode(const int& code, bool& inputIsController_OUT)
{
	if (code & BL_KEYBOARD_INPUT) {
		inputIsController_OUT = false;

		switch (code) {
		case BL_KEYBOARD_W:
			return SDL_SCANCODE_W;
			break;
		case BL_KEYBOARD_A:
			return SDL_SCANCODE_A;
			break;
		case BL_KEYBOARD_S:
			return SDL_SCANCODE_S;
			break;
		case BL_KEYBOARD_D:
			return SDL_SCANCODE_D;
			break;
		case BL_KEYBOARD_SPACE:
			return SDL_SCANCODE_SPACE;
			break;
		}
	}
	else if (code & BL_GAMEPAD_INPUT) {
		inputIsController_OUT = true;
		return Gamepad::TranslateToUsedGamepadCode(code);
	}

	std::cerr << "No translated code was found!\n";
	return -1;
}
