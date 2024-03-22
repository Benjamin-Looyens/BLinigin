#include "Command.h"
#include "DerivedCommands.h"
#include "GameObject.h"
#include <iostream>
#include "Timer.h"

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
