#include "Command.h"
#include "DerivedCommands.h"
#include "GameObject.h"
#include <iostream>
#include "Timer.h"
#include "DerivedObservers.h"
#include "Events.h"

dae::GameObjectCommand::GameObjectCommand(GameObject* pActor)
	: m_pGameObject{ pActor }
{}

#pragma region MoveCommand
dae::MoveCommand::MoveCommand(GameObject* pGameObject, glm::vec3 movDirection, float movSpeedPerSecond)
	: GameObjectCommand(pGameObject), m_MoveDirection{ glm::normalize(movDirection) }, m_Speed{movSpeedPerSecond}
{}

void dae::MoveCommand::Execute()
{
	auto pGameObject = GetGameObject();

	auto pos = pGameObject->GetLocalPosition();
	pos += (m_MoveDirection * m_Speed) * dae::Timer::GetInstance().GetDeltaTime();

	pGameObject->SetLocalTranslation(pos);
}
#pragma endregion MoveCommand

#pragma region ParentToggleCommand
dae::ParentToggleCommand::ParentToggleCommand(std::shared_ptr<GameObject> pGameObject, std::shared_ptr<GameObject> pParentToToggle)
	: m_pGameObject{ pGameObject }, m_pParentToToggle{ pParentToToggle }
{}

void dae::ParentToggleCommand::Execute()
{
	if (m_pGameObject->HasParent()) {
		m_pParentToToggle->RemoveChild(m_pGameObject);
	}
	else {
		m_pParentToToggle->AddChild(m_pGameObject);
	}
}
#pragma endregion ParentToggleCommand

#pragma region PrintStringCommand
dae::PrintStringCommand::PrintStringCommand(std::string stringToPrint)
	: m_StringToPrint{ stringToPrint }
{}

void dae::PrintStringCommand::Execute()
{
	std::cout << m_StringToPrint << "\n";
}
#pragma endregion PrintStringCommand

#pragma region NotifyObserverCommand
dae::NotifyObserverCommand::NotifyObserverCommand(dae::Observer* pObserver)
	: m_pObserver{ pObserver }
{}

void dae::NotifyObserverCommand::Execute()
{
	m_pObserver->OnNotify(nullptr, Event::PLAYER_DIED);
}
#pragma endregion NotifyObserverCommand