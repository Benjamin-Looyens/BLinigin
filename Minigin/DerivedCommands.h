#pragma once
#include "Command.h"
#include "glm/vec3.hpp"
#include <memory>
#include <string>

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

	class MoveCommand : public GameObjectCommand
	{
	public:
		void Execute() override;

		MoveCommand(GameObject* pGameObject, glm::vec3 movDirection, float movSpeedPerSecond = 50.0f);

	private:
		float m_Speed;
		glm::vec3 m_MoveDirection;
	};

	class ParentToggleCommand : public Command
	{
	public:
		void Execute() override;

		ParentToggleCommand(std::shared_ptr<GameObject> pGameObject, std::shared_ptr<GameObject> pParentToToggle);

	private:
		std::shared_ptr<GameObject> m_pParentToToggle;
		std::shared_ptr<GameObject> m_pGameObject;
	};

	class PrintStringCommand : public Command
	{
	public:
		void Execute() override;

		PrintStringCommand(std::string stringToPrint);

	private:
		std::string m_StringToPrint;
	};

	class NotifyObserverCommand : public GameObjectCommand
	{
	public:
		void Execute() override;

		NotifyObserverCommand(dae::GameObject* pGameObject, dae::Observer* pObserver);

	private:
		dae::Observer* m_pObserver;
	};
}
