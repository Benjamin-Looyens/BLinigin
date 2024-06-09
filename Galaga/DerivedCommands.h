#pragma once
#include "Command.h"
#include "glm/vec2.hpp"
#include <memory>
#include <string>
#include "Events.h"

namespace dae
{
	class GameObject;
	class IObserver;

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

		MoveCommand(GameObject* pGameObject, glm::vec2 movDirection, float movSpeedPerSecond = 50.0f);

	private:
		float m_SpawnSpeed;
		glm::vec2 m_MoveDirection;
	};

	class NotifyObserverCommand final : public GameObjectCommand
	{
	public:
		void Execute() override;

		NotifyObserverCommand(dae::GameObject* pGameObject, dae::Event eventToNotify);

	private:
		dae::Event m_EventToNotify;
	};

	class KillPlayerCommand final : public GameObjectCommand
	{
	public:
		void Execute() override;

		KillPlayerCommand(dae::GameObject* pGameObj);
	};

	class SkipLevelCommand final : public GameObjectCommand
	{
	public:
		void Execute() override;

		SkipLevelCommand(dae::GameObject* pStageManagerObj); 
	};

	class MainMenuCommand final : public GameObjectCommand
	{
	public:
		enum class MainMenuInput
		{
			UP,
			DOWN,
			CONFIRM,
			BACK
		};

		MainMenuCommand(GameObject* pMainMenuObj, dae::MainMenuCommand::MainMenuInput inputToSignal);

		void Execute() override;

	private:
		dae::MainMenuCommand::MainMenuInput m_InputToSignal;
	};

	class EnemyControlCommand final : public GameObjectCommand
	{
	public:
		enum class EnemyControlInput
		{
			FIRE,
			KAMIKAZE
		};
		
		EnemyControlCommand(GameObject* pEnemyControlObj, EnemyControlInput input = EnemyControlInput::FIRE);

		void Execute() override;

	private:
		EnemyControlInput m_Input;
	};
}
