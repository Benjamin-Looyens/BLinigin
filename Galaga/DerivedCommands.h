#pragma once
#include "Command.h"
#include "glm/vec3.hpp"
#include <memory>
#include <string>
#include "Events.h"

namespace dae
{
	class GameObject;
	class Observer;

	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(GameObject* pGameObject);
		virtual ~GameObjectCommand() = default;

	protected:
		GameObject* GetGameObject() const { return m_pGameObject; };
	
	private:
		GameObject* m_pGameObject;
	};

	class PrintStringCommand final : public Command
	{
	public:
		void Execute() override;

		PrintStringCommand(std::string stringToPrint);

	private:
		std::string m_StringToPrint;
	};

	class MoveCommand final : public GameObjectCommand
	{
	public:
		void Execute() override;

		MoveCommand(GameObject* pGameObject, glm::vec3 movDirection, float movSpeedPerSecond = 50.0f);

	private:
		float m_Speed;
		glm::vec3 m_MoveDirection;
	};

	class NotifyObserverCommand final : public GameObjectCommand
	{
	public:
		void Execute() override;

		NotifyObserverCommand(dae::GameObject* pGameObject, dae::Event eventToNotify);

	private:
		dae::Event m_EventToNotify;
	};
}
