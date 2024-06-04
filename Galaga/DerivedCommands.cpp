#include "Command.h"
#include "DerivedCommands.h"
#include "GameObject.h"
#include <iostream>
#include "Timer.h"
#include "DerivedEvents.h"
#include "DerivedComponents.h"
#include "ObserverComponents.h"
#include "PlayerComponents.h"
#include "SceneManager.h"

dae::GameObjectCommand::GameObjectCommand(GameObject* pActor)
	: m_pGameObject{ pActor }
{}

#pragma region MoveCommand
dae::MoveCommand::MoveCommand(GameObject* pGameObject, glm::vec2 movDirection, float movSpeedPerSecond)
	: GameObjectCommand(pGameObject), m_MoveDirection{ glm::normalize(movDirection) }, m_SpawnSpeed{movSpeedPerSecond}
{}

void dae::MoveCommand::Execute()
{
	auto pGameObject = GetGameObject();
	
	if (SCENE_MANAGER.IsActiveScene(GetGameObject()->GetContainingScene())) {
		auto pos = pGameObject->GetLocalPosition();
		pos += (m_MoveDirection * m_SpawnSpeed) * dae::Timer::GetInstance().GetDeltaTime();

		pGameObject->SetLocalTranslation(pos);
	}
}
#pragma endregion MoveCommand

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
dae::NotifyObserverCommand::NotifyObserverCommand(dae::GameObject* pGameObject, dae::Event eventToNotify)
	: GameObjectCommand(pGameObject), m_EventToNotify{ eventToNotify }
{}

void dae::NotifyObserverCommand::Execute()
{
	auto pSubjectComp = GetGameObject()->GetComponent<dae::SubjectComponent>();

	if (pSubjectComp)
		pSubjectComp->NotifyObservers(m_EventToNotify);
}
#pragma endregion NotifyObserverCommand