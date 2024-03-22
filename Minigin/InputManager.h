#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
//#include <utility>
#include "DerivedCommands.h"

namespace dae
{
	class Command;
	class GameObject;

	struct CommandData
	{
		std::unique_ptr<Command> pCommand;
		int commandCode;
		bool isController; // HACK : Can probably get rid of this and use custom Macro parser that has controller and keyboard both
	};

	template<typename CommandType>
	concept DerivedCommand = requires(CommandType command) { std::derived_from<CommandType, Command>; };

	// TODO : Pimpl the inputs
	// TODO : Add support for multiple players (& multiple controllers)
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

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

	private:
		std::vector<CommandData> m_Commands; 
		
		int TranslateCode(const int& code, bool& inputIsController_OUT);
	};

}
