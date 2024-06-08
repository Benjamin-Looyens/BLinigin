#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
#include <algorithm>

#include "Command.h"
#include "Defines.h"
#include "Gamepad.h"

bool dae::InputManager::ProcessInput()
{
    if (m_NeedToQuit) return false;

	// Keyboard & Mouse Input
	SDL_Event e;

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
    const Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

    // TODO : Some sort of system to make sure the controller does exist and stuff
	// TODO : Pimpl the controller

	for (auto& gamepad : m_Gamepads) gamepad.UpdateControllerState();

	// HACK : Please if you have time, find some other way to do this because good lord
    for (auto& command : m_Commands) {
        if (command.index > 0) {
            for (auto& gamepad : m_Gamepads) {
                if (static_cast<int>(command.index) != gamepad.GetPlayerIndex() + 1) continue; 

                switch (command.inputState) {
                case dae::InputState::PRESSED_THIS_FRAME:
                    if (gamepad.ButtonPressedThisFrame(command.commandCode)) command.pCommand->Execute();
                    break;
                case dae::InputState::RELEASED_THIS_FRAME:
                    if (gamepad.ButtonReleasedThisFrame(command.commandCode)) command.pCommand->Execute();
                    break;
                case dae::InputState::HELD:
                    if (gamepad.ButtonHeld(command.commandCode)) command.pCommand->Execute();
                    break;
                }
            }
        }
        else {
            switch (command.inputState) {
            case dae::InputState::HELD:
                if (pStates[static_cast<SDL_Scancode>(command.commandCode)])
                    command.pCommand->Execute();
                else if (mouseState & SDL_BUTTON(command.commandCode)) // TODO : Check that no scenario causes problems where the code is overlapping with SDL_SCANCODE and SDL_BUTTON
                    command.pCommand->Execute();
                break;
            }
        }
    }
    
    bool typed = false;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
            return false;
			break;
		case SDL_KEYDOWN:
            if (e.key.repeat == 0) HandleKeyboardInput(dae::InputState::PRESSED_THIS_FRAME, e);
			break;
		case SDL_KEYUP:
            HandleKeyboardInput(dae::InputState::RELEASED_THIS_FRAME, e);
			break;
		case SDL_MOUSEBUTTONDOWN:
            HandleMouseInput(dae::InputState::PRESSED_THIS_FRAME, e);
			break;
		case SDL_MOUSEBUTTONUP:
            HandleMouseInput(dae::InputState::RELEASED_THIS_FRAME, e);
			break;
		case SDL_MOUSEMOTION:
            m_MousePos.x = e.motion.x;
            m_MousePos.y = e.motion.y;

            m_RelMousePos.x = e.motion.xrel;
            m_RelMousePos.y = e.motion.yrel;
			break;
        case SDL_TEXTINPUT:
            m_LastTyped = e.text.text[0];
            typed = true;
            break;
		}

		// ImGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

    if (!typed) {
        m_LastTyped = '\0';
    }

	return true;
}

dae::input_index dae::InputManager::AddGamepad()
{
    if (m_Gamepads.size() == 4) {
#ifndef NDEBUG
        std::cerr << "Maximum amount of gamepads (4) have already been added!\n";
#endif
    }
    else {
	    m_Gamepads.push_back(static_cast<int>(m_Gamepads.size()));
    }
    return static_cast<dae::input_index>(m_Gamepads.size());
}

int dae::InputManager::TranslateCode(const int& code, bool& inputIsController_OUT)
{
    if (code & BL_KEYBOARD_INPUT) {
        inputIsController_OUT = false;

        // HACK : God, Fix this :)
        switch (code) {
        case BL_KEYBOARD_A:
            return SDL_SCANCODE_A;
            break;
        case BL_KEYBOARD_B:
            return SDL_SCANCODE_B;
            break;
        case BL_KEYBOARD_C:
            return SDL_SCANCODE_C;
            break;
        case BL_KEYBOARD_D:
            return SDL_SCANCODE_D;
            break;
        case BL_KEYBOARD_E:
            return SDL_SCANCODE_E;
            break;
        case BL_KEYBOARD_F:
            return SDL_SCANCODE_F;
            break;
        case BL_KEYBOARD_G:
            return SDL_SCANCODE_G;
            break;
        case BL_KEYBOARD_H:
            return SDL_SCANCODE_H;
            break;
        case BL_KEYBOARD_I:
            return SDL_SCANCODE_I;
            break;
        case BL_KEYBOARD_J:
            return SDL_SCANCODE_J;
            break;
        case BL_KEYBOARD_K:
            return SDL_SCANCODE_K;
            break;
        case BL_KEYBOARD_L:
            return SDL_SCANCODE_L;
            break;
        case BL_KEYBOARD_M:
            return SDL_SCANCODE_M;
            break;
        case BL_KEYBOARD_N:
            return SDL_SCANCODE_N;
            break;
        case BL_KEYBOARD_O:
            return SDL_SCANCODE_O;
            break;
        case BL_KEYBOARD_P:
            return SDL_SCANCODE_P;
            break;
        case BL_KEYBOARD_Q:
            return SDL_SCANCODE_Q;
            break;
        case BL_KEYBOARD_R:
            return SDL_SCANCODE_R;
            break;
        case BL_KEYBOARD_S:
            return SDL_SCANCODE_S;
            break;
        case BL_KEYBOARD_T:
            return SDL_SCANCODE_T;
            break;
        case BL_KEYBOARD_U:
            return SDL_SCANCODE_U;
            break;
        case BL_KEYBOARD_V:
            return SDL_SCANCODE_V;
            break;
        case BL_KEYBOARD_W:
            return SDL_SCANCODE_W;
            break;
        case BL_KEYBOARD_X:
            return SDL_SCANCODE_X;
            break;
        case BL_KEYBOARD_Y:
            return SDL_SCANCODE_Y;
            break;
        case BL_KEYBOARD_Z:
            return SDL_SCANCODE_Z;
            break;
        case BL_KEYBOARD_F1:
            return SDL_SCANCODE_F1;
            break;
        case BL_KEYBOARD_F2:
            return SDL_SCANCODE_F2;
            break;
        case BL_KEYBOARD_F3:
            return SDL_SCANCODE_F3;
            break;
        case BL_KEYBOARD_F4:
            return SDL_SCANCODE_F4;
            break;
        case BL_KEYBOARD_F5:
            return SDL_SCANCODE_F5;
            break;
        case BL_KEYBOARD_F6:
            return SDL_SCANCODE_F6;
            break;
        case BL_KEYBOARD_F7:
            return SDL_SCANCODE_F7;
            break;
        case BL_KEYBOARD_F8:
            return SDL_SCANCODE_F8;
            break;
        case BL_KEYBOARD_F9:
            return SDL_SCANCODE_F9;
            break;
        case BL_KEYBOARD_F10:
            return SDL_SCANCODE_F10;
            break;
        case BL_KEYBOARD_F11:
            return SDL_SCANCODE_F11;
            break;
        case BL_KEYBOARD_F12:
            return SDL_SCANCODE_F12;
            break;
        case BL_KEYBOARD_F13:
            return SDL_SCANCODE_F13;
            break;
        case BL_KEYBOARD_SPACE:
            return SDL_SCANCODE_SPACE;
            break;
        case BL_KEYBOARD_ENTER:
            return SDL_SCANCODE_RETURN;
            break;
        case BL_KEYBOARD_LSHIFT:
            return SDL_SCANCODE_LSHIFT;
            break;
        case BL_KEYBOARD_RSHIFT:
            return SDL_SCANCODE_RSHIFT;
            break;
        case BL_KEYBOARD_TAB:
            return SDL_SCANCODE_TAB;
            break;
        case BL_KEYBOARD_ESCAPE:
            return SDL_SCANCODE_ESCAPE;
            break;
        case BL_KEYBOARD_DELETE:
            return SDL_SCANCODE_DELETE;
            break;
        case BL_KEYBOARD_BACKSPACE:
            return SDL_SCANCODE_BACKSPACE;
            break;
        case BL_KEYBOARD_1:
            return SDL_SCANCODE_1;
            break;
        case BL_KEYBOARD_2:
            return SDL_SCANCODE_2;
            break;
        case BL_KEYBOARD_3:
            return SDL_SCANCODE_3;
            break;
        case BL_KEYBOARD_4:
            return SDL_SCANCODE_4;
            break;
        case BL_KEYBOARD_5:
            return SDL_SCANCODE_5;
            break;
        case BL_KEYBOARD_6:
            return SDL_SCANCODE_6;
            break;
        case BL_KEYBOARD_7:
            return SDL_SCANCODE_7;
            break;
        case BL_KEYBOARD_8:
            return SDL_SCANCODE_8;
            break;
        case BL_KEYBOARD_9:
            return SDL_SCANCODE_9;
            break;
        case BL_KEYBOARD_0:
            return SDL_SCANCODE_0;
            break;
        }
    }
    else if (code & BL_GAMEPAD_INPUT) {
        inputIsController_OUT = true;
        return Gamepad::TranslateToUsedGamepadCode(code);
    }
    else if (code & BL_MOUSE_INPUT) {
        inputIsController_OUT = false;
        switch (code) {
        case BL_MOUSE_LMB:
            return SDL_BUTTON_LEFT;
            break;
        case BL_MOUSE_MMB:
            return SDL_BUTTON_MIDDLE;
            break;
        case BL_MOUSE_RMB:
            return SDL_BUTTON_RIGHT;
            break;
        case BL_MOUSE_X1:
            return SDL_BUTTON_X1;
            break;
        case BL_MOUSE_X2:
            return SDL_BUTTON_X2;
            break;
        }
    }

	std::cerr << "No translated code was found!\n";
	return -1;
}

void dae::InputManager::HandleMouseInput(const dae::InputState state, SDL_Event& e)
{
    for (auto& command : m_Commands) {
        if (command.index > 0) continue;

        if (command.inputState == state) 
            if (e.button.button == command.commandCode) 
                command.pCommand->Execute();
    }
}

void dae::InputManager::HandleKeyboardInput(dae::InputState state, SDL_Event& e)
{
    for (auto& command : m_Commands) {
        if (command.index > 0) continue;

        if (command.inputState == state)
            if (e.key.keysym.scancode == command.commandCode)
                command.pCommand->Execute();
    }
}
