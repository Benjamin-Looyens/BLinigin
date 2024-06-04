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

	struct CommandData
	{
		std::unique_ptr<Command> pCommand;
		int commandCode;
		bool isController; // HACK : Can probably get rid of this and use custom Macro parser that has controller and keyboard both
		InputState inputState = InputState::HELD;
	};

	template<typename CommandType>
	concept DerivedCommand = requires(CommandType command) { std::derived_from<CommandType, Command>; };

	// TODO : Pimpl the inputs
	// TODO : Add support for multiple players (& multiple controllers)
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddGamepad();
		glm::ivec2 GetMousePos() const { return m_MousePos; };
		glm::ivec2 GetPreviosMouseMovement() const { return m_MousePos; };

		template<DerivedCommand CommandType, typename... TArgs>
		void AddCommand(int code, const TArgs&... args) 
		{
			bool inputTypeIsController{ false };
			code = TranslateCode(code, inputTypeIsController);

			m_Commands.emplace_back(
				std::make_unique<CommandType>(args...),
				code,
				inputTypeIsController
			);
		};

		template<DerivedCommand CommandType, typename... TArgs>
		void AddCommand(int code, InputState inputState, const TArgs&... args)
		{
			bool inputTypeIsController{ false };
			code = TranslateCode(code, inputTypeIsController);

			m_Commands.emplace_back(
				std::make_unique<CommandType>(args...),
				code,
				inputTypeIsController,
				inputState
			);
		};

	private:
		std::vector<CommandData> m_Commands; 
		std::vector<Gamepad> m_Gamepads;
		
		glm::ivec2 m_MousePos;
		glm::ivec2 m_RelMousePos;

		int TranslateCode(const int& code, bool& inputIsController_OUT);
		void HandleMouseInput(InputState state, SDL_Event& e);
		void HandleKeyboardInput(InputState state, SDL_Event& e);
	};

}
