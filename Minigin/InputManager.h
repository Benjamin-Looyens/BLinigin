#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
//#include <utility>

namespace dae
{
	class Command;
	class GameObject;


	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddCommand(std::shared_ptr<Command> pCommand, int code, bool isController = true);

	private:
		std::vector<std::shared_ptr<Command>> m_pCommands;
		std::vector<int> m_Codes;
		std::vector<bool> m_IsController;

	};

}
