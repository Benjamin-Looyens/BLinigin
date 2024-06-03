#pragma once
#include "Prefab.h"

namespace dae {
	class EnemyDeath final : public Prefab
	{
	public:
		// TODO : probably customize with parameters
		GameObject* AddToScene(Scene* pSceneToAddTo) override;

	private:
	};
}