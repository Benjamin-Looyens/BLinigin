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


	for (auto& gamepad : m_Gamepads) gamepad.UpdateControllerState();

	// HACK : Please if you have time, find some other way to do this because good lord
	for (size_t index{}; index < m_Commands.size(); ++index) {
		if (m_Commands[index].isController) {
			for (auto& gamepad : m_Gamepads) {
				switch (m_Commands[index].inputState) {
				case InputState::PRESSED_THIS_FRAME:
					if (gamepad.ButtonPressedThisFrame(m_Commands[index].commandCode))
						m_Commands[index].pCommand->Execute();
					break;
				case InputState::RELEASED_THIS_FRAME:
					if (gamepad.ButtonReleasedThisFrame(m_Commands[index].commandCode))
						m_Commands[index].pCommand->Execute();
					break;
				case InputState::HELD:
					if (gamepad.ButtonHeld(m_Commands[index].commandCode))
						m_Commands[index].pCommand->Execute();
					break;
				}
			}
		}
		else {
			switch (m_Commands[index].inputState) {
			case InputState::HELD:
				if (pStates[static_cast<SDL_Scancode>(m_Commands[index].commandCode)]) 
					m_Commands[index].pCommand->Execute();
				break;
			}
		}
	}

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			//if (e.key.keysym.scancode == SDL_SCANCODE_H) std::cout << "H"; // Example

			// TODO : Fix pressed this frame for Keyboard input
			if (e.key.repeat == 0) {
				for (auto& command : m_Commands) {
					if (command.isController) continue;
					switch (command.inputState) {
					case InputState::PRESSED_THIS_FRAME:
						if (e.key.keysym.scancode == command.commandCode)
							command.pCommand->Execute();
						break;
					}
				}
			}
		}
		if (e.type == SDL_KEYUP) {
			for (auto& command : m_Commands) {
				if (command.isController) continue;
				switch (command.inputState) {
				case InputState::RELEASED_THIS_FRAME:
					if (e.key.keysym.scancode == command.commandCode)
						command.pCommand->Execute();
					break;
				}
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}

		// ImGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::AddGamepad()
{
	m_Gamepads.push_back(static_cast<int>(m_Gamepads.size()));
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
