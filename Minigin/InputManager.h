#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Command.h"
#include "Gamepad.h"
#include <glm/vec2.hpp>

union SDL_Event;

namespace dae
{
	class Command;
	class GameObject;
	class Gamepad;
	
	enum class InputState
	{
		PRESSED_THIS_FRAME,
		RELEASED_THIS_FRAME,
		HELD
	};

	using input_index = unsigned short;

	struct CommandData
	{
		std::unique_ptr<dae::Command> pCommand;
		int commandCode;
		//bool isController; // HACK : Can probably get rid of this and use custom Macro parser that has controller and keyboard both
		input_index index; // Index 0 -> KBM, Index 1-4 -> Controllers 1-4
		dae::InputState inputState = dae::InputState::HELD;
	};

	// TODO : Potentially a wrapper class that handles input which itself handles whether it gets it from a controller or not

	template<typename CommandType>
	concept DerivedCommand = requires(CommandType command) { std::derived_from<CommandType, Command>; };

	// TODO : Pimpl the inputs
	// TODO : Add support for multiple players (& multiple controllers)
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		input_index AddGamepad();
		glm::ivec2 GetMousePos() const { return m_MousePos; };
		glm::ivec2 GetPreviosMouseMovement() const { return m_MousePos; };

		inline void QuitGame() { m_NeedToQuit = true; };

		template<DerivedCommand CommandType, typename... TArgs>
		void AddCommand(int code, input_index index, const TArgs&... args) 
		{
			bool inputTypeIsController{ false };
			code = TranslateCode(code, inputTypeIsController);

			m_Commands.emplace_back(
				std::make_unique<CommandType>(args...),
				code,
				index
			);
		};

		template<DerivedCommand CommandType, typename... TArgs>
		void AddCommand(int code, input_index index, dae::InputState inputState, const TArgs&... args)
		{
			bool inputTypeIsController{ false };
			code = TranslateCode(code, inputTypeIsController);

			m_Commands.emplace_back(
				std::make_unique<CommandType>(args...),
				code,
				index,
				inputState
			);
		};

		inline int GetNumGamepads() const { return static_cast<int>(m_Gamepads.size()); };

		inline char GetLatestTyped() const { return m_LastTyped; };

	private:
		std::vector<dae::CommandData> m_Commands; 
		std::vector<dae::Gamepad> m_Gamepads;
		
		glm::ivec2 m_MousePos{};
		glm::ivec2 m_RelMousePos{};

		int TranslateCode(const int& code, bool& inputIsController_OUT);
		void HandleMouseInput(dae::InputState state, SDL_Event& e);
		void HandleKeyboardInput(dae::InputState state, SDL_Event& e);

		bool m_NeedToQuit = false;
		char m_LastTyped{};
	};
}

#define BL_INPUT dae::InputManager::GetInstance()