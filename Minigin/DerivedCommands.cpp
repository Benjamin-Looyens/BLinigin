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

