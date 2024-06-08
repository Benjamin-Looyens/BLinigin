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
#include "StageComponents.h"
#include "MainMenu.h"

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
	
	if (BL_SCENE_MANAGER.IsActiveScene(GetGameObject()->GetContainingScene())) {
		auto pos = pGameObject->GetLocalPosition();
		pos += (m_MoveDirection * m_SpawnSpeed) * BL_TIMER.GetDeltaTime();

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
	bool sceneActive = BL_SCENE_MANAGER.IsActiveScene(GetGameObject()->GetContainingScene());
	bool enabled = GetGameObject()->IsEnabled();
	if (!sceneActive || !enabled) return;

	auto pSubjectComp = GetGameObject()->GetComponent<dae::SubjectComponent>();

	if (pSubjectComp)
		pSubjectComp->NotifyObservers(m_EventToNotify);
}
#pragma endregion NotifyObserverCommand

void dae::KillPlayerCommand::Execute()
{
	bool sceneActive = BL_SCENE_MANAGER.IsActiveScene(GetGameObject()->GetContainingScene());
	bool enabled = GetGameObject()->IsEnabled();
	if (!sceneActive || !enabled) return;

	auto pPlayerLogic = GetGameObject()->GetComponent<dae::PlayerLogic>();
	
	if (pPlayerLogic) pPlayerLogic->Die();
}

dae::KillPlayerCommand::KillPlayerCommand(dae::GameObject* pGameObj)
	: GameObjectCommand(pGameObj)
{}

void dae::SkipLevelCommand::Execute()
{
	bool sceneActive = BL_SCENE_MANAGER.IsActiveScene(GetGameObject()->GetContainingScene());
	bool enabled = GetGameObject()->IsEnabled();
	if (!sceneActive || !enabled) return;

	auto pStageLogic = GetGameObject()->GetComponent<dae::StageManagerLogic>();
	
	if (pStageLogic) pStageLogic->SkipStage();
}

dae::SkipLevelCommand::SkipLevelCommand(dae::GameObject* pStageManagerObj)
	: GameObjectCommand(pStageManagerObj)
{}

dae::MainMenuCommand::MainMenuCommand(GameObject* pMainMenuObj, dae::MainMenuCommand::MainMenuInput inputToSignal)
	: GameObjectCommand(pMainMenuObj), m_InputToSignal{ inputToSignal }
{}

void dae::MainMenuCommand::Execute()
{
	bool mainMenuActive = BL_SCENE_MANAGER.IsActiveScene(GetGameObject()->GetContainingScene());
	bool enabled = GetGameObject()->IsEnabled();
	if (!mainMenuActive || !enabled) return;
	
	auto pMenu = GetGameObject()->GetComponent<dae::MainMenuComponent>();

	if (!pMenu) return;

	switch (m_InputToSignal) {
	case MainMenuInput::UP:
		pMenu->UpPressed();
		break;
	case MainMenuInput::DOWN:
		pMenu->DownPressed();
		break;
	case MainMenuInput::CONFIRM:
		pMenu->ConfirmPressed();
		break;
	case MainMenuInput::BACK:
		pMenu->BackPressed();
		break;
	}
}
