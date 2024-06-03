#include "GameplayCommands.h"
#include "PlayerComponents.h"

dae::FireCommand::FireCommand(dae::GameObject* pGameObject)
	: GameObjectCommand(pGameObject)
{}

void dae::FireCommand::Execute()
{
	auto pGameObject = GetGameObject();
	auto pPlayerLogic = pGameObject->GetComponent<dae::PlayerLogic>();

	if (pPlayerLogic) pPlayerLogic->Fire();
}