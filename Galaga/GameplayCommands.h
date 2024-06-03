#pragma once
#include "Command.h"
#include "GameObject.h"
#include "DerivedCommands.h"

namespace dae {
	class FireCommand final : public GameObjectCommand
	{
	public:
		void Execute() override;

		FireCommand(dae::GameObject* pGameObject);
	};
}