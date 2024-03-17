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

	// HACK : Please if you have time, find some other way to do this because good lord
	// HOOOOOOOLY SHIT this is bad
	for (size_t index{}; index < m_pCommands.size(); ++index) {
		if (m_IsController[index] == false) {
			if (pStates[static_cast<SDL_Scancode>(m_Codes[index])]) {
				m_pCommands[index]->Execute();
			}
		}
	}

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

	// XInput Controller Input
	XINPUT_STATE currentState{};
	int controllerIndex{ };

	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	auto result = XInputGetState(controllerIndex, &currentState);

	if (result == ERROR_SUCCESS) {
		// Controller Quit
		if (currentState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			return false;
		}

		// HACK : Please if you have time, find some other way to do this because good lord
		// HOOOOOOOLY SHIT this is bad
		for (size_t index{}; index < m_pCommands.size(); ++index) {
			if (m_IsController[index]) {
				if (currentState.Gamepad.wButtons & m_Codes[index]) {
					m_pCommands[index]->Execute();
				}
			}
		}
	}

	return true;
}

void dae::InputManager::AddCommand(std::shared_ptr<Command> pCommand, int code, bool isController)
{
	// Translate code to used gamepad code (XINPUT)
	if (isController) 
		code = Gamepad::TranslateToUsedGamepadCode(code);

	m_pCommands.push_back(pCommand);
	m_Codes.push_back(code);
	m_IsController.push_back(isController);
}